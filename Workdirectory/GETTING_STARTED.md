# Getting started with this simulation

This is a C++ research code for real-time classical Yang–Mills fields on a three-dimensional lattice, with initial conditions for individual nuclei and nucleus collisions. You run a command-line solver and inspect the text files it writes. The selected gauge group in this checkout is **SU(2)**.

This guide was updated for the small local test defaults on 4 November 2026. The user compiled and ran the solver successfully in Ubuntu under WSL 2, with GCC 15.2 and GSL 2.8 available. The saved log and output files were inspected afterward; the recorded reference values are listed below.

## 1. Start with results that already exist

You can do this part in Windows PowerShell, from `Workdirectory`, without installing anything:

```powershell
Get-Content .\timetest008Coll\InfoID0.txt
Get-Content .\timetest008Coll\EnergyID0.txt -TotalCount 6
```

The info file describes that saved run: a `128 × 128 × 512` grid, spacings `1, 1, 0.25`, and time step `0.008`. These are historical results; they differ from some current source defaults.

The first energy row is:

```text
time  T00  TXX  TYY  TZZ  SOURCE
```

Every subsequent row has a different layout:

```text
step  time  T00  SOURCE
```

Those labels are explanations; the files themselves have no column headers. For example, the saved `timetest008Coll` data gives:

| Step | Time | Bulk T00 |
| --- | --- | --- |
| Initial | 0 | 33.2117297026722 |
| 20 | 0.16 | 33.2101454286254 |
| 100 | 0.8 | 33.2098512212341 |

`T00` is the energy-related observable; `TXX`, `TYY`, and `TZZ` are diagonal stress components. The bulk implementation sums field contributions over sites without dividing by lattice volume. Its values therefore need care when comparing different grid sizes. The longitudinal profiles described below average over the transverse grid instead.

Treat time and fields as simulation quantities. The presence of conversion parameters in the code does not establish a physical unit for every output column.

## 2. Understand what will run

The flow is:

```text
SUNcSolver.cpp
  -> read command-line settings
  -> allocate lattice fields
  -> generate seeded initial conditions
  -> evolve fields and currents in time
  -> write measurements and diagnostics
```

In `SourceCode/SUNcSolver.cpp`, exactly one simulation procedure is currently included:

```cpp
#include "RightNucleusSimulation.cpp"
```

That procedure initializes the right-positioned nucleus alone. Here, "right" names its initial position, not its direction of motion. The other available procedures include:

| Procedure | Purpose |
| --- | --- |
| `RightNucleusSimulation.cpp` | One right-positioned nucleus; currently selected |
| `LeftNucleusSimulation.cpp` | One left-positioned nucleus |
| `Simulation.cpp` | Both nuclei |
| `SimulationmitPC.cpp` | Alternative setup using nuclear-structure code; investigate separately |

To change procedures, select the corresponding include and comment out the other procedure includes, then rebuild. Changing an executable's filename does not change its simulation.

The code includes other `.cpp` files directly. Compile **only `SUNcSolver.cpp`** as the top-level source; compiling every `.cpp` independently is not the intended build.

## 3. Prepare a Linux environment

The supplied `.exe` files all have Linux ELF headers. Their extension does not make them Windows applications. Rebuilding also gives you a clear connection between source settings and the executable you run.

