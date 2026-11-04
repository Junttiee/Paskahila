# Single nucleus reference and validation plan

4 November 2026

Status: Reference and supporting validation. Read [PLANS.md](PLANS.md) first. The active work is the collision study; completing this standalone matrix is not a prerequisite. Use single-nucleus runs as short diagnostics or matched controls for the active collision configuration. The optional standalone matrix and full collision-control runs share t_end = 88.

## Objective

Check the propagation of one nucleus in the classical Yang–Mills solver when needed to interpret collision results. The reference matrix refines all three directions at dT = 0.08 and includes a finest-grid dT = 0.04 comparison. Preserve the established generator and center formulas. At each fixed transverse mesh, use the same source realization for temporal comparisons and collision controls. Cross-grid comparisons with different transverse meshes must account for stochastic variation. These checks do not establish the full space-time continuum limit or independence from box size.

## Fixed physics and geometry

All lengths and momenta below are in simulation units. Use the source-model convention in which the initializer amplitude is called Q_s; the earlier symbol Q denotes the same input scale in this plan.

| Parameter | Value |
|---|---:|
| Gauge group and configuration | SU(2), single right-positioned nucleus |
| Box lengths (L_x, L_y, L_z) | (128, 128, 256) |
| Source strength Q_s | 0.125 |
| Dimensionless longitudinal thickness Q_s σ_z | 0.5 |
| Gaussian standard deviation σ_z | 4 |
| Infrared regulator m | 0.125, retaining m/Q_s = 1 |
| Ultraviolet regulator Λ | 0.625, retaining Λ/Q_s = 5 |
| Main time step dT = Δt | 0.08 |
| Finest-grid time-step check dT | 0.04 |
| Ending time t_end | 88 for every full run |
| Dimensionless ending time Q_s t_end | 11 |
| Boundary conditions | Periodic in x and y; current open treatment in z |

Thus Q_s L_x = Q_s L_y = 16 and Q_s L_z = 32. Hold the regulators fixed in simulation momentum units as the spacings change. The number 0.125 specifies Q_s here; Q_s a_perp equals 0.125 only on the finest proposed grid.

## Three spatial grids and one time step check

| Parameter | Coarse | Medium | Fine | Fine time-step check |
|---|---:|---:|---:|---:|
| (N_x, N_y, N_z) | (64, 64, 256) | (96, 96, 384) | (128, 128, 512) | (128, 128, 512) |
| a_x = a_y | 2 | 4/3 | 1 | 1 |
| a_z | 1 | 2/3 | 0.5 | 0.5 |
| Width input σ_z/a_z | 4 | 6 | 8 | 8 |
| Q_s a_perp | 0.25 | 1/6 | 0.125 | 0.125 |
| Λ a_perp | 1.25 | 5/6 | 0.625 | 0.625 |
| dT | 0.08 | 0.08 | 0.08 | 0.04 |
| dT/a_z | 0.08 | 0.12 | 0.16 | 0.08 |
| Steps to t_end = 88 | 1,100 | 1,100 | 1,100 | 2,200 |
| Number of sites | 1,048,576 | 3,538,944 | 8,388,608 | 8,388,608 |
| Solver RAM budget | About 1.2 GiB | About 4 GiB | About 9 GiB | About 9 GiB |
| Total work relative to coarse main run | 1 | 3.375 | 8 | 16 |
| Estimated evolution time with one thread | 19 min | 62 min | 2 h 28 min | 4 h 55 min |

Every level satisfies N_i a_i = L_i and a_z/a_perp = 1/2. Use sufficiently precise decimal inputs for 4/3 and 2/3. The check repeats the finest spatial grid with half the time step and twice the steps. The coarse grid is intentionally rough; eight longitudinal cells per σ_z on the finest level do not by themselves establish convergence.

## Preparation required for a controlled comparison

1. **Fix the actual source amplitude.** The active initializer currently receives `Qs0A_L`, while `-QsA` changes a different variable. Ensure the amplitude actually used and the recorded value both equal 0.125. Set m and Λ explicitly after parsing; defaults derived before parsing must not redefine the benchmark.
2. **Retain the existing source centers.** The current right-nucleus formula gives indices 146, 219 and 293, or z = 146, 146 and 146.5 on these grids. Log these values and use the identical geometry in each corresponding collision control. Compare translated profiles where useful. A new fixed-center prescription is not part of the current implementation scope.
3. **Preserve the existing random sampling.** Use seed 1 for new pilots unless a historical-data comparison requires another recorded seed. Keep the generator, normalization and draw order unchanged. The same seed on different transverse meshes does not produce a matched physical realization; use event statistics before interpreting such differences as numerical errors. A Fourier-mode sampling replacement would be a separate future study.
4. **Normalize observables consistently.** The bulk T00 output is a sum over sites. Use mean density T00_bulk/(N_x N_y N_z), or integrated energy (a_x a_y a_z) T00_bulk with the current scale factor of one. Longitudinal profiles already average over x and y. Convert their first column from site index to z = index × a_z before comparison.

