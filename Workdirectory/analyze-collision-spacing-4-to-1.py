#!/usr/bin/env python3
"""Plot and compare the completed initial 4:1 collision grid pair."""

import argparse
import bisect
import csv
from html import escape
import math
import re
from pathlib import Path

try:
    import matplotlib

    matplotlib.use("Agg")
    import matplotlib.pyplot as plt
    PLOT_EXTENSION = ".png"
except ImportError:
    plt = None
    PLOT_EXTENSION = ".svg"


FIELDS = [
    ("T00", "T00"),
    ("T0Z", "T0z"),
    ("P_T", "Transverse pressure (TXX + TYY)/2"),
    ("P_L", "Longitudinal pressure TZZ"),
]
COLORS = {"Grid A": "#1565c0", "Grid B": "#d84315"}
PROFILE_NAME = re.compile(r"^EMTDynT(.+)ID(\d+)\.txt$")


def svg_number(value):
    return "{:.4g}".format(value)


def write_svg_figure(path, title, panels, x_label):
    """Write a compact, dependency-free 2x2 multi-series scientific plot."""
    width, height = 1200, 850
    panel_width, panel_height = 570, 380
    left_margin, top_margin = 30, 62
    parts = [
        '<svg xmlns="http://www.w3.org/2000/svg" width="{}" height="{}" viewBox="0 0 {} {}">'.format(width, height, width, height),
        '<rect width="100%" height="100%" fill="white"/>',
        '<text x="{}" y="34" text-anchor="middle" font-family="sans-serif" font-size="19" font-weight="bold">{}</text>'.format(width / 2, escape(title)),
    ]

    for panel_index, panel in enumerate(panels):
        col, row = panel_index % 2, panel_index // 2
        panel_x = left_margin + col * panel_width
        panel_y = top_margin + row * panel_height
        plot_x = panel_x + 76
        plot_y = panel_y + 18
        plot_width = 450
        plot_height = 275
        all_x = [x for series in panel["series"] for x in series["x"]]
        all_y = [y for series in panel["series"] for y in series["y"]]
        if not all_x or not all_y:
            continue
        x_min, x_max = min(all_x), max(all_x)
        if x_min == x_max:
            x_min, x_max = x_min - 1.0, x_max + 1.0
        y_min, y_max = min(all_y), max(all_y)
        y_span = y_max - y_min
        if y_span == 0:
            y_span = max(abs(y_max), 1e-12)
        y_min -= y_span * 0.08
        y_max += y_span * 0.08

        parts.append('<text x="{}" y="{}" font-family="sans-serif" font-size="14" font-weight="bold">{}</text>'.format(
            plot_x, panel_y + 13, escape(panel["ylabel"])))
        for tick in range(5):
            fraction = tick / 4.0
            x = plot_x + fraction * plot_width
            y = plot_y + plot_height - fraction * plot_height
            x_value = x_min + fraction * (x_max - x_min)
            y_value = y_min + fraction * (y_max - y_min)
            parts.append('<line x1="{:.2f}" y1="{:.2f}" x2="{:.2f}" y2="{:.2f}" stroke="#dddddd" stroke-width="1"/>'.format(
                plot_x, y, plot_x + plot_width, y))
            parts.append('<line x1="{:.2f}" y1="{:.2f}" x2="{:.2f}" y2="{:.2f}" stroke="#eeeeee" stroke-width="1"/>'.format(
                x, plot_y, x, plot_y + plot_height))
            parts.append('<text x="{:.2f}" y="{:.2f}" text-anchor="middle" font-family="sans-serif" font-size="10">{}</text>'.format(
                x, plot_y + plot_height + 17, escape(svg_number(x_value))))
            parts.append('<text x="{:.2f}" y="{:.2f}" text-anchor="end" font-family="sans-serif" font-size="10">{}</text>'.format(
                plot_x - 8, y + 3, escape(svg_number(y_value))))

        parts.append('<line x1="{:.2f}" y1="{:.2f}" x2="{:.2f}" y2="{:.2f}" stroke="#444444" stroke-width="1.1"/>'.format(
            plot_x, plot_y, plot_x, plot_y + plot_height))
        parts.append('<line x1="{:.2f}" y1="{:.2f}" x2="{:.2f}" y2="{:.2f}" stroke="#444444" stroke-width="1.1"/>'.format(
            plot_x, plot_y + plot_height, plot_x + plot_width, plot_y + plot_height))
        if y_min <= 0.0 <= y_max:
            zero_y = plot_y + plot_height - (0.0 - y_min) * plot_height / (y_max - y_min)
            parts.append('<line x1="{:.2f}" y1="{:.2f}" x2="{:.2f}" y2="{:.2f}" stroke="#888888" stroke-width="0.8"/>'.format(
                plot_x, zero_y, plot_x + plot_width, zero_y))
        if panel.get("reference_x") is not None and x_min <= panel["reference_x"] <= x_max:
            reference_x = plot_x + (panel["reference_x"] - x_min) * plot_width / (x_max - x_min)
            parts.append('<line x1="{:.2f}" y1="{:.2f}" x2="{:.2f}" y2="{:.2f}" stroke="#555555" stroke-width="0.9" stroke-dasharray="4,4"/>'.format(
                reference_x, plot_y, reference_x, plot_y + plot_height))
        parts.append('<text x="{:.2f}" y="{:.2f}" text-anchor="middle" font-family="sans-serif" font-size="11">{}</text>'.format(
            plot_x + plot_width / 2, plot_y + plot_height + 36, escape(x_label)))

        for series_index, series in enumerate(panel["series"]):
            points = []
            for x_value, y_value in zip(series["x"], series["y"]):
                x = plot_x + (x_value - x_min) * plot_width / (x_max - x_min)
                y = plot_y + plot_height - (y_value - y_min) * plot_height / (y_max - y_min)
                points.append("{:.2f},{:.2f}".format(x, y))
            parts.append('<polyline fill="none" stroke="{}" stroke-width="1.7" points="{}"/>'.format(
                series["color"], " ".join(points)))
            legend_y = plot_y + 13 + series_index * 16
            legend_x = plot_x + plot_width - 118
            parts.append('<line x1="{:.2f}" y1="{:.2f}" x2="{:.2f}" y2="{:.2f}" stroke="{}" stroke-width="2"/>'.format(
                legend_x, legend_y - 3, legend_x + 18, legend_y - 3, series["color"]))
            parts.append('<text x="{:.2f}" y="{:.2f}" font-family="sans-serif" font-size="10">{}</text>'.format(
                legend_x + 23, legend_y, escape(series["label"])))

    parts.append("</svg>")
    path.write_text("\n".join(parts), encoding="utf-8")