This Windows machine has Ubuntu through [Windows Subsystem for Linux (Microsoft instructions)](https://learn.microsoft.com/en-us/windows/wsl/install). The Windows checkout is accessible from Ubuntu at the `/mnt/c/` path below, so edits and runs use the same files.

Once Ubuntu is available, run these commands **inside Ubuntu**:

```bash
sudo apt update
sudo apt install build-essential libgsl-dev libfftw3-dev
cd /mnt/c/Users/eeroj/Rojektit/Paskahila/Workdirectory
```

The solver uses a C++ compiler, OpenMP for CPU threads, [GSL](https://packages.ubuntu.com/noble/libgsl-dev) for random numbers, and [FFTW](https://packages.ubuntu.com/noble/libfftw3-dev) for Fourier transforms. The package links document Ubuntu 24.04 packages.

The supplied `Makefile` uses `mpicxx` and cluster-specific library paths. I found no MPI calls or MPI header includes in the source. The direct `g++` recipe below was used for the successful local build and avoids needing the MPI compiler wrapper.

The bundled `my-env` contains an old Python 3.6 environment. It is not needed to build or run the C++ solver.

## 4. Build a small learning run

The source defaults are now a `16 × 16 × 256` grid, ending time `0.8`, longitudinal widths `4, 4`, and seed `1`. The time step is `0.008`, giving approximately 100 evolution steps. The selected procedure is the single right-positioned nucleus with SU(2).

The previous defaults were a `128 × 128 × 2048` grid and ending time `88`. Choose production settings deliberately before using this checkout for larger experiments.

Compile from Ubuntu with:

```bash
mkdir -p build

g++ -std=gnu++14 -O2 -fopenmp \
  SourceCode/SUNcSolver.cpp \
  -o build/RightSUNcSolver-small \
  -lgsl -lgslcblas -lfftw3_omp -lfftw3 -lm
```

The ending time is set in `SourceCode/LATTICE/Parameters.cpp`; there is no command-line option for it. Rebuild after changing it.

When ready to execute, use the prepared launcher:

```bash
bash run-small.sh
```

The launcher sets `OMP_NUM_THREADS=1`, passes the small grid, spacings, time step, widths, and seed explicitly, and creates a fresh `runs/first-right-XXXXXX` output directory with a `run.log` file. Its default executable is `build/RightSUNcSolver-small`; an alternative executable path can be passed as its first argument. Compile the selected executable from the current source before launching.

This is a deliberately small exercise to learn the workflow, with narrower longitudinal profiles. It is not a validated physics configuration. Thread count is selected by the launcher; when launching the solver directly, set `OMP_NUM_THREADS=1` for the same first-test setup.

The output directory must exist before launching: the solver sets a path but does not create the directory or reliably report failed output-file opens. Use a fresh directory for each run. Some files are overwritten while `GausCheck` is appended, so rerunning into the same directory can mix results.

Look for `#COMPLETED ID=1` in `run.log`, nonempty output files, finite numerical values, and time advancing to approximately `0.8`. Floating-point arithmetic may produce a slightly different final decimal or an extra step.

This short run should write an initial energy-momentum profile and another profile at approximately time `0.8`. Each profile should contain 256 rows, one for each z site. Later profiles are written every 100 steps.

### Recorded first local run

The saved run from 4 November 2026 completed with one thread, seed `1`, SU(2), a `16 × 16 × 256` grid, and 100 evolution steps. Its log ends with `#COMPLETED ID=1`. Both energy-momentum profiles contain 256 rows and 14 columns. All numerical values in the energy, Gauss-law, and profile files were checked and are finite.

| Measurement | Initial | At step 100 |
| --- | --- | --- |
| Time | `0` | `0.800000000000001` |
| Bulk T00 | `5.07400822042214` | `5.07403056754339` |
| Maximum Gauss-law residual | `6.93889390390723e-18` | `1.28360059381494e-14` |
| RMS Gauss-law residual | `2.25208651922912e-19` | `1.35710330884308e-16` |

These values record the first working setup for later comparisons; they do not establish physical convergence or acceptance thresholds. Compiler and library differences can change floating-point results. The generated executable and full output remain local under `build/` and `runs/`, which are excluded from Git.

## 5. Know which settings are adjustable

Options are case-sensitive and use a single hyphen followed by a separate value. Unknown options are ignored; there is no implemented help-only mode. Do not use `--help` as a harmless test because it can proceed into a simulation.

| Option | Meaning / source default |
| --- | --- |
| `-o PATH` | Output directory; default `OUTPUT`, which must already exist |
| `-Nx`, `-Ny`, `-Nz` | Site counts; defaults `16, 16, 256` |
| `-N VALUE` | Set all three site counts together |
| `-ax`, `-ay`, `-az` | Spacings; defaults `1, 1, 0.25` |
| `-a VALUE` | Set all three spacings together |
| `-dT VALUE` | Time step; default `0.008` |
| `-SEED VALUE` | Random seed and output identifier; default `1` |
| `-RlOverAzL`, `-RlOverAzR` | Longitudinal profile widths in site units; defaults `4, 4` |
| `-MEff`, `-Lambda` | IR and UV cutoff values; initial defaults `0.25, 1.25` |
| `-NucleusL`, `-NucleusR` | Integer multipliers of the generated charge densities; defaults `1, 1` |

When setting dimensions or spacings independently, supply all three, as in the example. Otherwise the parser can retain the entire default triple. Alternatively, supply `-N` or `-a` first and then override individual axes.

Some settings require a rebuild: the simulation procedure and gauge group in `SUNcSolver.cpp`, boundary selection in `LATTICE/3DGrid.cpp`, and ending time in `LATTICE/Parameters.cpp`. The current indexing wraps the transverse x/y directions; the open/periodic boundary selection controls longitudinal ghost cells.

### Parameter details to check before a physics study

`-QsA` is parsed, but the selected procedure passes `Parameters::Qs0A_L` to both calls that generate initial Wilson lines. That is a separate variable with no command-line option; `Qs0A_R` is defined but not passed by these procedures. Changing `-QsA` alone therefore does not change the fluctuation amplitude used by those calls.

Several quantities, including cutoff defaults and unit-conversion values, are initialized before command-line processing. They are not automatically recomputed when a base quantity changes. Read `LATTICE/Parameters.cpp` and the actual initialization calls together before assuming a setting changes all related scales.

Restart flags (`-Loaded`, `-i1`, `-i2`, `-i3`, `-Time`) exist, but checkpoint writes are commented out in the normal selected run. The restart path also has different ending-time and initialization behavior. Treat restarting as a separate feature that needs validation.

## 6. Read the output files

For seed `1`, the selected procedure writes:

| File | Contents |
| --- | --- |
| `InfoID1.txt` | A subset of run settings; retain the command and source snapshot too |
| `EnergyID1.txt` | Initial six-column row, then four-column rows every 20 steps |
| `EMTDynT0ID1.txt` | Initial longitudinal energy-momentum profile |
| `EMTDynT<time>ID1.txt` | Further profiles every 100 steps |
| `GausCheck1.txt` | Time, maximum Gauss-law residual, RMS Gauss-law residual |
| `GVDynT0ID1.txt` | Initial z profile of the transverse average of the squared Gauss-law residual |

Each energy-momentum profile has one row per z site, averaging over x and y. Its 14 columns are:

```text
z T00 TXX TYY TZZ T0X T0Y T0Z T0Xmod T0Ymod T0Zmod TXXmod TYYmod TZZmod
```

`z` is a site index. Start by plotting column 2 (`T00`) against column 1 (`z`). The `mod` columns are alternative definitions implemented in `OBSERVABLES/EnergyMomentumTensor.cpp`; examine those formulas before interpreting them.

Gauss-law residuals are numerical diagnostics. Use their size and evolution to compare runs with controlled settings; this repository does not specify an acceptance threshold. In the current code, the initial console message from `CheckViolation(string)` does not update the global maximum/RMS values it prints. Prefer the values actually computed by `SaveViolation` in `GausCheck`.

For Python 3 users, this example reads the mixed-format energy file using only the standard library:

```python
from pathlib import Path

with Path("timetest008Coll/EnergyID0.txt").open() as stream:
    initial = [float(value) for value in next(stream).split()]
    records = [(initial[0], initial[1])]  # time, T00
    for line in stream:
        if line.strip():
            step, time, energy, source = map(float, line.split())
            records.append((time, energy))

for time, energy in records:
    print(f"time={time:g}, bulk T00={energy:.10g}")
```

## 7. Learn the source in this order

1. `SUNcSolver.cpp`: entry point, selected procedure, startup sequence.
2. `CommandlineArguments.cpp`: supported runtime options.
3. `LATTICE/3DGrid.cpp` and `LATTICE/Parameters.cpp`: defaults and compile-time settings.
4. `RightNucleusSimulation.cpp`: `Init`, `Run`, and `Evolve`; follow the current workflow before comparing other procedures.
5. `COLLIDINGOBJECTS/CollidingObjects.cpp` and `INITIALCONDITIONS/SUNCCURRENTS/RevSingleNucleus.cpp`: how initial fields and currents are built.
6. `DYNAMICS/SUNcDynamics.cpp`: time evolution.
7. `OBSERVABLES/BulkObservables.cpp`, `EnergyMomentumTensor.cpp`, and `GaussLawViolation.cpp`: measurement definitions.

All paths in this list are under `SourceCode`. As a first experiment, keep the grid and all physics settings fixed, change only `-SEED`, and compare the resulting profiles. After that, compare smaller time steps at the same ending time, remembering that the output intervals are specified in steps.

The `.sbat` files and `Script.sh` describe larger cluster jobs, not the local tutorial. They contain a previous user's scheduler account and notification settings. `Script.sh` submits 24 jobs, and `INTELHSWCompile.sh` refers to an `INTELMakefileHSW` file absent from this checkout. Study them after the local workflow is familiar.
