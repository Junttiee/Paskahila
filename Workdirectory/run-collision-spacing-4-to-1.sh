#!/usr/bin/env bash
# Run one planned 4:1 collision grid into a fresh directory with a full manifest.
set -euo pipefail

cd -- "$(dirname -- "${BASH_SOURCE[0]}")"

usage() {
    printf 'Usage: %s {A|B} {pilot|main}\n' "$0" >&2
    printf 'Optional: set COLLISION_DT=0.008 for the later Grid B main-run extension.\n' >&2
}

if [[ $# -ne 2 ]]; then
    usage
    exit 2
fi

grid="$1"
stage="$2"
dt="${COLLISION_DT:-0.08}"
seed="${COLLISION_SEED:-1}"
threads="${OMP_NUM_THREADS:-8}"
export OMP_NUM_THREADS="$threads"
export OMP_DYNAMIC="${OMP_DYNAMIC:-FALSE}"

if [[ ! "$threads" =~ ^[1-9][0-9]*$ ]]; then
    printf 'OMP_NUM_THREADS must be a positive integer.\n' >&2
    exit 2
fi
if [[ ! "$seed" =~ ^[1-9][0-9]*$ ]]; then
    printf 'COLLISION_SEED must be a positive integer.\n' >&2
    exit 2
fi

case "$grid" in
    A)
        grid_label="Grid A"
        nx=32; ny=32; nz=256
        ax=4; ay=4; az=1
        lx=128; ly=128; lz=256
        width=4
        center_l=107; center_r=146
        z_l=107; z_r=146; z_c=126.5; t_coll=19.5
        ;;
    B)
        grid_label="Grid B"
        nx=64; ny=64; nz=512
        ax=2; ay=2; az=0.5
        lx=128; ly=128; lz=256
        width=8
        center_l=216; center_r=293
        z_l=108; z_r=146.5; z_c=127.25; t_coll=19.25
        ;;
    *)
        usage
        printf 'Grid must be A or B.\n' >&2
        exit 2
        ;;
esac

case "$stage" in
    pilot)
        if [[ "$grid" != "A" ]]; then
            printf 'The planned t=40 pilot uses Grid A.\n' >&2
            exit 2
        fi
        if [[ "$dt" != "0.08" ]]; then
            printf 'The planned pilot uses dT=0.08.\n' >&2
            exit 2
        fi
        endpoint=40
        role="pilot-t40"
        executable="build/CollSUNcSolver-4to1-pilot-t40"
        ;;
    main)
        endpoint=88
        role="main-t88"
        executable="build/CollSUNcSolver-4to1-main-t88"
        if [[ "$dt" != "0.08" && ! ( "$grid" == "B" && "$dt" == "0.008" ) ]]; then
            printf 'Main runs use dT=0.08; dT=0.008 is reserved for the later Grid B extension.\n' >&2
            exit 2
        fi
        ;;
    *)
        usage
        printf 'Stage must be pilot or main.\n' >&2
        exit 2
        ;;
esac

if [[ ! -x "$executable" ]]; then
    printf 'Executable not found: %s\nRun bash build-collision-spacing-4-to-1.sh first.\n' "$executable" >&2
    exit 1
fi
if [[ ! -r "${executable}.build-info.txt" ]]; then
    printf 'Build record not found: %s\nRebuild with bash build-collision-spacing-4-to-1.sh.\n' "${executable}.build-info.txt" >&2
    exit 1
fi

mkdir -p runs
run_dir="$(mktemp -d "runs/collision-spacing-4-to-1-${grid}-${role}-XXXXXX")"
repo_root="$(git -C "$(pwd)/.." rev-parse --show-toplevel)"
nominal_steps="$(awk -v end="$endpoint" -v step="$dt" 'BEGIN {printf "%.0f", end / step}')"
start_time="$(date -Is)"
start_epoch="$(date +%s)"
run_args=(
    -Nx "$nx" -Ny "$ny" -Nz "$nz"
    -ax "$ax" -ay "$ay" -az "$az"
    -dT "$dt"
    -RlOverAzL "$width" -RlOverAzR "$width"
    -QsA 0.125 -MEff 0.125 -Lambda 0.625
    -NucleusL 1 -NucleusR 1 -SEED "$seed" -o "$run_dir"
)