def read_manifest(run_dir):
    values = {}
    with (run_dir / "MANIFEST.txt").open("r", encoding="utf-8") as stream:
        for line in stream:
            if "=" in line:
                key, value = line.rstrip("\n").split("=", 1)
                values[key] = value
    return values


def finite_float(value, context):
    result = float(value)
    if not math.isfinite(result):
        raise ValueError("Non-finite value in {}: {}".format(context, value))
    return result


def read_profiles(run_dir, manifest):
    seed = int(manifest["seed"])
    nz = int(manifest["Nx_Ny_Nz"].split()[2])
    az = float(manifest["ax_ay_az"].split()[2])
    z_center = float(manifest["aligned_center_z_c"])
    result = {}

    for path in run_dir.glob("EMTDynT*ID{}.txt".format(seed)):
        match = PROFILE_NAME.match(path.name)
        if not match:
            continue
        reported_time = finite_float(match.group(1), path.name)
        z_values = []
        aligned_z = []
        fields = {name: [] for name, _ in FIELDS}

        with path.open("r", encoding="utf-8") as stream:
            for row_number, line in enumerate(stream, 1):
                if not line.strip():
                    continue
                values = line.split()
                if len(values) != 14:
                    raise ValueError("{} row {} has {} columns, expected 14".format(path, row_number, len(values)))
                values = [finite_float(value, "{} row {}".format(path.name, row_number)) for value in values]
                z = values[0] * az
                z_values.append(z)
                aligned_z.append(z - z_center)
                fields["T00"].append(values[1])
                fields["T0Z"].append(values[7])
                fields["P_T"].append(0.5 * (values[2] + values[3]))
                fields["P_L"].append(values[4])

        if len(z_values) != nz:
            raise ValueError("{} has {} rows, expected {}".format(path, len(z_values), nz))
        if z_values != sorted(z_values):
            raise ValueError("{} has non-monotone z coordinates".format(path))
        result[reported_time] = {"z": z_values, "zeta": aligned_z, "fields": fields}

    if not result:
        raise ValueError("No EMT profiles found in {}".format(run_dir))
    return result


