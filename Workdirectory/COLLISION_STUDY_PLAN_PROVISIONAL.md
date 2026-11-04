# Provisional collision discretization study plan

4 November 2026

## Aim and scope

Study a symmetric collision of two longitudinally extended nuclei in the SU(2) classical Yang–Mills solver using the three spatial grids discussed for single-nucleus propagation. Compare spatial sensitivity at dT = 0.08 and add a finest-grid check at dT = 0.04. Use one matched pair of random source realizations initially. This is a numerical demonstration with simplified source profiles, not a calibrated nuclear-collision prediction.

The working proposal follows the system until Q_s times the elapsed time after nominal center crossing equals 10. The endpoint and analysis criteria below remain provisional.

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
| Left-positioned nucleus center and motion | z_L = 104, moving toward +z |
| Right-positioned nucleus center and motion | z_R = 152, moving toward −z |
| Collision midpoint | z_c = 128 |
| Boundary conditions | Periodic in x and y; current open treatment in z |

All quantities use simulation units. Q_s denotes the source-strength convention of the existing model. The two nuclei have independent transverse fluctuations; each realization must be reused across spatial grids, time steps and its corresponding single-nucleus control.

The initial centers are 48 units apart and each lies six Gaussian widths from the midpoint. Initial overlap there is negligible. At unit propagation speed, nominal center crossing occurs at t_coll = 24. Define post-collision coordinates s = t − 24 and ζ = z − 128. Use this same nominal origin on every grid; any grid-dependent shift in crossing time is part of the discretization result.

## Spatial grids and time steps

| Parameter | Coarse | Medium | Fine | Fine time-step check |
|---|---:|---:|---:|---:|
| (N_x, N_y, N_z) | (64, 64, 256) | (96, 96, 384) | (128, 128, 512) | (128, 128, 512) |
| a_x = a_y | 2 | 4/3 | 1 | 1 |
| a_z | 1 | 2/3 | 0.5 | 0.5 |
| Both width inputs σ_z/a_z | 4 | 6 | 8 | 8 |
| Left center index | 104 | 156 | 208 | 208 |
| Right center index | 152 | 228 | 304 | 304 |
| Q_s a_perp | 0.25 | 1/6 | 0.125 | 0.125 |
| dT | 0.08 | 0.08 | 0.08 | 0.04 |
| dT/a_z | 0.08 | 0.12 | 0.16 | 0.08 |
| Steps to proposed t_end = 104 | 1,300 | 1,300 | 1,300 | 2,600 |
| Estimated RAM per solver process | About 1.2 GiB | About 4 GiB | About 9 GiB | About 9 GiB |

Each grid preserves the box lengths and a_z/a_perp = 1/2. Use precise decimal inputs for 4/3 and 2/3. The finest spatial grid is a comparison reference, not an established continuum solution.

## Endpoint options

| Choice | Simulation endpoint | Q_s s_end | Steps at dT = 0.08 / 0.04 |
|---|---:|---:|---:|
| Working proposal | t_end = 104 | 10 | 1,300 / 2,600 |
| Shorter alternative from the propagation plan | t_end = 80 | 7 | 1,000 / 2,000 |

For the working proposal, Q_s t_end = 13 from initialization, while Q_s s_end = 10 after nominal impact. Expected nucleus centers at t = 104 are z = 208 and 48, still well inside the box. Check the evolved profiles and boundary diagnostics rather than relying only on these nominal trajectories.

## Implementation preparation

1. Select the existing two-nucleus procedure for collision runs, and the existing left-only and right-only procedures for controls. Build identifiable executables from the same source revision. The collision initializer already constructs fields and currents for both incoming nuclei.
2. Make the physical centers explicit instead of using the current grid-dependent integer formulas. Set both widths from the table. Ensure the actual source amplitudes equal 0.125: the current right-nucleus initialization call also receives `Qs0A_L`, while the command-line `-QsA` option changes another variable. Set and record m and Λ explicitly.
3. Generate the left and right random fields independently using physical Fourier-mode identifiers, with consistent normalization and reality constraints. Share each nucleus's coefficients across meshes before the lattice filter. Fix the treatment of unresolved modes. Sampling the current site-based generator with the same seed on different meshes is insufficient.
4. Set the endpoint explicitly and stop at the required integer step count. Initialize staggered fields consistently with each dT. Check initial Gauss-law residuals and verify that the nuclei propagate correctly before appreciable overlap.

## Run matrix and measurements

For each of the four configurations, run the full collision, the left nucleus alone and the right nucleus alone. Keep the source pair, positions, physical inputs and output times matched. This gives 12 runs for the full study. The four full-collision runs alone are sufficient for an initial total-field demonstration.

To measure the collision-induced contribution, subtract the matched control tensors component by component:

**T_induced(t,z) = T_collision(t,z) − T_left(t,z) − T_right(t,z).**

Inspect transverse-averaged T00, T0z, transverse pressure (TXX + TYY)/2 and longitudinal pressure TZZ. Check that the subtracted signal is small before appreciable overlap. Convert profile indices to physical z. Normalize the bulk site sum as either mean density T00_bulk/(N_x N_y N_z) or integrated energy (a_x a_y a_z) T00_bulk with the current scale factor of one.

Save shared comparison profiles at t = 0, 8, …, 104: every 100 main steps or 200 check steps. These include nominal impact at t = 24 and Q_s s = 0, 1, …, 10 afterward. Compare energy, source terms and Gauss-law diagnostics at shared physical intervals, for example every 20 main steps and 40 check steps.

## Checks and analysis

Begin with 20–50-step timing pilots, initially using 8 CPU threads, then run the coarse collision and its controls through impact before committing to the complete matrix. Record peak memory, time per step, thread count, commands, source revision and realization identifiers. Use separate output directories and run sequentially on the 32 GB home PC.

Compare spatial differences on a common physical grid at shared times, including Q_s s = 0, 5 and 10. Report quadrature-weighted L2 differences, normalized energy and pressure histories, and maximum and RMS Gauss-law residuals. Separate initial discretization differences from subsequent evolution. Use the unequal spacing ratios in convergence fits; for a scalar O(h) = O_0 + C h² with h = a_perp, the expected signed coarse–medium to medium–fine difference ratio is 20/7.

For the temporal check, subtract the controls separately at each dT, then compare the two finest-grid collision-induced tensors at equal physical times. Provisionally require the time-step difference to be below 10% of the medium–fine spatial difference in the principal observables. Near-zero spatial differences make that ratio inconclusive. If temporal sensitivity is appreciable, reconsider the common main time step. A single halving does not establish temporal convergence order.

## Cost and decisions to refine

Memory per run should remain similar to the propagation study because both source sectors are already allocated. At t_end = 104, the four collision-only runs extrapolate to about 10 hours with one thread; adding both controls gives roughly 31 hours. With an assumed 2–5-fold speed-up on 8 threads, the complete matrix would take roughly 6–16 hours. These are planning estimates from the small propagation run, not collision benchmarks; replace them with pilot measurements.

Refine the endpoint choice, output cadence near impact, treatment of unresolved random modes, analysis region and error criterion after the coarse pilot. The first deliverables are profile and pressure comparisons, spatial and temporal difference tables, diagnostic histories and measured resource costs. Conclusions will apply to this source realization, fixed box and finite time interval.

## Reference

S. Schlichting and P. Singh, *3-D structure of the Glasma initial state*, Physical Review D 103, 014003 (2021), Secs. II and III and Appendix A. [Initial conditions, collision observables and matched single-nucleus subtraction](https://arxiv.org/html/2010.11172).
