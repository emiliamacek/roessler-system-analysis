import matplotlib.pyplot as plt
import numpy as np
from matplotlib.ticker import FormatStrFormatter, MaxNLocator
from mpl_toolkits.mplot3d import Axes3D
from scipy.integrate import solve_ivp

a = 0.2
b = 0.2
c = 5.25


def rossler(t, state):
    x, y, z = state
    return [-y - z, x + a * y, b + z * (x - c)]


total_time = 5000
dt = 0.001
t_span = np.arange(0, total_time, dt)

transient_time = total_time / 2

transient_idx = int(transient_time / dt)

initial_state = [0.1, 0, 0]
sol = solve_ivp(rossler, [t_span[0], t_span[-1]], initial_state, t_eval=t_span)

t = sol.t[transient_idx:]
x, y, z = (
    sol.y[0, transient_idx:],
    sol.y[1, transient_idx:],
    sol.y[2, transient_idx:],
)

# --- sekcja Poincare'go ---
x_section = []
y_section = []
z_section = []

for i in range(1, len(x)):
    if (x[i - 1] < 0 and x[i] >= 0) or (x[i - 1] > 0 and x[i] <= 0):
        if y[i] < 0:
            x_section.append(x[i])
            y_section.append(y[i])
            z_section.append(z[i])

fig = plt.figure(figsize=(10, 8))

ax = fig.add_subplot(111, projection="3d")

# attractor
ax.plot(x, y, z, color="gray", linewidth=0.5, alpha=0.6)

# points which intersect with the section
ax.scatter(
    x_section,
    y_section,
    z_section,
    color="red",
    s=20,
)

y_plane = np.linspace(min(y) - 3, max(y), 100)
z_plane = np.linspace(-3, 4, 100)
Y_plane, Z_plane = np.meshgrid(y_plane, z_plane)
X_plane = np.zeros_like(Y_plane)

mask = Y_plane >= 0
X_plane_masked = np.copy(X_plane)
Y_plane_masked = np.copy(Y_plane)
Z_plane_masked = np.copy(Z_plane)

X_plane_masked[mask] = np.nan
Y_plane_masked[mask] = np.nan
Z_plane_masked[mask] = np.nan

ax.plot_surface(
    X_plane_masked,
    Y_plane_masked,
    Z_plane_masked,
    alpha=0.1,
    color="blue",
    rstride=100,
    cstride=100,
    label=r"Poincaré section $\Pi$",
)

# ax.plot([], [], [], color="blue", alpha=0.3, label=r"Sekcja $\Pi$")
ax.legend()
ax.set_xlabel("x")
ax.set_ylabel("y")
ax.set_zlabel("z")

ax.view_init(elev=25, azim=250)

ax.xaxis.set_major_formatter(FormatStrFormatter("%.0f"))
ax.yaxis.set_major_formatter(FormatStrFormatter("%.0f"))
ax.zaxis.set_major_formatter(FormatStrFormatter("%.0f"))
ax.xaxis.set_major_locator(MaxNLocator(5))
ax.yaxis.set_major_locator(MaxNLocator(5))
ax.zaxis.set_major_locator(MaxNLocator(5))

# ax.set_zlim(-4, 20)
# ax.set_ylim(-15, 10)
plt.tight_layout()
plt.savefig("../images/section_with_points.png", bbox_inches="tight", dpi=500)
# plt.show()
