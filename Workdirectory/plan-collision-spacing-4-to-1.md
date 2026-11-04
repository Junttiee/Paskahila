# Collision study plan with four to one spacing

4 November 2026

Status: The initial Grid A pilot and two-grid dT=0.08 batch used the pre-correction transverse source normalization and remain a historical baseline. The corrected Grid A pilot and Grid A/B t=88 main runs completed on 4 November 2026; see [PLANS.md](PLANS.md) for run records. The optional Grid B dT=0.008 extension remains pending. This is the friend's two-grid collision series, with a_perp:a_z = 4:1. The separate [2:1 plan](plan-collision-spacing-2-to-1.md) has a different run matrix and time-step check.

## Purpose and compatibility

Compare two collision lattices with longitudinal spacing one quarter of the transverse spacing. Begin with dT = 0.08. Consider a later dT = 0.008 run on the finer of these two grids. The grid and time-step schedule are unchanged by the source correction; pre-correction results remain a separate historical baseline.

Retain the evolution equations, random-number generator and draw order, center formulas, boundary treatment, floating-point options and output column definitions. Use a cell-centered density convention for the transverse white-noise source: its per-site variance scales as 1/(a_x*a_y), so the Gaussian amplitude scales as 1/sqrt(a_x*a_y). The Poisson solver divides by physical p^2 and therefore consumes density, not cell-integrated charge. Any source edits should be small, recorded and kept with the run manifest. A new initial-condition generator is not part of this plan.

## Shared physical inputs

| Parameter | Value |
|---|---:|
| Model | SU(2), two colliding nuclei |
| Box lengths (L_x, L_y, L_z) | (128, 128, 256) |
| Source strength of each nucleus Q_s | 0.125 |
| Thickness of each nucleus Q_s σ_z | 0.5 |
| Gaussian standard deviation σ_z | 4 |
| Infrared regulator m | 0.125 |
| Ultraviolet regulator Λ | 0.625 |
| Charge-density multipliers | Left = 1, right = 1 |
| Boundaries | Existing periodic x/y and open z treatment |
| Main time step | dT = 0.08 |
| Optional later time step | dT = 0.008 |
| Main-run endpoint | t_end = 88 from initialization |

All lengths and momenta are in simulation units. Q_s is the source-strength convention of the current model. Q_s is held fixed in physical simulation units; it is not Q_s a_perp.

For one color component and fixed z, the corrected source must satisfy `(a_x*a_y)*Var(rho_i) = (NucleusMultiplier*Q_s*T(z))^2`. Equivalently, after dividing out the nucleus multiplier and longitudinal profile, `(a_x*a_y)*Var(rho_i/[NucleusMultiplier*T(z)]) = Q_s^2` on both grids. Validate this source statistic before interpreting another full collision comparison.

## Requested grids

| Parameter | Grid A | Grid B |
|---|---:|---:|
| (N_x, N_y, N_z) | (32, 32, 256) | (64, 64, 512) |
| (a_x, a_y, a_z) | (4, 4, 1) | (2, 2, 0.5) |
| a_z/a_perp | 1/4 | 1/4 |
| Both longitudinal width inputs σ_z/a_z | 4 | 8 |
| Q_s a_perp | 0.5 | 0.25 |
| Λ a_perp | 2.5 | 1.25 |
| dT/a_z at dT = 0.08 | 0.08 | 0.16 |
| dT/a_z at dT = 0.008 | 0.008 | 0.016 |
| Steps targeting t_end = 88 at dT = 0.08 | 1,100 | 1,100 |
| Steps targeting t_end = 88 at dT = 0.008 | 11,000 | 11,000 |
| Number of sites | 262,144 | 2,097,152 |
| Estimated solver RAM budget | About 0.3 GiB | About 2.3 GiB |
| Work per step relative to Grid A | 1 | About 8 |

These choices preserve N_i a_i = L_i. Grid B halves every spatial spacing, so the two-grid comparison includes both transverse and longitudinal effects. The z spacing is finer relative to the transverse spacing than in the first collision plan; the absolute z spacings equal that plan's coarse and fine values. The transverse resolution is comparatively coarse, especially on Grid A, so treat these as preliminary sensitivity levels rather than established continuum results. A 128 × 128 × 1024 run is outside the current scope.

## Retained initial centers and time origin

Use the current formulas exactly, including integer truncation:

**i_L = int(0.85 N_z/2 − 1), i_R = int(1.15 N_z/2 − 1).**

| Quantity | Grid A | Grid B |
|---|---:|---:|
| Left and right center indices | (107, 146) | (216, 293) |
| Physical centers (z_L, z_R) | (107, 146) | (108, 146.5) |
| Nominal crossing position z_c | 126.5 | 127.25 |
| Nominal crossing time t_coll | 19.5 | 19.25 |
| Q_s (t_end − t_coll) at t_end = 88 | 8.5625 | 8.59375 |

