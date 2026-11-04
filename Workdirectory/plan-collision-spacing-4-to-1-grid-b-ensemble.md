# Grid B timestep and collision-induced EMT ensemble plan

4 November 2026

Status: Drafted; no ensemble runs have started. This follow-up studies the corrected 4:1 Grid B at two time steps and uses matched single-nucleus controls to define the collision-induced energy-momentum tensor. The completed one-seed, two-grid checkpoint is summarized in [COLLISION_4_TO_1_HANDOFF.md](COLLISION_4_TO_1_HANDOFF.md). Shared implementation history and the source-normalization check are in [PLANS.md](PLANS.md).

## Questions and estimands

1. How much do the longitudinal EMT profiles at `dT=0.08` and `dT=0.008` differ on the same Grid B source realization?
2. What is the operational collision-induced profile, defined componentwise as
   `T_induced = T_collision - T_left-only - T_right-only`?
3. How much do these paired quantities vary across eight independent legacy-generator seeds?

The subtraction defines the requested comparison; because the field evolution is nonlinear, it is not assumed to be a unique decomposition of the full interacting field. Report it as a collision-minus-isolated-controls observable.

## Fixed configuration

| Setting | Value |
|---|---:|
| Gauge group | SU(2) |
| Grid | `64 x 64 x 512` |
| Spacings `(a_x,a_y,a_z)` | `(2,2,0.5)` |
| Physical box `(L_x,L_y,L_z)` | `(128,128,256)` |
| Source strength for each active nucleus | `Q_s=0.125` simulation units |
| Width inputs `sigma_z/a_z` | `8` for both nuclei (`sigma_z=4`) |
| Infrared and ultraviolet regulators | `m=0.125`, `Lambda=0.625` |
| End time | `t_end=88` from initialization |
| Time steps | `dT=0.08` and `dT=0.008` |
| Seeds | `1,2,3,4,5,6,7,8` |
| Threads | Eight, fixed for the campaign |
| EMT profile cadence | `0.8` simulation-time units |

For the 4:1 implementation keep the accepted center formulas and boundary treatment. On Grid B the center indices are `(216,293)`, the nominal physical centers are `(108,146.5)`, the aligned midpoint is `z_c=127.25`, and the nominal crossing time is `t_coll=19.25`. Keep raw `t,z` coordinates and also record `s=t-t_coll` and `zeta=z-z_c` for aligned analysis.

The corrected source convention is cell-centered transverse density: the Gaussian amplitude scales as `1/sqrt(a_x*a_y)`, so `(a_x*a_y)*Var(rho_i)` equals `(NucleusMultiplier*Q_s*T(z))^2` before smoothing. Confirm the compiled source and manifest use this convention before launching. The current tracked corrected-code results in the handoff used the inverse-area normalization; do not mix the earlier pre-correction batch into this ensemble.

## Run matrix and matching

Run all combinations below for every seed:

| Time step | Collision | Left-only | Right-only | Runs across 8 seeds |
|---|---:|---:|---:|---:|
| `0.08` | 8 | 8 | 8 | 24 |
| `0.008` | 8 | 8 | 8 | 24 |
| **Total** | **16** | **16** | **16** | **48** |

For each `(seed,dT)` triple, use exactly the same seed and grid in all three run types. Set density multipliers to `(1,1)` for collision, `(1,0)` for left-only, and `(0,1)` for right-only. The existing initializer draws both transverse source arrays in the same order even when one multiplier is zero, so each active nucleus retains the corresponding realization from the collision run. The initialization is independent of `dT`, so reusing a seed also pairs the source realization across the two time steps. Preserve this code path and verify the multipliers in each manifest.

The existing launcher supports these modes on Grid B main runs:

```bash
COLLISION_SEED=1 COLLISION_DT=0.08 bash run-collision-spacing-4-to-1.sh B main collision
COLLISION_SEED=1 COLLISION_DT=0.08 bash run-collision-spacing-4-to-1.sh B main left-only
COLLISION_SEED=1 COLLISION_DT=0.08 bash run-collision-spacing-4-to-1.sh B main right-only
```