def read_gauss_history(run_dir, seed):
    path = run_dir / "GausCheck{}.txt".format(seed)
    rows = []
    with path.open("r", encoding="utf-8") as stream:
        for row_number, line in enumerate(stream, 1):
            if not line.strip():
                continue
            values = line.split()
            if len(values) != 3:
                raise ValueError("{} row {} has {} columns, expected 3".format(path, row_number, len(values)))
            rows.append(tuple(finite_float(value, "{} row {}".format(path.name, row_number)) for value in values))
    if not rows:
        raise ValueError("No Gauss-law history in {}".format(path))
    return rows


def interpolate(xs, ys, x):
    index = bisect.bisect_left(xs, x)
    if index < len(xs) and abs(xs[index] - x) < 1e-12:
        return ys[index]
    if index == 0 or index == len(xs):
        raise ValueError("Requested interpolation would extrapolate at x={}".format(x))
    x0, x1 = xs[index - 1], xs[index]
    y0, y1 = ys[index - 1], ys[index]
    fraction = (x - x0) / (x1 - x0)
    return y0 + fraction * (y1 - y0)


def verify_pair(manifest_a, manifest_b, profiles_a, profiles_b):
    expected = {
        "plan": "collision-spacing-4-to-1",
        "procedure": "Simulation.cpp",
        "stage": "main",
        "seed": "1",
        "dT": "0.08",
        "endpoint_from_initialization": "88",
        "run_status": "completed",
        "completion_marker": "yes",
        "exit_status": "0",
    }
    for label, manifest in (("Grid A", manifest_a), ("Grid B", manifest_b)):
        for key, value in expected.items():
            if manifest.get(key) != value:
                raise ValueError("{} manifest has {}={}, expected {}".format(label, key, manifest.get(key), value))
    if manifest_a["grid"] != "Grid A" or manifest_b["grid"] != "Grid B":
        raise ValueError("Grid manifests do not match the requested pair")
    if sorted(profiles_a) != sorted(profiles_b):
        raise ValueError("The two grids do not have matching reported profile times")


def plot_profiles(output_dir, profiles_a, profiles_b, t_coll_a, t_coll_b, z_center_a, z_center_b):
    selected = [time for time in (0.0, 16.0, 24.0, 40.0, 88.0) if time in profiles_a]
    for time in selected:
        for coordinates, aligned in (("raw", False), ("aligned", True)):
            panels = []
            for field_name, field_label in FIELDS:
                series = []
                for label, profile, color in (
                    ("Grid A", profiles_a[time], COLORS["Grid A"]),
                    ("Grid B", profiles_b[time], COLORS["Grid B"]),
                ):
                    x_values = profile["zeta"] if aligned else profile["z"]
                    series.append({"label": label, "x": x_values,
                                   "y": profile["fields"][field_name], "color": color})
                panels.append({"ylabel": field_label, "series": series})
            if aligned:
                s_a = time - t_coll_a
                s_b = time - t_coll_b
                title = "Aligned profiles at reported t={:g}; s_A={:g}, s_B={:g}".format(time, s_a, s_b)
                filename = "profiles-aligned-t{:03g}{}".format(time, PLOT_EXTENSION)
            else:
                title = "Raw profiles at reported t={:g}".format(time)
                filename = "profiles-raw-t{:03g}{}".format(time, PLOT_EXTENSION)
            x_label = "zeta = z - z_c (simulation units)" if aligned else "z (simulation units)"
            if plt is None:
                write_svg_figure(output_dir / filename, title, panels, x_label)
            else:
                figure, axes = plt.subplots(2, 2, figsize=(11, 8), sharex=True)
                axes = axes.ravel()
                for axis, panel in zip(axes, panels):
                    for series in panel["series"]:
                        axis.plot(series["x"], series["y"], label=series["label"],
                                  color=series["color"], linewidth=1.25)
                    axis.axhline(0.0, color="#777777", linewidth=0.6, alpha=0.5)
                    axis.grid(True, alpha=0.22)
                    axis.set_ylabel(panel["ylabel"])
                    axis.legend(frameon=False, fontsize=8)
                for axis in axes[2:]:
                    axis.set_xlabel(x_label)
                figure.suptitle(title)
                figure.tight_layout(rect=(0, 0, 1, 0.95))
                figure.savefig(str(output_dir / filename), dpi=170)
                plt.close(figure)