Report raw coordinates and aligned coordinates s = t − t_coll, ζ = z − z_c. At the endpoint Q_s t_end = 11 from initialization. Nominal impact is the center-crossing convention, not an instantaneous collision of finite-width profiles. Retain the accepted placement differences and assess their contribution through aligned comparisons. The nominal endpoint centers remain well inside the box.

## Minimal preparation before running

1. Select and rebuild the existing collision procedure. Keep separately identified left-only and right-only builds if background subtraction is needed. The corrected transverse source variance was checked on both grids before the corrected batch; the initial historical batch used the old normalization.
2. Set the amplitude actually passed to the initializer to Q_s = 0.125. The current calls use `Qs0A_L` for both nuclei; `-QsA` alone does not set that value. For the symmetric case, a small configuration edit can make the actual and recorded source strengths agree. Pass m, Λ, widths, dimensions, spacings and dT explicitly using the existing options. Set the compile-time endpoint to 88.
3. Keep the existing random sampling. At a fixed grid, reuse the seed and generation order for collision/control runs and for the optional smaller-time-step run. Across different N_x and N_y, the same seed does not produce the same physical realization. Start with a common seed for workflow checks, but use available event ensembles and uncertainty estimates before interpreting cross-grid differences as numerical error. Historical events can help if their actual settings match.
4. Preserve the existing evolution stopping loop initially. Floating-point time accumulation can add a final step; record the actual count and compare common saved times rather than changing the algorithm solely to enforce the nominal count.

## Initial run sequence and optional extension

Use seed 1 for new pilots unless a documented historical comparison requires a different seed. Begin with a short timing and output check on Grid A, then a short collision pilot through t = 40, nominally 500 steps. Check initial Gauss-law residuals, incoming propagation and the central post-collision fields. Proceed to t = 88 on both grids once the pilot is satisfactory. Select and record a fixed thread count appropriate to the friend's machine; our measurements support eight threads as a starting point where available.

The initial scope is two full-collision runs at dT = 0.08. If collision-induced energy and stresses are required, add matched left-only and right-only controls at each grid, giving six runs. Form T_induced = T_collision − T_left − T_right component by component.

The optional next run uses Grid B at dT = 0.008, with the same source realization, geometry and endpoint. It is a later extension, not part of the initial batch. This adds one collision run, or three runs when including its matched controls. It requires ten times as many evolution steps as the dT = 0.08 run. No dT = 0.04 run or third spatial grid is scheduled for this series.

## Outputs and comparisons

Keep the legacy output format and initially retain profiles every 100 steps. At dT = 0.08 this gives reported times t = 0, 8, …, 88; the dT = 0.008 run additionally has 0.8-unit sampling. Selecting every tenth smaller-step profile matches reported counters, not necessarily physical measurement times. Before a quantitative temporal comparison, establish the staggered-time mapping described in [PLANS.md](PLANS.md) and interpolate from sufficiently close samples; treat the initial frame separately. If finer sampling is needed near impact or for aligned comparisons, make only an explicit output-cadence adjustment and record it. Interpolation across an eight-unit gap should not support a precision claim near impact.

Compare transverse-averaged T00, T0z, transverse pressure (TXX + TYY)/2 and longitudinal pressure TZZ. Convert z indices to physical coordinates. Normalize the bulk site sum as mean density T00_bulk/(N_x N_y N_z), or integrated energy (a_x a_y a_z) T00_bulk with the current scale factor of one. Record maximum and RMS Gauss-law residuals, initial differences, resource costs and statistical uncertainty. Two spatial levels cannot establish convergence order; the optional time-step run tests sensitivity for its chosen grid and realization.

A useful comparison with the [2:1 collision plan](plan-collision-spacing-2-to-1.md) is Grid B here versus its 64 × 64 × 256 grid: they share the transverse mesh while a_z changes from 1 to 0.5. With identical seed, draw order and actual physics parameters, the existing generator can match their transverse source fluctuations without a new sampling algorithm. The retained position shifts still need alignment and documentation.

## Historical data and deliverables

Before combining new and old results, record the source version, executable/build settings, actual source amplitude, seed, dimensions, spacings, widths, cutoffs, dT, endpoint, center rule, boundary treatment and output definitions. Existing info-file labels alone may not identify the amplitude actually used. Keep any historical configuration differences explicit.

Deliver the run manifests, initial and evolved profile plots, pressure histories, resolution and optional time-step differences, diagnostics and measured runtimes. This plan specifies the implementation starting point; numerical adequacy and historical-data compatibility must be assessed from the pilot and available metadata. Event counts, final accuracy targets and the optional smaller-time-step extension remain later research decisions.