Repeat for seeds 1 through 8, then repeat the same triples with `COLLISION_DT=0.008`. Run sequentially at eight threads to stay within the measured memory footprint. Keep every run in its own fresh directory and retain its command, revision and source hashes, seed, multiplier pair, actual endpoint/step count, elapsed time and peak RSS in `MANIFEST.txt`.

## Output schedule and time alignment

Save the EMT profile every `0.8` simulation-time units: every 10 steps at `dT=0.08`, and every 100 steps at `dT=0.008`. The initial profile at `t=0` is retained. Both resolutions should therefore provide the same nominal profile times from `t=0` through `t=88`, 111 profiles per run or 5,328 profiles in the campaign. The existing 20-step bulk-energy and Gauss-law outputs keep their step-based schedule; they are diagnostics, not the EMT analysis series.

The evolution loop can take one extra final step because of floating-point accumulation. Record the actual marker from each log; the primary profile comparison uses the common saved frames through nominal `t=88`, not an assumption about the final loop time. Before computing time-step differences, use the measurement-time staggering described in [PLANS.md](PLANS.md). Match physical times using that mapping, treat `t=0` separately and interpolate only from bracketing frames at the common 0.8-unit cadence. Retain raw reported timestamps alongside any corrected physical-time estimate.

One Grid B `dT=0.08` corrected run took 561 seconds with the previous 100-step profile cadence and used about 2.1 GiB peak RSS. A step-count-only projection gives about 41 hours for 48 runs at the same eight-thread setting: 24 coarse runs at roughly 9m21s each and 24 fine runs at roughly ten times that duration. This is a baseline estimate; increasing coarse-run profile writes from 12 to 111 per run adds output work that has not yet been benchmarked. Use the first seed-1 coarse collision run to measure the new cadence overhead before treating the total as a firm schedule. At 145,595 bytes for the existing Grid B initial profile, EMT text output alone is on the order of 0.75 GB for this matrix.

## Analysis

For each seed and time step, calculate `T_induced` componentwise for `T00`, `T0Z`, `P_T=(TXX+TYY)/2`, and `P_L=TZZ`. Convert the longitudinal index to physical `z`, and produce both raw and center-aligned profiles. Compare the two time steps within each run type and for `T_induced` at matched physical times. Keep the full EMT tensor columns in archived raw files.

Across seeds, report the ensemble mean and standard deviation or standard error for each paired timestep difference and for `T_induced`. Use seed-paired statistics: form each seed's control subtraction and fine-minus-coarse difference before aggregating. Show individual seed results as well as the aggregate so a small mean does not hide seed variation. Do not present the eight seeds as eight independent samples for each of the three terms in a control subtraction; each seed contributes one matched triple.

Review the initial profiles and Gauss-law residuals for each run, check all output values are finite, and verify profiles occur at the requested common physical times. Report measured runtime and memory by time step and run type. The existing corrected one-seed comparison is a checkpoint, not an ensemble result; this plan does not by itself establish spatial convergence or a universal physical prediction.

## Execution gates and deliverables

1. Rebuild the Grid B collision-study executable from the confirmed corrected source, then inspect its build record and source hashes.
2. Run the seed-1, `dT=0.08` collision case first. Confirm a 0.8-unit profile cadence, expected multiplier pair, finite profiles and acceptable initial diagnostics; record its actual runtime before scheduling the remainder.
3. Complete and review each matched three-run group before proceeding to the next seed/time step. Resume only from manifest-confirmed completed runs; never reuse a run directory.
4. Deliver a 48-run manifest index, profile-time audit, paired raw and aligned CSVs, ensemble plots and uncertainty summaries, plus an updated handoff document.

No run should be launched until the source-normalization and output-cadence gates are satisfied. This document is a plan only; it does not authorize starting the ensemble.