def write_resolution_differences(output_dir, profiles_a, profiles_b, t_coll_a, t_coll_b):
    csv_path = output_dir / "aligned-profile-differences.csv"
    with csv_path.open("w", newline="", encoding="utf-8") as stream:
        writer = csv.writer(stream)
        writer.writerow(["reported_time", "aligned_s_grid_a", "aligned_s_grid_b", "zeta", "field", "Grid_A", "Grid_B_interpolated", "Grid_B_minus_Grid_A"])
        for time in sorted(profiles_a):
            profile_a, profile_b = profiles_a[time], profiles_b[time]
            for field_name, _ in FIELDS:
                for zeta_a, value_a in zip(profile_a["zeta"], profile_a["fields"][field_name]):
                    if zeta_a < profile_b["zeta"][0] or zeta_a > profile_b["zeta"][-1]:
                        continue
                    value_b = interpolate(profile_b["zeta"], profile_b["fields"][field_name], zeta_a)
                    writer.writerow([time, "{:.12g}".format(time - t_coll_a),
                                     "{:.12g}".format(time - t_coll_b), "{:.12g}".format(zeta_a), field_name,
                                     "{:.12g}".format(value_a), "{:.12g}".format(value_b),
                                     "{:.12g}".format(value_b - value_a)])

    selected = [time for time in (24.0, 40.0, 88.0) if time in profiles_a]
    panels = []
    time_colors = {24.0: "#2e7d32", 40.0: "#6a1b9a", 88.0: "#ef6c00"}
    for field_name, field_label in FIELDS:
        series = []
        for time in selected:
            profile_a, profile_b = profiles_a[time], profiles_b[time]
            common_zeta = [z for z in profile_a["zeta"] if profile_b["zeta"][0] <= z <= profile_b["zeta"][-1]]
            values_a = profile_a["fields"][field_name]
            values_b = profile_b["fields"][field_name]
            differences = [
                interpolate(profile_b["zeta"], values_b, z) - interpolate(profile_a["zeta"], values_a, z)
                for z in common_zeta
            ]
            series.append({"label": "t={:g}".format(time), "x": common_zeta,
                           "y": differences, "color": time_colors[time]})
        panels.append({"ylabel": "Grid B - Grid A: {}".format(field_label), "series": series, "reference_y": 0.0})

    output_path = output_dir / ("aligned-resolution-differences" + PLOT_EXTENSION)
    title = "Aligned profile differences (one-seed resolution and sampling sensitivity)"
    x_label = "zeta = z - z_c (simulation units)"
    if plt is None:
        write_svg_figure(output_path, title, panels, x_label)
    else:
        figure, axes = plt.subplots(2, 2, figsize=(11, 8), sharex=True)
        axes = axes.ravel()
        for axis, panel in zip(axes, panels):
            for series in panel["series"]:
                axis.plot(series["x"], series["y"], label=series["label"],
                          color=series["color"], linewidth=1.15)
            axis.axhline(0.0, color="#555555", linewidth=0.7)
            axis.grid(True, alpha=0.22)
            axis.set_ylabel(panel["ylabel"])
            axis.legend(frameon=False, fontsize=8)
        for axis in axes[2:]:
            axis.set_xlabel(x_label)
        figure.suptitle(title)
        figure.tight_layout(rect=(0, 0, 1, 0.95))
        figure.savefig(str(output_path), dpi=170)
        plt.close(figure)


