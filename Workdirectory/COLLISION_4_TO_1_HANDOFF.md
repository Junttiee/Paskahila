# Collision 4:1 checkpoint handoff

Updated 5 November 2026. This note records the corrected SU(2) 4:1 collision checkpoint. The more recent Grid B seed-1 timestep and matched-control work has its own [handoff](GRID_B_SEED1_HANDOFF.md).

## Current state

- The source array is treated as a cell-centered transverse charge density. In `SourceCode/COLLIDINGOBJECTS/CollidingObjects.cpp`, the Gaussian amplitude scales as `Qs0A/sqrt(a_x*a_y)`. The Poisson solve uses physical lattice momenta and consumes density directly.
- A GSL check of the raw transverse draws, before the longitudinal profile and smoothing, measured `(a_x*a_y)*Var(rho_i)` as 0.01581645 on Grid A and 0.01583379 on Grid B. The target is `Qs^2 = 0.015625` for `Qs=0.125`.
- The corrected Grid A pilot and the corrected Grid A and Grid B main runs all completed with seed 1, eight threads, and `dT=0.08`. They finished at 40.08 and 88.08 simulation-time units, respectively. All output was finite and the Gauss-law residuals remained small.
- Those checkpoint runs used the old profile schedule of 100 steps (8 simulation-time units at `dT=0.08`). New Grid B ensemble runs use EMT profiles every 0.8 simulation-time units: 10 steps at `dT=0.08` and 100 at `dT=0.008`. The bulk-energy and Gauss-law records retain their existing 20-step schedule.
- The initial, pre-correction runs are preserved as historical baseline. Do not combine them with the corrected runs.

## Corrected results

The bulk T00 output is a site sum. Multiplying it by cell volume gives integrated energies of 10.5014 (A) and 9.45867 (B) at `t=0`, so `B/A=0.9007`. At `t=88`, the ratio is 0.8863. The pre-correction initial ratio was 17.665.

Aligned-profile RMS differences relative to Grid A at reported `t=24/40/88` were:

| Field | Relative RMS difference |
|---|---:|
| T00 | 10.9% / 11.6% / 14.2% |
| T0Z | 28.7% / 28.2% / 29.1% |
| Transverse pressure | 6.4% / 17.9% / 9.4% |
| Longitudinal pressure | 10.9% / 11.6% / 14.2% |

This is a one-seed sensitivity comparison, not a convergence result: the legacy generator's same seed does not match physical realizations across the different transverse meshes. Profiles are compared at equal reported times; impact ages differ by 0.25 units between grids, and the eight-unit output cadence is too coarse for precise near-impact time alignment. Small Gauss-law residuals demonstrate constraint satisfaction, not physical accuracy.

## Reproducing the runs and analysis

From `Workdirectory` in Ubuntu/WSL, build both executables and run the corrected sequence with:

```bash
bash build-collision-spacing-4-to-1.sh
OMP_NUM_THREADS=8 bash run-collision-spacing-4-to-1.sh A pilot
OMP_NUM_THREADS=8 bash run-collision-spacing-4-to-1.sh A main
OMP_NUM_THREADS=8 bash run-collision-spacing-4-to-1.sh B main
```

The completed run directories in the current workspace are:

- `runs/collision-spacing-4-to-1-A-pilot-t40-aTVfkb/`
- `runs/collision-spacing-4-to-1-A-main-t88-6gwTqr/`
- `runs/collision-spacing-4-to-1-B-main-t88-asxl1e/`

Recreate the aligned comparison with:

```bash
python3 analyze-collision-spacing-4-to-1.py \
  --grid-a runs/collision-spacing-4-to-1-A-main-t88-6gwTqr \
  --grid-b runs/collision-spacing-4-to-1-B-main-t88-asxl1e \
  --output runs/collision-spacing-4-to-1-corrected-analysis
```

`runs/` is ignored by Git. The manifests, raw data, CSV and SVG plots are available in the current workspace but are not included in the PR; this tracked handoff and `PLANS.md` preserve the key results and commands.

## Completed Grid B ensemble

The Grid B timestep and induced-EMT ensemble completed all 48 runs across eight matched seeds, two time steps, and collision/left-only/right-only triples. The [ensemble handoff](GRID_B_ENSEMBLE_HANDOFF.md) records the run audit, paired timestep differences, control subtraction, and seed uncertainty.

The detailed run record and shared study decisions are in [PLANS.md](PLANS.md) and [plan-collision-spacing-4-to-1.md](plan-collision-spacing-4-to-1.md).
