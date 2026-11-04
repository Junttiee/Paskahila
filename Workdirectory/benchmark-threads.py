#!/usr/bin/env python3
"""Measure OpenMP scaling on the current short single-nucleus test.

Run from Ubuntu/WSL after compiling the current source with -fopenmp.
These are performance runs, not the matched-source discretization study.
The source ending time must remain 0.8 for this benchmark.
"""

import argparse
import csv
import json
import math
import os
from pathlib import Path
import re
import statistics
import subprocess
import tempfile
import time


ROOT = Path(__file__).resolve().parent
TIMING = re.compile(r"#ELAPSED TIME ([0-9.eE+-]+) s FOR 10 EVOLUTION STEPS")
FINAL_TIME = re.compile(r"^#t = ([0-9.eE+-]+)$", re.MULTILINE)


def numeric_rows(path):
    rows = [[float(value) for value in line.split()]
            for line in path.read_text().splitlines() if line.strip()]
    if not rows or not all(math.isfinite(value) for row in rows for value in row):
        raise RuntimeError("Missing or non-finite numerical output: " + str(path))
    return rows


def compare_rows(actual, reference, label, absolute_tolerance=1e-12):
    if len(actual) != len(reference):
        raise RuntimeError("Output row counts differ: " + label)
    largest = 0.0
    for row, expected in zip(actual, reference):
        if len(row) != len(expected):
            raise RuntimeError("Output column counts differ: " + label)
        for value, baseline in zip(row, expected):
            largest = max(largest, abs(value - baseline))
            if not math.isclose(value, baseline, rel_tol=1e-10, abs_tol=absolute_tolerance):
                raise RuntimeError("Thread-count output mismatch: " + label)
    return largest


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--solver", type=Path,
                        default=ROOT / "build" / "RightSUNcSolver-threads")
    parser.add_argument("--threads", type=int, nargs="+", default=[1, 2, 4, 8, 16])
    parser.add_argument("--repeats", type=int, default=1)
    parser.add_argument("--resume", type=Path,
                        help="Reuse validated runs from a previous benchmark directory")
    parser.add_argument("--grid", type=int, nargs=3, default=[64, 64, 256])
    parser.add_argument("--spacings", type=float, nargs=3, default=[2, 2, 1])
    parser.add_argument("--timeout", type=float, default=240,
                        help="Maximum wall-clock seconds per run")
    args = parser.parse_args()
    if (any(n <= 0 for n in args.threads + args.grid) or args.repeats < 1
            or any(not math.isfinite(a) or a <= 0 for a in args.spacings)
            or not math.isfinite(args.timeout) or args.timeout <= 0):
        parser.error("Thread counts, repeats, dimensions, spacings and timeout must be positive")
    solver = args.solver.resolve()
    if not solver.is_file() or not os.access(solver, os.X_OK):
        parser.error("Compile an executable solver first: " + str(solver))
    parameters = (ROOT / "SourceCode" / "LATTICE" / "Parameters.cpp").read_text()
    if not re.search(r"DOUBLE\s+EndSimulationT\s*=\s*0\.8\s*;", parameters):
        parser.error("This benchmark expects the current source ending time of 0.8")

    counts = sorted(set([1] + args.threads))
    (ROOT / "runs").mkdir(exist_ok=True)
    previous = {}
    if args.resume:
        out = args.resume.resolve()
        if not out.is_dir():
            parser.error("Resume directory does not exist: " + str(out))
        previous = {(row["threads"], row["repeat"]): row for row in json.loads(
            (out / "measurements.json").read_text())}
    else:
        out = Path(tempfile.mkdtemp(prefix="thread-benchmark-", dir=ROOT / "runs"))
    records = []
    reference = None
    print("Benchmark output: " + str(out), flush=True)
    for count in counts:
        for repeat in range(1, args.repeats + 1):
            prior = previous.get((count, repeat))
            if prior:
                run_dir = Path(prior["directory"])
            else:
                run_dir = out / ("threads-{}-repeat-{}".format(count, repeat))
                if run_dir.exists():
                    run_dir = Path(tempfile.mkdtemp(
                        prefix="threads-{}-repeat-{}-retry-".format(count, repeat), dir=out))
                else:
                    run_dir.mkdir()
            command = [str(solver)]
            for flag, value in zip(["Nx", "Ny", "Nz"], args.grid):
                command += ["-" + flag, str(value)]
            for flag, value in zip(["ax", "ay", "az"], args.spacings):
                command += ["-" + flag, str(value)]
            command += ["-dT", "0.008", "-RlOverAzL", "4", "-RlOverAzR", "4",
                        "-SEED", "1", "-o", str(run_dir)]
            env = os.environ.copy()
            env.update(OMP_NUM_THREADS=str(count), OMP_DYNAMIC="FALSE",
                       OMP_MAX_ACTIVE_LEVELS="1")
            configuration = {
                "command": command,
                "environment": {key: env.get(key) for key in [
                    "OMP_NUM_THREADS", "OMP_DYNAMIC", "OMP_MAX_ACTIVE_LEVELS",
                    "OMP_PROC_BIND", "OMP_PLACES", "GOMP_CPU_AFFINITY"]},
                "purpose": "Current short-test performance benchmark; physics setup unchanged",
            }
            if prior:
                recorded = json.loads((run_dir / "command.json").read_text())
                if (recorded != configuration
                        or solver.stat().st_mtime > (run_dir / "command.json").stat().st_mtime):
                    raise RuntimeError("Resume settings or executable changed; start a fresh benchmark")
                wall = prior["wall_seconds"]
                print("Rechecking saved {}-thread run, repeat {}...".format(count, repeat), flush=True)
            else:
                (run_dir / "command.json").write_text(json.dumps(configuration, indent=2) + "\n")
                print("Running {} thread(s), repeat {}...".format(count, repeat), flush=True)
                start = time.perf_counter()
                with (run_dir / "run.log").open("w") as log:
                    subprocess.run(command, cwd=ROOT, env=env, stdout=log,
                                   stderr=subprocess.STDOUT, check=True, timeout=args.timeout)
                wall = time.perf_counter() - start
            log = (run_dir / "run.log").read_text()
            if ("#COMPLETED ID=1" not in log or "#FFTW THREAD STATUS 1" not in log
                    or "#NUMBER OF THREADS {}\n".format(count) not in log):
                raise RuntimeError("Completion or thread initialization check failed: " + str(run_dir))
            times = [float(value) for value in FINAL_TIME.findall(log)]
            if len(times) not in [100, 101] or not math.isclose(times[-1], 0.8, abs_tol=0.00801):
                raise RuntimeError("Unexpected evolution length: " + str(run_dir))
            blocks = [float(value) / 10 for value in TIMING.findall(log)]
            if len(blocks) < 3:
                raise RuntimeError("Insufficient step timings: " + str(run_dir))
            energy = numeric_rows(run_dir / "EnergyID1.txt")
            gauss = numeric_rows(run_dir / "GausCheck1.txt")
            profiles = {path.name: numeric_rows(path)
                        for path in sorted(run_dir.glob("EMTDynT*ID1.txt"))}
            if len(profiles) != 2 or any(len(rows) != args.grid[2] for rows in profiles.values()):
                raise RuntimeError("Expected initial and step-100 profiles: " + str(run_dir))
            if reference is None:
                reference = {"energy": energy, "profiles": profiles}
            # Initial transverse stresses subtract large electric/magnetic sums.
            # Scale the absolute floor to bulk energy to handle cancellation.
            energy_absolute_tolerance = 1e-12 * max(abs(reference["energy"][0][1]), 1e-30)
            differences = [compare_rows(energy, reference["energy"], "energy",
                                        energy_absolute_tolerance)]
            if profiles.keys() != reference["profiles"].keys():
                raise RuntimeError("Profile times differ across thread counts")
            differences += [compare_rows(rows, reference["profiles"][name], name)
                            for name, rows in profiles.items()]
            record = dict(threads=count, repeat=repeat, steps=len(times),
                          seconds_per_step=statistics.median(blocks[1:]),
                          wall_seconds=wall, maximum_energy_or_profile_difference=max(differences),
                          maximum_gauss_residual=max(row[1] for row in gauss),
                          directory=str(run_dir))
            records.append(record)
            print("  {:.5f} s/step; {:.2f} s total; numerical comparison passed".format(
                record["seconds_per_step"], wall), flush=True)
            (out / "measurements.json").write_text(json.dumps(records, indent=2) + "\n")

    baseline_step = statistics.median(r["seconds_per_step"] for r in records if r["threads"] == 1)
    baseline_wall = statistics.median(r["wall_seconds"] for r in records if r["threads"] == 1)
    summary = []
    for count in counts:
        selected = [r for r in records if r["threads"] == count]
        step = statistics.median(r["seconds_per_step"] for r in selected)
        wall = statistics.median(r["wall_seconds"] for r in selected)
        summary.append(dict(threads=count, seconds_per_step=step,
                            step_speedup=baseline_step / step,
                            wall_seconds=wall, wall_speedup=baseline_wall / wall))
    with (out / "summary.csv").open("w", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=list(summary[0]))
        writer.writeheader()
        writer.writerows(summary)
    (out / "summary.json").write_text(json.dumps({
        "grid": args.grid, "spacings": args.spacings, "dT": 0.008,
        "repeats": args.repeats, "summary": summary,
        "comparison_relative_tolerance": 1e-10,
        "profile_absolute_tolerance": 1e-12,
        "energy_absolute_tolerance": energy_absolute_tolerance,
        "notes": "The first timing block is omitted. Step timers exclude some diagnostics. "
                 "Wall times include initialization and output. These are not study physics runs.",
    }, indent=2) + "\n")
    print("\nThreads  Seconds/step  Step speed-up  Total seconds", flush=True)
    for row in summary:
        print("{:7d}  {:12.5f}  {:13.2f}  {:13.2f}".format(
            row["threads"], row["seconds_per_step"], row["step_speedup"], row["wall_seconds"]))
    print("Summary: " + str(out / "summary.csv"), flush=True)


if __name__ == "__main__":
    main()