manifest="$run_dir/MANIFEST.txt"
{
    printf 'plan=collision-spacing-4-to-1\n'
    printf 'procedure=Simulation.cpp\n'
    printf 'model=SU(2)\n'
    printf 'stage=%s\n' "$stage"
    printf 'grid=%s\n' "$grid_label"
    printf 'started=%s\n' "$start_time"
    printf 'source_revision='
    git -C "$repo_root" rev-parse HEAD
    printf 'source_hashes:\n'
    sha256sum SourceCode/SUNcSolver.cpp SourceCode/LATTICE/Parameters.cpp SourceCode/Simulation.cpp SourceCode/COLLIDINGOBJECTS/CollidingObjects.cpp
    printf 'executable=%s\n' "$executable"
    printf 'executable_hash='
    sha256sum "$executable"
    printf '\nbuild_record:\n'
    cat "${executable}.build-info.txt"
    printf '\nphysics_and_grid:\n'
    printf 'Qs=0.125 (simulation units)\n'
    printf 'source_density_convention=cell-centered transverse density\n'
    printf 'transverse_gaussian_std=Qs0A/sqrt(ax*ay)\n'
    printf 'variance_check=(ax*ay)*Var(rho_i/(NucleusMultiplier*T(z)))=Qs^2 per color component\n'
    printf 'Qs_sigma_z=0.5\n'
    printf 'sigma_z=4\n'
    printf 'm=0.125\nLambda=0.625\n'
    printf 'source_amplitudes_recorded_Qs0A_Qs0A_L_Qs0A_R=0.125 0.125 0.125\n'
    printf 'actual_initializer_amplitudes_left_right=0.125 0.125 (both calls use Qs0A_L)\n'
    printf 'Nx_Ny_Nz=%s %s %s\n' "$nx" "$ny" "$nz"
    printf 'ax_ay_az=%s %s %s\n' "$ax" "$ay" "$az"
    printf 'box_lengths=%s %s %s\n' "$lx" "$ly" "$lz"
    printf 'sigma_z_over_az_left_right=%s %s\n' "$width" "$width"
    printf 'charge_density_multipliers_left_right=1 1\n'
    printf 'boundary_conditions=existing periodic x/y and open z\n'
    printf 'center_rule=int(0.85*Nz/2-1), int(1.15*Nz/2-1)\n'
    printf 'center_indices_left_right=%s %s\n' "$center_l" "$center_r"
    printf 'physical_centers_z_left_right=%s %s\n' "$z_l" "$z_r"
    printf 'aligned_center_z_c=%s\n' "$z_c"
    printf 'nominal_collision_time=%s\n' "$t_coll"
    printf 'Q_s_times_endpoint_minus_t_coll=%s\n' "$(awk -v end="$endpoint" -v tc="$t_coll" 'BEGIN {printf "%.8g", 0.125 * (end - tc)}')"
    printf 'seed=%s\ndT=%s\nendpoint_from_initialization=%s\n' "$seed" "$dt" "$endpoint"
    printf 'nominal_steps=%s\n' "$nominal_steps"
    printf 'threads=%s\nOMP_DYNAMIC=%s\n' "$OMP_NUM_THREADS" "$OMP_DYNAMIC"
    printf 'profile_cadence=every 100 steps\n'
    printf 'output_directory=%s\n' "$run_dir"
    printf 'resource_usage_file=%s/resource-usage.txt\n' "$run_dir"
    printf 'command='
    printf '%q ' "$executable" "${run_args[@]}"
    printf '\nrun_status=running\n'
} > "$manifest"

printf 'Writing %s (%s threads)\n' "$run_dir" "$OMP_NUM_THREADS"
set +e
if [[ -x /usr/bin/time ]]; then
    LC_ALL=C /usr/bin/time -v -o "$run_dir/resource-usage.txt" \
        "$executable" "${run_args[@]}" > "$run_dir/run.log" 2>&1
    exit_status=$?
else
    "$executable" "${run_args[@]}" > "$run_dir/run.log" 2>&1
    exit_status=$?
    printf 'GNU time is unavailable; peak resident memory was not captured.\n' > "$run_dir/resource-usage.txt"
fi
set -e
finish_time="$(date -Is)"
elapsed_seconds="$(( $(date +%s) - start_epoch ))"
peak_rss_kb="$(awk -F': ' '/Maximum resident set size/ {print $2}' "$run_dir/resource-usage.txt" 2>/dev/null || true)"
if [[ -z "$peak_rss_kb" ]]; then
    peak_rss_kb="not-recorded"
fi
end_record="$(grep '^#EVOLUTION_END_STEPS=' "$run_dir/run.log" | tail -n 1 || true)"
completion_marker="no"
if grep -Fq "#COMPLETED ID=${seed}" "$run_dir/run.log"; then
    completion_marker="yes"
fi
actual_steps="not-recorded"
actual_endpoint="not-recorded"
if [[ -n "$end_record" ]]; then
    actual_steps="$(sed -E 's/^#EVOLUTION_END_STEPS=([0-9]+).*/\1/' <<< "$end_record")"
    actual_endpoint="$(sed -E 's/.* TIME=//' <<< "$end_record")"
fi
run_status="failed"
if [[ "$exit_status" -eq 0 && "$completion_marker" == "yes" && -n "$end_record" ]]; then
    run_status="completed"
fi
{
    printf '\nfinished=%s\n' "$finish_time"
    printf 'elapsed_seconds=%s\n' "$elapsed_seconds"
    printf 'peak_rss_kb=%s\n' "$peak_rss_kb"
    printf 'actual_steps=%s\n' "$actual_steps"
    printf 'actual_endpoint=%s\n' "$actual_endpoint"
    printf 'completion_marker=%s\n' "$completion_marker"
    printf 'run_status=%s\n' "$run_status"
    printf 'exit_status=%s\n' "$exit_status"
} >> "$manifest"

printf 'Finished %s with exit status %s.\n' "$run_dir" "$exit_status"
exit "$exit_status"
