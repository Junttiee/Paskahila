# Study plans and implementation handoff

4 November 2026

## Start here

The goal is a collision discretization and resolution-sensitivity demonstration using the existing SU(2) classical Yang–Mills code with minimal changes. The plans are ready to guide implementation and pilots. Their parameters are specified; numerical adequacy and scientific conclusions still require measurements.

| Document | Role and scope |
|---|---|
| [Collision plan with 2 to 1 spacing](plan-collision-spacing-2-to-1.md) | Active local series: 64 × 64 × 256, 96 × 96 × 384, 128 × 128 × 512; dT = 0.08; finest-grid dT = 0.04 check |
| [Collision plan with 4 to 1 spacing](plan-collision-spacing-4-to-1.md) | Friend's series: 32 × 32 × 256 and 64 × 64 × 512; dT = 0.08; optional later finer-grid dT = 0.008 run |
| [Single nucleus reference](plan-single-nucleus-reference.md) | Supporting diagnostics and historical study outline; not a required standalone campaign |

The ratio in each collision filename is a_perp:a_z. Both series keep the physical box (128, 128, 256). Do not add a 128 × 128 × 1024 run to the friend's series. Keep each series' time-step check distinct.

## Shared decisions

- Preserve the established evolution equations, source normalization, random-number generator and draw order, center formulas, boundaries, floating-point options and output columns. Start with configuration edits and thin launchers. Additional output frames are a small documented measurement change, not a reason to rewrite dynamics.
- For both nuclei use Q_s = 0.125, Q_s σ_z = 0.5, σ_z = 4, m = 0.125 and Λ = 0.625 in simulation units. Q_s is the input source-strength convention; do not reinterpret 0.125 as Q_s a_perp or assign GeV/fm units without another specification.
- Retain i_L = int(0.85 N_z/2 − 1) and i_R = int(1.15 N_z/2 − 1), including truncation. Record physical positions, nominal crossing time and midpoint. Use raw and appropriately sampled aligned comparisons; a fixed-center rewrite is outside the current implementation scope.
- Keep the legacy generator. Identical seed and draw order match sources at the same transverse grid for collision/control and time-step comparisons. The same seed across different transverse meshes does not match physical realizations. A new Fourier-mode generator is not required or scheduled.
- Default to seed 1 for new pilots. Use another seed when explicitly matching documented historical data. Record the seed in every run; ensemble seed lists and counts will be chosen later.
- Initially use eight CPU threads on the local i7-13700K. Run configurations sequentially. Benchmark the friend's own resources before choosing its fixed thread count.
- All full study runs, including the single-nucleus reference, use t_end = 88 from initialization. This gives Q_s t_end = 11 and nominally 1,100 steps at dT = 0.08. For collisions, Q_s times the elapsed time after nominal impact ranges from 8.5625 to 8.625 across these grids. The short coarse collision pilot still ends at t = 40, nominally 500 steps; its first 20–50 steps provide timing data.
- Collision fields are the first target. To isolate collision-induced T00, T0z and pressures, reuse matched left-only and right-only controls and subtract their tensors. This is supporting collision work, not a separate convergence campaign that must finish first.

## Current implementation state

As of this handoff, the source still selects `RightNucleusSimulation.cpp` in `SourceCode/SUNcSolver.cpp`. The collision procedure exists in `SourceCode/Simulation.cpp`; left-only and right-only procedures also exist. Exactly one procedure include must be active when building. An executable's name does not select its physics.

The study physics has not yet been configured or run. In `SourceCode/LATTICE/Parameters.cpp`, the source defaults remain expressions beginning with 0.25, and `EndSimulationT` remains 0.8. The current transverse-spacing defaults are one, so the initializer amplitude is currently 0.25. Both initialization calls receive `Qs0A_L`. The command-line `-QsA` option changes `Qs0A`, a different variable. Make the actual and recorded strengths agree: a minimal configuration edit can set `Qs0A`, `Qs0A_L` and `Qs0A_R` to the fixed simulation value 0.125. Do not depend on runtime spacing changes to recompute static defaults.

Existing runtime options include dimensions, spacings, `-dT`, both width inputs, cutoffs, seed, density multipliers and output directory. There is no endpoint option. Set `EndSimulationT` for each build and rebuild. Preserve the stopping loop initially: floating-point accumulation can produce an extra final step, so record actual counts and compare common saved times. The targeted additional output frames described in the 2:1 plan are not implemented yet.

CPU threading is already implemented and tested. `run-small.sh` honors `OMP_NUM_THREADS`; the solver uses OpenMP and threaded FFTW. A 100-step coarse-grid performance sweep took 200.9 seconds with one thread, 33.4 with eight and 30.4 with sixteen. Numerical comparisons passed. Results are in `runs/thread-benchmark-2c2gsgcw/`. These runs used the current short-test physics and dT = 0.008, not the study configuration.

