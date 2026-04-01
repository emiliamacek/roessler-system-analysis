import numpy as np
import matplotlib.pyplot as plt

a_values = [-2, -1, 0, 0.2, 1, 2]

x_vals = np.linspace(-4, 4, 20)
y_vals = np.linspace(-4, 4, 20)
X, Y = np.meshgrid(x_vals, y_vals)

fig, axes = plt.subplots(2, 3, figsize=(12, 10))
axes = axes.flatten()

for i, a in enumerate(a_values):
    U = (-1) * Y
    V = X + a * Y
    speed = np.sqrt(U**2 + V**2)

    ax = axes[i]
    strm = ax.streamplot(
        X, Y, U, V, color=speed, cmap="plasma", density=0.8, linewidth=1
    )
    ax.set_title(f"a = {a}")
    ax.set_xlabel("x")
    ax.set_ylabel("y")
    ax.grid(True)
    ax.set_aspect("equal")

plt.tight_layout()
plt.savefig("../images/phase-portraits-xy", bbox_inches="tight", dpi=500)
