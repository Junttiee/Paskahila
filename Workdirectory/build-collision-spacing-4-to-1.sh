#!/usr/bin/env bash
# Build separately identified 4:1 collision pilot and main executables.
set -euo pipefail

cd -- "$(dirname -- "${BASH_SOURCE[0]}")"
mkdir -p build

compiler="${CXX:-g++}"
common_flags=(-std=gnu++14 -O2 -fopenmp -DCOLLISION_STUDY_BUILD)
libraries=(-lgsl -lgslcblas -lfftw3_omp -lfftw3 -lm)

build_one() {
    local endpoint="$1"
    local label="$2"
    local executable="build/CollSUNcSolver-4to1-${label}"
    local repo_root
    repo_root="$(git -C "$(pwd)/.." rev-parse --show-toplevel)"

    "$compiler" "${common_flags[@]}" "-DCOLLISION_STUDY_END_T=${endpoint}" \
        SourceCode/SUNcSolver.cpp -o "$executable" "${libraries[@]}"

    {
        printf 'plan=collision-spacing-4-to-1\n'
        printf 'role=%s\n' "$label"
        printf 'endpoint=%s\n' "$endpoint"
        printf 'selected_procedure=Simulation.cpp\n'
        printf 'COLLISION_STUDY_BUILD=1\n'
        printf 'Qs0A_Qs0A_L_Qs0A_R=0.125 0.125 0.125\n'
        printf 'source_density_convention=cell-centered transverse density\n'
        printf 'transverse_gaussian_std=Qs0A/sqrt(ax*ay)\n'
        printf 'MEff=0.125\nLambda=0.625\n'
        printf 'compiler=%s\n' "$compiler"
        printf 'compiler_version=%s\n' "$("$compiler" --version | head -n 1)"
        printf 'compile_command='
        printf '%q ' "$compiler" "${common_flags[@]}" \
            "-DCOLLISION_STUDY_END_T=${endpoint}" SourceCode/SUNcSolver.cpp \
            -o "$executable" "${libraries[@]}"
        printf '\nsource_revision='
        git -C "$repo_root" rev-parse HEAD
        printf 'source_hashes:\n'
        sha256sum SourceCode/SUNcSolver.cpp SourceCode/LATTICE/Parameters.cpp SourceCode/Simulation.cpp SourceCode/COLLIDINGOBJECTS/CollidingObjects.cpp
        printf 'executable_hash:\n'
        sha256sum "$executable"
    } > "${executable}.build-info.txt"

    printf 'Built %s\n' "$executable"
}

build_one 40.0 pilot-t40
build_one 88.0 main-t88
