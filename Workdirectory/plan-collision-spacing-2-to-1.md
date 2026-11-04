# Collision study plan with two to one spacing

4 November 2026

Status: Ready for implementation and pilot validation. This is the active local collision series. Read [PLANS.md](PLANS.md) for shared decisions, code status and the first implementation steps. The spacing ratio is a_perp:a_z = 2:1. The separate [4:1 series](plan-collision-spacing-4-to-1.md) has different grids and an optional dT = 0.008 check; do not combine their run matrices.

## Aim and scope

Study a symmetric collision of two longitudinally extended nuclei in the SU(2) classical Yang–Mills solver using the three spatial grids discussed for single-nucleus propagation. Compare spatial sensitivity at dT = 0.08 and add a finest-grid check at dT = 0.04. Prioritize minimal changes to the established code and compatibility with historical data. Use one seed initially for workflow checks; quantitative cross-grid comparisons must account for stochastic variation. This is a numerical demonstration with simplified source profiles, not a calibrated nuclear-collision prediction.

Retain the existing center formulas for the first demonstration. This measures the spatial sensitivity of the complete numerical setup, including rounding of initial positions. It does not isolate evolution-stencil error at exactly fixed physical centers. The main-run endpoint is t_end = 88, giving Q_s t_end = 11 from initialization and about 8.6 units of Q_s times the elapsed time after nominal center crossing. Start with a short coarse collision pilot to t = 40, using seed 1 and eight CPU threads. No separate single-nucleus campaign is required first.

## Physics and initial geometry

| Parameter | Working value |
|---|---:|
| Box lengths (L_x, L_y, L_z) | (128, 128, 256) |
| Source strength of each nucleus Q_s | 0.125 |
| Thickness of each nucleus Q_s σ_z | 0.5 |
| Gaussian standard deviation of each nucleus σ_z | 4 |
| Infrared regulator m | 0.125 |
| Ultraviolet regulator Λ | 0.625 |
| Charge-density multipliers for the collision | Left = 1, right = 1 |
| Left-positioned nucleus center index and motion | i_L = int(0.85 N_z/2 − 1), moving toward +z |
| Right-positioned nucleus center index and motion | i_R = int(1.15 N_z/2 − 1), moving toward −z |
| Physical centers | z_L = a_z i_L; z_R = a_z i_R |
| Nominal crossing point and time | z_c = (z_L + z_R)/2; t_coll = (z_R − z_L)/2 |
| Boundary conditions | Periodic in x and y; current open treatment in z |

All quantities use simulation units. Q_s denotes the source-strength convention of the existing model. The two nuclei have independent transverse fluctuations. At each fixed transverse grid, reuse each realization across time steps and its corresponding single-nucleus control. The legacy generator with the same seed does not match physical fluctuations across different transverse grids; use event statistics to assess that contribution.

The integer conversion truncates the positive fractional indices, matching the current code. The centers are 38–39 physical units apart, or 9.5–9.75 Gaussian widths. At the crossing midpoint, each Gaussian is about 0.0007–0.0013% of its own peak initially. This supports retaining the old placement for a demonstration; inspect initial overlap and the half-domain initialization seam in the pilot.

The physical midpoint and nominal crossing time differ between grids, as tabulated below. In the continuum limit the formulas approach z_L = 108.8, z_R = 147.2, z_c = 128 and t_coll = 19.2. At finite spacing, retain and report the actual positions. Define s = t − t_coll and ζ = z − z_c separately for each grid only when presenting aligned profiles. Keep the raw t and z comparisons as the primary record: alignment diagnoses translation and timing effects but does not remove differences in pre-collision propagation or boundary placement. Any further dynamically measured crossing-time shift is also part of the numerical result.

## Spatial grids and time steps

