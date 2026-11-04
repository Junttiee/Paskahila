#!/usr/bin/env bash
# Small local test. Compile the executable separately before using this script.
set -euo pipefail

cd -- "$(dirname -- "${BASH_SOURCE[0]}")"
solver="${1:-./build/RightSUNcSolver-small}"

if [[ ! -x "$solver" ]]; then
    printf 'Executable not found or not executable: %s\nCompile it first, or pass its path to this script.\n' "$solver" >&2
    exit 1
fi

export OMP_NUM_THREADS="${OMP_NUM_THREADS:-1}"
export OMP_DYNAMIC="${OMP_DYNAMIC:-FALSE}"
if [[ ! "$OMP_NUM_THREADS" =~ ^[1-9][0-9]*$ ]]; then
    printf 'OMP_NUM_THREADS must be a positive integer.\n' >&2
    exit 1
fi

mkdir -p runs
run_dir=$(mktemp -d runs/first-right-XXXXXX)
printf 'Writing results to %s\n' "$run_dir"

exec "$solver" \
    -Nx 16 -Ny 16 -Nz 256 \
    -ax 1.0 -ay 1.0 -az 0.25 \
    -dT 0.008 \
    -RlOverAzL 4 -RlOverAzR 4 \
    -SEED 1 -o "$run_dir" \
    > "$run_dir/run.log" 2>&1
