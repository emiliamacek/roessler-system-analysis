import math

import matplotlib.pyplot as plt
import numpy as np
from matplotlib.ticker import FormatStrFormatter, MaxNLocator
from mpl_toolkits.mplot3d import Axes3D
from scipy.integrate import solve_ivp

configs = {
    "a": {
        "fixed": {"b": 0.2, "c": 5.7},
        "values": [-0.2, 0.1, 0.2, 0.25, 0.3, 0.38],
        "standard_val": 0.2,
    },
    "b": {
        "fixed": {"a": 0.2, "c": 5.7},
        "values": [0.000002, 0.0002, 0.2, 0.5, 3, 5],
        "standard_val": 0.2,
    },
    "c": {
        "fixed": {"a": 0.2, "b": 0.2},
        "values": [1, 2.7, 3.5, 5, 5.3, 5.7, 5.9, 5.99, 6],
        "standard_val": 5.7,
    },
}

print("==========================================================")
print("|| 1. Vary parameter 'a' (b=0.2, c=5.7)")
print("|| 2. Vary parameter 'b' (a=0.2, c=5.7)")
print("|| 3. Vary parameter 'c' (a=0.2, b=0.2)")
print("==========================================================")
choice = input("Your choice (1/2/3): ")

if choice == "1":
    target_param = "a"
elif choice == "2":
    target_param = "b"
elif choice == "3":
    target_param = "c"
else:
    print("Invalid choice. Defaulting to 'c'.")
    target_param = "c"

# Extract the chosen setup
cfg = configs[target_param]
fixed_params = cfg["fixed"]
values = cfg["values"]
standard_val = cfg["standard_val"]

print(f"\n[~] Generating data for changing '{target_param}'... Please wait.")

total_time = 5000
dt = 0.001
t = np.arange(0, total_time, dt)
transient_time = total_time / 2
transient_idx = int(transient_time / dt)


# accept all three parameters explicitly
def rossler(t, X, a, b, c):
    x, y, z = X
    return [-y - z, x + a * y, b + z * (x - c)]


all_sols = []
for val in values:
    # build current parameter set
    curr = fixed_params.copy()
    curr[target_param] = val

    sol = solve_ivp(
        rossler,
        [t[0], t[-1]],
        [0.1, 0.1, 0],
        args=(curr.get("a", val), curr.get("b", val), curr.get("c", val)),
        t_eval=t,
    )
    all_sols.append(sol)

# ==========================================
# calculate grid size dynamically (e.g., 6 values -> 2 rows, 9 values -> 3 rows)
cols = 3
rows = math.ceil(len(values) / cols)

fig, axs = plt.subplots(
    rows, cols, figsize=(14, 4 * rows), subplot_kw={"projection": "3d"}
)
# ensure axs is always a flat list even if it's 1D
axs_flat = axs.flatten() if hasattr(axs, "flatten") else [axs]

for i, (val, sol) in enumerate(zip(values, all_sols)):
    ax = axs_flat[i]

    # reconstruct current parameters for math logic
    curr = fixed_params.copy()
    curr[target_param] = val
    a_curr = curr.get("a", val)
    b_curr = curr.get("b", val)
    c_curr = curr.get("c", val)

    # reject transient points
    x, y, z = sol.y
    x, y, z = x[transient_idx:], y[transient_idx:], z[transient_idx:]

    # change color if it's the "standard" classical value
    col = "gray" if abs(val - standard_val) < 1e-6 else "tab:blue"
    ax.plot(x, y, z, lw=0.1, color=col)

    # calculate P2 fixed point
    D = c_curr**2 - 4 * a_curr * b_curr
    if D >= 0:
        sqrtD = np.sqrt(D)
        x0 = (c_curr - sqrtD) / 2
        y0 = (-c_curr + sqrtD) / (2 * a_curr)
        z0 = (c_curr - sqrtD) / (2 * a_curr)
        ax.scatter(x0, y0, z0, c="red", marker=".", s=100, label=r"punkt stały $P_{2}$")

    ax.set_title(rf"${target_param} = {val}$")
    ax.set_box_aspect((1, 1, 1))
    ax.set_xlabel("x")
    ax.set_ylabel("y")
    ax.set_zlabel("z")
    ax.view_init(elev=25, azim=250)

    ax.xaxis.set_major_locator(MaxNLocator(4))
    ax.yaxis.set_major_locator(MaxNLocator(4))
    ax.zaxis.set_major_locator(MaxNLocator(4))
    if D >= 0:
        ax.legend()

# hide any extra empty subplots
for j in range(len(values), len(axs_flat)):
    fig.delaxes(axs_flat[j])

plt.tight_layout()

out_file = f"../images/change-of-{target_param}-param.png"
plt.savefig(out_file, bbox_inches="tight", dpi=500)
print(f"[+] Plot successfully saved to {out_file}!\n")