| Parameter | Coarse | Medium | Fine | Fine time-step check |
|---|---:|---:|---:|---:|
| (N_x, N_y, N_z) | (64, 64, 256) | (96, 96, 384) | (128, 128, 512) | (128, 128, 512) |
| a_x = a_y | 2 | 4/3 | 1 | 1 |
| a_z | 1 | 2/3 | 0.5 | 0.5 |
| Both width inputs σ_z/a_z | 4 | 6 | 8 | 8 |
| Left center index | 107 | 162 | 216 | 216 |
| Right center index | 146 | 219 | 293 | 293 |
| Physical z_L | 107 | 108 | 108 | 108 |
| Physical z_R | 146 | 146 | 146.5 | 146.5 |
| Nominal crossing midpoint z_c | 126.5 | 127 | 127.25 | 127.25 |
| Nominal crossing time t_coll | 19.5 | 19 | 19.25 | 19.25 |
| Q_s a_perp | 0.25 | 1/6 | 0.125 | 0.125 |
| dT | 0.08 | 0.08 | 0.08 | 0.04 |
| dT/a_z | 0.08 | 0.12 | 0.16 | 0.08 |
| Target steps to t_end = 88 | 1,100 | 1,100 | 1,100 | 2,200 |
| Estimated RAM per solver process | About 1.2 GiB | About 4 GiB | About 9 GiB | About 9 GiB |

Each grid preserves the box lengths and a_z/a_perp = 1/2, while keeping the exact legacy center rule. Use precise decimal inputs for 4/3 and 2/3. The finest spatial grid is a comparison reference, not an established continuum solution.

## Endpoint

| Choice | Simulation endpoint | Q_s s_end on coarse / medium / fine | Steps at dT = 0.08 / 0.04 |
|---|---:|---:|---:|
| All full runs | t_end = 88 | 8.5625 / 8.625 / 8.59375 | 1,100 / 2,200 |

At the main-run endpoint, Q_s t_end = 11 from initialization. The final aligned comparison Q_s s = 8 occurs at t = 83.5, 83 and 83.25 on the coarse, medium and fine grids, respectively, within every run. Use the documented measurement-time mapping and interpolation for comparisons at exactly equal s. Expected nucleus centers at t = 88 are (195, 58), (196, 58) and (196, 58.5), still well inside the box. Check the evolved profiles and boundary diagnostics rather than relying only on these nominal trajectories.

## Implementation preparation

1. Select the existing two-nucleus procedure for collision runs, and the existing left-only and right-only procedures for controls. Build identifiable executables from the same source revision. The collision initializer already constructs fields and currents for both incoming nuclei.
2. Preserve the current center formulas and log the resulting integer indices, physical centers, midpoint and nominal crossing time for each run. Use identical geometry in the collision and its matched controls. Set both widths from the table. Ensure the actual source amplitudes equal 0.125: the current right-nucleus initialization call also receives `Qs0A_L`, while the command-line `-QsA` option changes another variable. Set and record m and Λ explicitly.
3. Preserve the existing random-number generator, normalization and draw order as the compatibility baseline. Use identical seeds and generation order for collision/control and time-step comparisons at each fixed transverse grid. Do not interpret a shared seed as a matched physical realization when N_x or N_y changes. Use existing or additional event ensembles to quantify stochastic variation. A matched Fourier-mode generator is a possible later extension, not required in the first implementation.
4. Set the endpoint explicitly and record actual step counts and times. Preserve the stopping loop initially; floating-point time accumulation can add a final step, so use common saved frames for analysis. Initialize staggered fields consistently with each dT. Check initial Gauss-law residuals and verify that the nuclei propagate correctly before appreciable overlap.

## Run matrix and measurements

First run the short coarse collision pilot to t = 40. Once its checks pass, the main collision matrix contains three spatial levels at dT = 0.08 and the finest level at dT = 0.04, all to t = 88. Add left-only and right-only controls for each configuration when extracting the collision-induced tensor. Keep the source pair, positions, physical inputs and output times matched within each configuration. This gives four full-collision runs, or 12 runs including all controls. Reuse matched controls; do not first run a duplicate standalone single-nucleus campaign.

To measure the collision-induced contribution, subtract the matched control tensors component by component:

**T_induced(t,z) = T_collision(t,z) − T_left(t,z) − T_right(t,z).**

Inspect transverse-averaged T00, T0z, transverse pressure (TXX + TYY)/2 and longitudinal pressure TZZ. Check that the subtracted signal is small before appreciable overlap. Convert profile indices to physical z. Normalize the bulk site sum as either mean density T00_bulk/(N_x N_y N_z) or integrated energy (a_x a_y a_z) T00_bulk with the current scale factor of one.