`run-small.sh` still passes the original 16 × 16 × 256 smoke-test arguments. `benchmark-threads.py` expects source ending time 0.8. Neither is a collision-study launcher. Existing binaries and historical output must not be assumed to match the plans.

## First implementation steps

1. Read the selected collision plan and [the source and output guide](GETTING_STARTED.md). Preserve existing local changes and historical data. Record the source revision and local configuration differences used for each executable.
2. Prepare a collision-pilot build by selecting `Simulation.cpp`, setting the actual and recorded source amplitudes to 0.125, and setting `EndSimulationT = 40`. Keep the center and sampling algorithms unchanged. Use a separate pilot executable; do not overwrite historical binaries.
3. Compile only `SourceCode/SUNcSolver.cpp` from Ubuntu/WSL, using the working recipe below. Build the pilot from the prepared source before launching it.
4. Create a fresh run directory, pass all dimensions and all spacings explicitly, and use the planned widths, cutoffs, seed and dT. Unknown options are silently ignored, and `--help` is not a harmless help mode. Verify recorded values against the manifest rather than relying on defaults.
5. Inspect the pilot's finite outputs, initial geometry, pre-impact propagation, post-impact central fields, Gauss-law diagnostics, completion, actual endpoint, peak memory and runtime. Profile files should contain N_z rows. Small Gauss-law residuals alone do not establish physical accuracy. Investigate unexplained diagnostic growth before extending the matrix.
6. Once the pilot is satisfactory, set `EndSimulationT = 88` and build separately identified main executables. Proceed with the selected plan's matrix and matched controls. Keep the optional dT = 0.008 extension in the 4:1 series separate from its initial batch.

After the source configuration in step 2 is prepared, a local 2:1 coarse pilot can use:

```bash
mkdir -p build runs
g++ -std=gnu++14 -O2 -fopenmp \
  SourceCode/SUNcSolver.cpp \
  -o build/CollSUNcSolver-pilot-t40 \
  -lgsl -lgslcblas -lfftw3_omp -lfftw3 -lm

export OMP_NUM_THREADS=8
export OMP_DYNAMIC=FALSE
run_dir=$(mktemp -d runs/collision-spacing-2-to-1-pilot-XXXXXX)
./build/CollSUNcSolver-pilot-t40 \
  -Nx 64 -Ny 64 -Nz 256 \
  -ax 2 -ay 2 -az 1 \
  -dT 0.08 -RlOverAzL 4 -RlOverAzR 4 \
  -QsA 0.125 -MEff 0.125 -Lambda 0.625 \
  -NucleusL 1 -NucleusR 1 -SEED 1 -o "$run_dir" \
  > "$run_dir/run.log" 2>&1
```

For the friend's 4:1 pilot, use its Grid A instead: dimensions 32, 32, 256 and spacings 4, 4, 1. The widths remain 4, 4. Neither command fixes the amplitude/endpoint wiring by itself; the source preparation and fresh build are essential.

## Outputs and completion of the first stage

Retain a manifest with the plan name, procedure, actual source parameters, dimensions/spacings, centers, seed, dT, endpoint, thread count, source/build identity and command. Keep each run in its own directory. `InfoID` contains only part of the required information.

The longitudinal EMT columns are z_index, T00, TXX, TYY, TZZ, T0X, T0Y, T0Z, followed by six alternative `mod` definitions. Use the first definitions consistently unless a separate comparison is intended. Bulk energy has a six-column initial row and four-column subsequent rows; parse these separately. Bulk T00 is a site sum, so normalize by site count for mean density or multiply by cell volume for integrated energy. The profile already averages over the transverse grid. Convert z_index to physical z.

Before quantifying time-step differences, trace and document measurement-time staggering in the unchanged code. `DYNAMICS/SUNcDynamics.cpp` saves `UOld` between its two half link updates and increments the reported time after the second half update. `OBSERVABLES/EnergyMomentumTensor.cpp` computes the main profile from `UOld` and `E`, while the initializer first constructs links one step apart to obtain the electric fields. Matching filenames/counters across different dT therefore does not by itself establish identical physical measurement times, and the initial frame needs separate treatment. Preserve the numerical scheme and raw labels; establish the physical-time mapping in analysis and use sufficiently close bracketing samples for interpolation. Bulk diagnostics have their own staggering and must not automatically inherit the profile convention.

The first stage is complete when a correctly configured coarse collision has reached t = 40 and its manifest, profiles, diagnostic histories and measured costs have been reviewed. The full study additionally requires the main grid comparisons and its selected temporal check, plus matched controls when reporting the induced tensor.

Event counts, a final quantitative accuracy target, the analysis region, historical-data compatibility and the friend's optional smaller-step run remain research decisions. They do not prevent implementing the specified pilot. Report one-seed, different-transverse-grid differences as combined stochastic and resolution sensitivity until uncertainty is controlled. Removing draft status from these plans does not certify the simulation or establish convergence.