## Run protocol

If the optional standalone matrix is used, start with 20–50-step timing pilots, initially using 8 CPU threads. Confirm the recorded inputs, finite output, recorded source geometry, peak memory and time per step. Run sequentially on the i7-13700K with 32 GB RAM; confirm sufficient memory is available inside WSL before the finest run. Keep the selected, recorded thread count fixed across all runs. Short timing pilots at different dT are not comparisons at equal physical time.

CPU threading is now available through `OMP_NUM_THREADS`, and `benchmark-threads.py` provides a repeatable sweep. A preliminary 100-step coarse-grid performance run with the current short-test physics took 200.9 seconds with one thread, 33.4 with eight and 30.4 with sixteen; all numerical comparisons passed. Use eight threads initially, then confirm the choice on the final study configuration. These preliminary runs use dT = 0.008 and do not replace the source-model preparation or study-specific pilots above.

If this standalone reference matrix is used, set t_end = 88 explicitly, giving Q_s t_end = 11. The target counts are 1,100 steps at dT = 0.08 and 2,200 at dT = 0.04. This replaces the current source ending-time default of 0.8 and requires rebuilding. Preserve the legacy stopping loop initially, record any extra step from floating-point accumulation, and compare common saved frames. The expected right-positioned nucleus centers at the endpoint are z = 58, 58 and 58.5. Collision controls inherit the active collision plan's geometry and output schedule, with the same full-run endpoint of 88.

Save comparison profiles at t = 0, 8, …, 88, corresponding to Q_s t = 0, 1, …, 11. These are every 100 steps in the main runs and every 200 steps in the check. Compare energy and Gauss-law diagnostics every 20 main steps and 40 check steps, giving a shared physical interval of 1.6. Additional intermediate outputs may be retained. Reuse exactly the same finest-grid initial charge realization and center for the check, and initialize staggered fields consistently with its own dT. Retain commands, source revision, realization identifier, logs and outputs in separate run directories.

These output times are reported labels. For quantitative temporal comparisons, use the physical measurement-time mapping and initial-frame treatment described in [PLANS.md](PLANS.md); equal reported times at different dT need not be coeval measurements.

The one-thread evolution-time estimates extrapolate the completed small run and total roughly nine hours for all four runs, before initialization and measurement overhead. The RAM budgets are allocation estimates, not measured peaks. An initial budget with 8 threads is about 2–5 hours for this optional matrix, assuming a 2–5-fold speed-up; replace this estimate with study-specific measurements.

## Analysis and interpretation

Compare transverse-averaged T00(z,t) and T0z(z,t) at t = 0, 40 and 88, and inspect their evolution at the other saved times. Put the profiles on a common physical z grid using the same documented interpolation or projection. Report relative, quadrature-weighted L2 differences between adjacent spatial levels at dT = 0.08, together with normalized energy histories and maximum and RMS Gauss-law residuals. Include initial differences as part of the spatial error; report subsequent propagation changes separately. Use translation toward decreasing z at unit speed as an additional single-nucleus propagation diagnostic.

For the finest grid, compare dT = 0.08 and 0.04 at the same physical times. Once spatial differences are distinguished from stochastic and placement effects, use a time-step difference below 10% of the medium–fine spatial difference as a working screen. Near-zero or statistically unresolved spatial differences make the ratio inconclusive. One time-step halving measures sensitivity; it does not establish temporal convergence order or rigorously bound the remaining temporal error.

Treat the fine grid as a comparison reference, not an exact solution. Because the spacing ratios are unequal, do not use a factor-of-two convergence formula. Retained position rounding and different source realizations can affect trends; this reference matrix alone does not justify a second-order error fit. A more controlled convergence-order study is outside the current minimal-change implementation.

Deliver a profile comparison figure, spatial and temporal difference tables, diagnostic histories and measured resource costs. State whether refinement produces a consistent trend, whether the time-step check meets the working criterion, and whether the chosen levels appear adequate for this realization and time interval. Residual temporal error remains, and simultaneous spatial refinement cannot identify the separate transverse and longitudinal contributions.

## Reference

S. Schlichting and P. Singh, *3-D structure of the Glasma initial state*, Physical Review D 103, 014003 (2021), Sec. III and Appendix A. [Model definitions and discretization discussion](https://arxiv.org/html/2010.11172#S3). The longitudinal width denoted R_gamma there is the Gaussian standard deviation denoted σ_z here.
