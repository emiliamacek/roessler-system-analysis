import sys

import plotter

CONFIGS = {
    "first": {
        "lower_bound ": 4.380026185035703,
        "upper_bound ": 4.39562124248497,
        "file_path ": "../images/fpw-le.png",
        "path_x ": "../output/fpw-le-a.csv",
        "path_y ": "../output/fpw-le.csv",
    },
    "second": {
        "lower_bound ": 4.694104184150694,
        "upper_bound ": 4.720761523046092,
        "file_path ": "../images/spw-le.png",
        "path_x ": "../output/sec_pw-le-a.csv",
        "path_y ": "../output/sec_pw-le.csv",
    },
    "third": {
        "lower_bound ": 5.185404746055601,
        "upper_bound ": 5.497374749498998,
        "file_path ": "../images/tpw-le-1.png",
        "path_x ": "../output/tpw-le-a.csv",
        "path_y ": "../output/tpw-le.csv",
    },
}

choice = (
    input("Which dataset would you like to plot? (first / second / third) window: ")
    .strip()
    .lower()
)

if choice not in CONFIGS:
    print(f"Invalid choice '{choice}'. Please restart and select a valid option.")
    sys.exit(1)

config = CONFIGS[choice]
file_path = config["file_path"]
path_x = config["path_x"]
path_y = config["path_y"]

lower_bound = config.get("lower_bound")
upper_bound = config.get("upper_bound")


def draw_zero_line(ax):
    ymin, ymax = ax.get_ylim()
    text_y = ymax + 0.02 * (ymax - ymin)  # 2% nad górną granicą

    ax.axhline(0, color="red", linewidth=0.5, linestyle="-")
    ax.axvline(lower_bound, color="blue", linewidth=0.2, linestyle="--")
    ax.text(
        lower_bound,
        text_y,
        f"{lower_bound:.4f}",
        rotation=90,
        verticalalignment="bottom",
        horizontalalignment="center",
        fontsize=4,
        color="blue",
    )

    ax.axvline(upper_bound, color="green", linewidth=0.2, linestyle="--")
    ax.text(
        upper_bound,
        text_y,
        f"{upper_bound:.4f}",
        rotation=90,
        verticalalignment="bottom",
        horizontalalignment="center",
        fontsize=4,
        color="green",
    )


x_label = "c"
y_label = "Lyapunov exponent"
alpha_ = 1
DPI = 350
major_ticker = 0.05
minor_ticker = 0.005
format = "%.1f"
markerformat = ","

plotter.plot(
    file_path,
    path_x,
    path_y,
    x_label,
    y_label,
    alpha_,
    DPI,
    major_ticker,
    minor_ticker,
    format,
    markerformat,
    custom_draw=draw_zero_line,
    ylim=[-0.5, 0.15],
    xlim=[4.378, 4.398],
)