def plot_crossing_plane_histories(output_dir, profiles_a, profiles_b, t_coll_a, t_coll_b):
    panels = []
    for field_name, field_label in FIELDS:
        series = []
        for label, profiles, t_coll, color, style in (
            ("Grid A", profiles_a, t_coll_a, COLORS["Grid A"], "o-"),
            ("Grid B", profiles_b, t_coll_b, COLORS["Grid B"], "s-"),
        ):
            aligned_times = []
            values = []
            for reported_time in sorted(profiles):
                profile = profiles[reported_time]
                value = interpolate(profile["zeta"], profile["fields"][field_name], 0.0)
                aligned_times.append(reported_time - t_coll)
                values.append(value)
            series.append({"label": label, "x": aligned_times, "y": values, "color": color})
        panels.append({"ylabel": field_label + " at zeta=0", "series": series, "reference_x": 0.0, "reference_y": 0.0})

    output_path = output_dir / ("crossing-plane-histories" + PLOT_EXTENSION)
    title = "Crossing-plane histories using longitudinal linear interpolation"
    x_label = "s = t - t_coll (reported simulation time)"
    if plt is None:
        write_svg_figure(output_path, title, panels, x_label)
    else:
        figure, axes = plt.subplots(2, 2, figsize=(10, 7), sharex=True)
        axes = axes.ravel()
        for axis, panel in zip(axes, panels):
            for series in panel["series"]:
                axis.plot(series["x"], series["y"], color=series["color"],
                          linewidth=1.15, markersize=3.2, marker="o", label=series["label"])
            axis.axvline(0.0, color="#555555", linewidth=0.7, linestyle="--")
            axis.axhline(0.0, color="#777777", linewidth=0.55, alpha=0.5)
            axis.grid(True, alpha=0.22)
            axis.set_ylabel(panel["ylabel"])
            axis.legend(frameon=False, fontsize=8)
        for axis in axes[2:]:
            axis.set_xlabel(x_label)
        figure.suptitle(title)
        figure.tight_layout(rect=(0, 0, 1, 0.95))
        figure.savefig(str(output_path), dpi=170)
        plt.close(figure)


