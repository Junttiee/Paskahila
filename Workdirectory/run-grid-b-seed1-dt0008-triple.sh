#!/usr/bin/env bash
set -euo pipefail

cd -- "$(dirname -- "${BASH_SOURCE[0]}")"

seed=1
dt=0.008
threads=8
runner_source="run-collision-spacing-4-to-1.sh"
build="build/CollSUNcSolver-4to1-main-t88"
build_record="${build}.build-info.txt"

if [[ ! -x "$build" || ! -r "$build_record" ]]; then
    printf 'Grid B main executable or build record is missing.\n' >&2
    exit 1
fi
if ! grep -Fq 'selected_procedure=Simulation.cpp' "$build_record" \
    || ! grep -Fq 'source_density_convention=cell-centered transverse density' "$build_record"; then
    printf 'Build record does not confirm the planned collision procedure and corrected source convention.\n' >&2
    exit 1
fi

for source in \
    SourceCode/SUNcSolver.cpp \
    SourceCode/LATTICE/Parameters.cpp \
    SourceCode/Simulation.cpp \
    SourceCode/COLLIDINGOBJECTS/CollidingObjects.cpp; do
    expected="$(awk -v path="$source" '$2 == path {print $1}' "$build_record")"
    actual="$(sha256sum "$source" | awk '{print $1}')"
    if [[ -z "$expected" || "$actual" != "$expected" ]]; then
        printf 'Source hash mismatch for %s; rebuild before running.\n' "$source" >&2
        exit 1
    fi
done

expected_binary="$(awk -v path="$build" '$2 == path {print $1}' "$build_record")"
actual_binary="$(sha256sum "$build" | awk '{print $1}')"
if [[ -z "$expected_binary" || "$actual_binary" != "$expected_binary" ]]; then
    printf 'Executable hash does not match the build record; rebuild before running.\n' >&2
    exit 1
fi

sequence_dir="$(mktemp -d 'runs/grid-b-seed1-dt0008-triple-XXXXXX')"
sequence_manifest="$sequence_dir/SEQUENCE.txt"
sequence_log="$sequence_dir/sequence.log"
latest_pointer="runs/grid-b-seed1-dt0008-triple-LATEST.txt"
runner_script=""
sequence_status=running
sequence_exit_status=0
sequence_started="$(date -Is)"

printf '%s\n' "$sequence_dir" > "$latest_pointer"
{
    printf 'sequence=grid-b-seed1-dT-0.008-matched-triple\n'
    printf 'run_order=collision,left-only,right-only\n'
    printf 'seed=%s\ndT=%s\nthreads=%s\n' "$seed" "$dt" "$threads"
    printf 'source_revision=%s\n' "$(git rev-parse HEAD)"
    printf 'started=%s\n' "$sequence_started"
    printf 'status=running\n'
} > "$sequence_manifest"

finish_sequence() {
    local shell_status=$?
    if [[ "$sequence_status" == running ]]; then
        sequence_status=failed
        if [[ "$sequence_exit_status" -eq 0 ]]; then
            sequence_exit_status="$shell_status"
        fi
    fi
    {
        printf '\nfinished=%s\n' "$(date -Is)"
        printf 'sequence_status=%s\n' "$sequence_status"
        printf 'exit_status=%s\n' "$sequence_exit_status"
    } >> "$sequence_manifest"
    if [[ -n "$runner_script" ]]; then
        rm -f -- "$runner_script"
    fi
}
trap finish_sequence EXIT

runner_script="$(mktemp '.run-collision-spacing-4-to-1.XXXXXX.sh')"
sed 's/\r$//' "$runner_source" > "$runner_script"

printf 'SEQUENCE_DIR=%s\nStarting seed %s, dT=%s, at %s\n' \
    "$sequence_dir" "$seed" "$dt" "$sequence_started" | tee -a "$sequence_log"

for run_type in collision left-only right-only; do
    run_started="$(date -Is)"
    printf '\nStarting %s at %s\n' "$run_type" "$run_started" | tee -a "$sequence_log"

    set +e
    runner_output="$(COLLISION_SEED="$seed" COLLISION_DT="$dt" OMP_NUM_THREADS="$threads" \
        bash "$runner_script" B main "$run_type" 2>&1)"
    run_exit_status=$?
    set -e

    if [[ -n "$runner_output" ]]; then
        printf '%s\n' "$runner_output" | tee -a "$sequence_log"
    fi
    run_dir="$(printf '%s\n' "$runner_output" | sed -n 's/^Writing \(.*\) (8 threads)$/\1/p' | tail -n 1)"
    run_finished="$(date -Is)"
    {
        printf '\nrun_type=%s\n' "$run_type"
        printf 'run_started=%s\nrun_finished=%s\n' "$run_started" "$run_finished"
        printf 'output_directory=%s\n' "${run_dir:-not-recorded}"
        printf 'exit_status=%s\n' "$run_exit_status"
    } >> "$sequence_manifest"

    if [[ "$run_exit_status" -ne 0 || -z "$run_dir" || ! -r "$run_dir/MANIFEST.txt" ]]; then
        sequence_status=failed
        sequence_exit_status="$run_exit_status"
        if [[ "$sequence_exit_status" -eq 0 ]]; then
            sequence_exit_status=1
        fi
        printf 'Stopping sequence after %s; inspect its run log and manifest.\n' "$run_type" \
            | tee -a "$sequence_log" >&2
        exit "$sequence_exit_status"
    fi
    if ! grep -Fq 'run_status=completed' "$run_dir/MANIFEST.txt" \
        || ! grep -Fq 'emt_profile_cadence_steps=100' "$run_dir/MANIFEST.txt"; then
        sequence_status=failed
        sequence_exit_status=1
        printf 'Stopping sequence: %s did not pass completion or cadence checks.\n' "$run_type" \
            | tee -a "$sequence_log" >&2
        exit 1
    fi
done

sequence_status=completed
sequence_exit_status=0
printf '\nAll three seed-1 dT=0.008 runs completed.\n' | tee -a "$sequence_log"