For the first pilot, keep the existing profile interval of 100 steps. Main runs at dT = 0.08 then have shared frames t = 0, 8, …, 88. The dT = 0.04 check has additional frames every 4 units; select every second check frame for equal-time comparisons. Existing energy and Gauss-law output every 20 steps can likewise be matched by taking every second check row.

Before precision comparisons at aligned Q_s s = 0, 5 and 8, establish the measurement-time mapping described in [PLANS.md](PLANS.md). Then add a small output-only change to save steps closely bracketing the desired physical times t_coll, t_coll + 40 and t_coll + 64 in each run and its controls. For temporal checks, account for the differing half-step offsets and verify interpolation sensitivity; doubling counters alone need not match physical measurement times. This retains evolution and output columns. Record the cadence change explicitly. Eight-unit sampling alone is insufficient for precise alignment near impact; the initial pilot need not claim such precision.

## Checks and analysis

Use seed 1 and initially eight CPU threads for new pilots, unless a documented historical comparison requires a different seed. The first scientific pilot is the coarse collision to t = 40, nominally 500 steps at dT = 0.08. Its first 20–50 steps provide timing data. Check pre-impact propagation and the post-collision central fields before the main matrix. Record peak memory, time per step, thread count, commands, source revision and realization identifiers. Use separate output directories and run sequentially on the 32 GB home PC. Any initial single-nucleus diagnostic is short and supports this pilot.

Compare spatial differences on a common physical grid at shared raw times, for example t = 0, 40 and 88. Add aligned plots at Q_s s = 0, 5 and 8 using each grid's tabulated nominal origin and documented interpolation. Report quadrature-weighted L2 differences, normalized energy and pressure histories, and maximum and RMS Gauss-law residuals. Separate initial differences from subsequent evolution and show how much profile alignment changes the comparisons.

For different transverse meshes, report event means and statistical uncertainty when available. A one-seed comparison using the legacy generator includes both realization differences and spatial effects; it is a preliminary sensitivity comparison, not a quantitative discretization-error estimate.

Do not assume second-order convergence for this legacy-placement study. The center formulas introduce position errors of order a_z relative to their continuum locations, with nonuniform rounding between the chosen grids. Unaligned differences can be substantial. Alignment removes the leading translation and timing offsets; the remaining placement effect is expected to be smaller, but has not been measured in the nonlinear collision. Keeping the old placement is an accepted choice for this demonstration. Freezing centers or replacing the generator would be a separate later study.

For the temporal check, subtract the controls separately at each dT, then compare the two finest-grid collision-induced tensors at equal physical times. Once medium–fine spatial differences are distinguished from stochastic variation, use a time-step difference below 10% of that spatial difference as a working screen. Near-zero or statistically unresolved spatial differences make the ratio inconclusive. If temporal sensitivity is appreciable, revisit the common main time step and document the decision. A single halving does not establish temporal convergence order or an absolute accuracy guarantee.

## Cost and decisions to refine

Memory per run should remain similar to the propagation study because both source sectors are already allocated. At t_end = 88, evolution-only extrapolations are about nine hours with one thread for four collision-only runs and roughly 26 hours including both controls, with an initial 5–13-hour budget on 8 threads. These are per source pair, exclude initialization and measurement overhead, and remain rough estimates; replace them with collision-pilot timings using the actual output schedule. Additional events multiply the work. The measured coarse-grid CPU sweep supports starting with 8 threads, but is not a collision benchmark.

The fixed inputs and initial sequence above are sufficient to begin implementation. Event counts, final accuracy targets, analysis region and the value of extra output should be decided from the pilot and historical data. Record later changes to the default endpoint rather than silently adopting another horizon. Deliver profile and pressure comparisons, spatial and temporal difference tables, diagnostic histories and measured resource costs. Conclusions will apply to the analyzed source ensemble, fixed box and finite time interval.

## Reference

S. Schlichting and P. Singh, *3-D structure of the Glasma initial state*, Physical Review D 103, 014003 (2021), Secs. II and III and Appendix A. [Initial conditions, collision observables and matched single-nucleus subtraction](https://arxiv.org/html/2010.11172).