def diagnostic_summary(run_dir, manifest):
    seed = int(manifest["seed"])
    gauss = read_gauss_history(run_dir, seed)
    max_row = max(gauss, key=lambda row: row[1])
    rms_row = max(gauss, key=lambda row: row[2])
    peak_memory = manifest.get("peak_rss_kb", "not-recorded")
    memory_gib = "not-recorded"
    if peak_memory not in ("not-recorded", ""):
        memory_gib = "{:.3f}".format(float(peak_memory) / (1024.0 * 1024.0))
    return [
        "- Run: `{}`".format(run_dir.name),
        "- Grid: {}; N={} ; a={}".format(manifest["grid"], manifest["Nx_Ny_Nz"], manifest["ax_ay_az"]),
        "- Seed {}, dT={}, nominal endpoint {}, actual endpoint {}, actual steps {}.".format(
            manifest["seed"], manifest["dT"], manifest["endpoint_from_initialization"],
            manifest.get("actual_endpoint", "not-recorded"), manifest.get("actual_steps", "not-recorded")),
        "- Wall time {} s; peak RSS {} KiB ({} GiB).".format(
            manifest.get("elapsed_seconds", "not-recorded"), peak_memory, memory_gib),
        "- Gauss max residual {:.6g} at reported t={:g}; max RMS {:.6g} at reported t={:g}.".format(
            max_row[1], max_row[0], rms_row[2], rms_row[0]),
    ]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--grid-a", required=True, type=Path)
    parser.add_argument("--grid-b", required=True, type=Path)
    parser.add_argument("--output", required=True, type=Path)
    args = parser.parse_args()

    run_a = args.grid_a.resolve()
    run_b = args.grid_b.resolve()
    output_dir = args.output.resolve()
    manifest_a, manifest_b = read_manifest(run_a), read_manifest(run_b)
    profiles_a, profiles_b = read_profiles(run_a, manifest_a), read_profiles(run_b, manifest_b)
    verify_pair(manifest_a, manifest_b, profiles_a, profiles_b)

    t_coll_a = float(manifest_a["nominal_collision_time"])
    t_coll_b = float(manifest_b["nominal_collision_time"])
    z_center_a = float(manifest_a["aligned_center_z_c"])
    z_center_b = float(manifest_b["aligned_center_z_c"])
    output_dir.mkdir(parents=True, exist_ok=True)

    plot_profiles(output_dir, profiles_a, profiles_b, t_coll_a, t_coll_b, z_center_a, z_center_b)
    write_resolution_differences(output_dir, profiles_a, profiles_b, t_coll_a, t_coll_b)
    plot_crossing_plane_histories(output_dir, profiles_a, profiles_b, t_coll_a, t_coll_b)

    summary = [
        "# 4:1 collision initial-grid comparison",
        "",
        "Both runs use dT=0.08, seed 1, and the reported EMT profile times. The profiles use the first EMT definitions: T00, T0Z, (TXX+TYY)/2, and TZZ.",
        "",
        "Grid A uses z_c={} and t_coll={}; Grid B uses z_c={} and t_coll={}. Aligned plots use ζ=z−z_c. Each profile difference compares the same reported t; its s_A and s_B values remain separate in the CSV. No time interpolation across the eight-unit profile interval is used. Spatial comparisons linearly sample Grid B at Grid A's aligned coordinates, with no extrapolation. Crossing-plane histories linearly interpolate each profile to ζ=0 and plot each grid at its own s=t−t_coll.".format(
            z_center_a, t_coll_a, z_center_b, t_coll_b),
        "",
        "The cross-grid differences combine longitudinal/transverse resolution changes with different transverse random realizations. The shared seed and legacy draw order do not match physical realizations when Nx and Ny differ. Treat the differences as one-seed sensitivity, not as a convergence estimate.",
        "",
        "The optional Grid B dT=0.008 run is not included. These profiles share the same reported dT and cadence; no time-step difference is computed here.",
        "",
        "Plot backend: {}.".format("Matplotlib PNG" if plt is not None else "dependency-free SVG (Matplotlib is unavailable in WSL)"),
        "",
        "## Run and diagnostic summary",
        "",
    ]
    summary.extend(diagnostic_summary(run_a, manifest_a))
    summary.extend(diagnostic_summary(run_b, manifest_b))
    summary.extend([
        "",
        "## Files",
        "",
        "- `profiles-raw-t*{}` and `profiles-aligned-t*{}`: T00, T0z, transverse pressure, and longitudinal pressure at selected times.".format(PLOT_EXTENSION, PLOT_EXTENSION),
        "- `crossing-plane-histories{}`: the same fields linearly interpolated at the nominal crossing plane.".format(PLOT_EXTENSION),
        "- `aligned-resolution-differences{}` and `aligned-profile-differences.csv`: Grid B minus Grid A after longitudinal alignment and sampling on the Grid A coordinates.".format(PLOT_EXTENSION),
    ])
    with (output_dir / "SUMMARY.md").open("w", encoding="utf-8") as stream:
        stream.write("\n".join(summary) + "\n")

    print("Analysis written to {}".format(output_dir))
    with (output_dir / "aligned-profile-differences.csv").open("r", encoding="utf-8") as stream:
        difference_rows = sum(1 for _ in stream) - 1
    print("Compared {} matched profile times; wrote {} aligned difference rows.".format(
        len(profiles_a), difference_rows))


if __name__ == "__main__":
    main()
