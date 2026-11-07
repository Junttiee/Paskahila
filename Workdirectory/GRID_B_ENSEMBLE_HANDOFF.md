# Grid B ensemble handoff

7 November 2026

All 48 runs in the Grid B ensemble completed and passed validation. The queue for seeds 2–8 ran from 5 November at 23:16 to 7 November at 18:40 Helsinki time, about 43 h 24 min. The total recorded wall time for the 48 runs, including seed 1, was 49 h 58 min. [Ensemble plan](plan-collision-spacing-4-to-1-grid-b-ensemble.md) · [Seed-1 pilot](GRID_B_SEED1_HANDOFF.md)

## Run audit

Each of the eight seeds has a collision and both matched single-nucleus controls at each timestep. The 48 manifests agree on the corrected source hashes and executable SHA-256 `09818024fe227a1d60195555421f67c1d037da28d3884f381c439892c42f8594`, built from `2feb0d629954280be377836d9d1359ce0c2e276e`. Each run used eight threads and produced 111 finite EMT profiles of 512 rows and 14 columns at reported times 0 through 88. All coarse runs ended after 1,101 steps at `t≈88.08`. All fine runs ended after 11,001 steps at `t≈88.008`. [Manifest index](results/grid-b-ensemble/manifest-index.csv)

| `dT` | Run type | Runs | Mean wall time (s) | Range (s) | Peak RSS range (KiB) |
|---|---|---:|---:|---:|---:|
| 0.08 | Collision | 8 | 2,188 | 2,128–2,304 | 2,080,128–2,080,324 |
| 0.08 | Left-only | 8 | 2,188 | 2,128–2,317 | 2,079,892–2,080,252 |
| 0.08 | Right-only | 8 | 2,184 | 2,136–2,305 | 2,080,008–2,080,284 |
| 0.008 | Collision | 8 | 5,290 | 5,182–5,551 | 2,079,932–2,080,260 |
| 0.008 | Left-only | 8 | 5,327 | 5,169–5,643 | 2,079,864–2,080,284 |
| 0.008 | Right-only | 8 | 5,305 | 5,164–5,557 | 2,080,068–2,080,276 |

Peak Gauss-law max and RMS residuals across runs were `1.55e-11` and `1.42e-13` at the coarse step and `6.10e-13` and `6.02e-15` at the fine step. At the initial frame, the pooled RMS of collision-minus-controls `T00` was `2.91e-17` at both timesteps, compared with about `7.82e-6` for the full collision. The initial bulk energy and profile peaks are recorded per run in the index.

## Paired results

For each seed and timestep, `T_induced = T_collision − T_left-only − T_right-only` for the conventional `T00`, `T0Z`, `P_T=(TXX+TYY)/2`, and `P_L=TZZ`. The evolved profile measurement time is `t_reported + dT/2`. The comparison uses the 109 fine measurement times from `1.604` to `88.004` and linearly interpolates each coarse profile between its two bracketing saves. The fine frame at `0.804` precedes the first evolved coarse bracket. The mixed-time initial frame is compared separately at reported `t=0`. The z grids are identical, with `zeta=z−127.25` and `s=t_physical−19.25`.

The table pools the eight seeds, 109 matched times, and 512 z sites. Relative RMS divides the RMS of the paired fine-minus-coarse differences by the RMS of the interpolated coarse field. The range is the minimum and maximum of the eight per-seed relative RMS values. The final column is the RMS over time and z of the pointwise sample SD across seeds at the fine step. [Timestep summary](results/grid-b-ensemble/timestep-summary.csv) · [Per-seed metrics](results/grid-b-ensemble/seed-timestep-metrics.csv)

| Induced component | Fine RMS | RMS of difference | Relative RMS | Per-seed range | Fine seed SD, RMS over time and z |
|---|---:|---:|---:|---:|---:|
| `T00` | 1.22e-7 | 1.60e-10 | 0.131% | 0.109–0.151% | 6.14e-8 |
| `T0Z` | 9.53e-8 | 1.53e-10 | 0.161% | 0.133–0.203% | 6.08e-8 |
| `P_T` | 3.65e-8 | 2.27e-11 | 0.062% | 0.053–0.068% | 4.51e-9 |
| `P_L` | 8.86e-8 | 1.58e-10 | 0.178% | 0.153–0.224% | 6.03e-8 |

The seed spread is about 200–400 times the induced timestep difference on these RMS scales. At the frame nearest nominal crossing (`t=19.204`), the pooled induced `T00` difference was 0.649%, with a per-seed range of 0.372–1.127%. The pooled induced differences were below 0.27% for all four components at `t=24.004`, `40.004`, and `88.004`. These differences include the effect of linear interpolation between coarse saves spaced by 0.8 time units.

The full collision profiles differ by 0.062–0.170% in pooled relative RMS. Both controls have roughly 0.166% differences for `T00`, `T0Z`, and `P_L`. Control `P_T` has a fine RMS near `1.7e-13`, so the relative differences of 6.5% and 10.4% correspond to absolute differences of `1.07e-14` and `1.66e-14`. The collision and induced `P_T` curves therefore nearly overlap in the timestep plot. [Timestep differences](results/grid-b-ensemble/timestep-differences.png)

At `t=24.004`, `40.004`, and `88.004`, the fine induced `T00` RMS is about 0.97%, 1.75%, and 1.95% of the full collision RMS. At the first of these snapshots, the mean profiles show central positive `T00` and `P_T` peaks and a negative `P_L` trough. At `t=88.004`, much of the seed spread in `T00`, `T0Z`, and `P_L` is concentrated near the two outgoing peaks. [Induced profiles](results/grid-b-ensemble/induced-profiles.png)

## Files and reproduction

The review plots, manifest index, and RMS summaries are tracked under `results/grid-b-ensemble/`. The manifest paths are relative to `Workdirectory` and refer to the local `runs/` archive, which retains all 14 EMT columns, individual run logs, and `runs/grid-b-ensemble.log`.

The full derived tables below are in the Git-ignored `runs/grid-b-ensemble-analysis/` directory. They use the four conventional observables above.

| File | Content |
|---|---|
| `profile-time-audit.csv` | 5,328 saved frames with reported and physical times and alignment roles |
| `paired-raw.csv` | 909,312 rows of the two raw timestep grids, including controls and induced profiles by seed |
| `paired-aligned.csv` | 446,464 rows with coarse brackets, fine times, aligned coordinates, and fine-minus-coarse by seed |
| `initial-frame-comparison.csv` | Separate comparison at reported `t=0` |
| `ensemble-induced.csv` and `ensemble-timestep-difference.csv` | Pointwise mean, sample SD, and SEM (`SD/√8`) after pairing within each seed |

With the 48 run directories in `Workdirectory/runs/`, use Python with NumPy and Matplotlib from `Workdirectory`. Use a fresh output directory for a repeat analysis. [Analysis script](analyze-grid-b-ensemble.py)

```bash
python3 -B analyze-grid-b-ensemble.py --output runs/grid-b-ensemble-analysis-recheck
```
