import numpy as np
import matplotlib.pyplot as plt
from scipy.integrate import solve_ivp
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.ticker import MaxNLocator, FormatStrFormatter

a = 0.2
b = 0.2
c = 5.7


def rossler(t, state):
    x, y, z = state
    return [-y - z, x + a * y, b + z * (x - c)]


total_time = 1000
dt = 0.001
t_span = np.arange(0, total_time, dt)

transient_time = 500

transient_idx = int(transient_time / dt)
initial_state = [0.1, 0, 0]
sol = solve_ivp(rossler, [t_span[0], t_span[-1]], initial_state, t_eval=t_span)


t = sol.t[transient_idx:]
x, y, z = (
    sol.y[0, transient_idx:],
    sol.y[1, transient_idx:],
    sol.y[2, transient_idx:],
)

P1 = np.array([5.692974, -28.464869, 28.464869])
P2 = np.array([0.007026, -0.035131, 0.035131])

v1_P1 = np.array([0.004965, -0.707577, 0.706619])
v2_re_P1 = np.array([-0.000242, -0.034440, -0.981716])
v2_im_P1 = np.array([-0.187205, 0.00131, 0])
v3_im_P1 = np.array([0.187205, -0.00131, 0])

v1_P2 = np.array([0.168236, -0.028578, 0.985332])
v2_re_P2 = np.array([0.707280, -0.072775, 0.004168])
v2_im_P2 = np.array([0, -0.703164, -0.000716])
v3_im_P2 = np.array([0, 0.703164, 0.000716])


fig, axs = plt.subplots(1, 2, figsize=(14, 6), subplot_kw={"projection": "3d"})


def plot_attractor_with_vectors_ax(num, ax, fixed_point, vectors, title):
    ax.plot(x, y, z, color="gray", linewidth=0.5, alpha=0.6)

    labels = [
        r"$v_{3}$",
        r"$Re(v_{1}) = Re(v_{2})$",
        r"$Im(v_{1})$",
        r"$Im(v_{2})$",
    ]
    for v, c, lbl in zip(vectors, ("r", "g", "b", "c"), labels):
        ax.quiver(*fixed_point, *v, color=c, length=10, normalize=True, label=lbl)

    ax.plot(
        fixed_point[0],
        fixed_point[1],
        fixed_point[2],
        color="black",
        marker="o",
    )
    if num == "1":
        ax.text(
            fixed_point[0],
            fixed_point[1],
            fixed_point[2] + 1,
            r"$P_{}$".format(num),
            fontsize=12,
        )
    else:
        ax.text(
            fixed_point[0] - 1.8,
            fixed_point[1],
            fixed_point[2],
            r"$P_{}$".format(num),
            fontsize=12,
        )
    ax.set_title(title)
    ax.set_xlabel("x")
    ax.set_ylabel("y")
    ax.set_zlabel("z")

    ax.xaxis.set_major_formatter(FormatStrFormatter("%.0f"))
    ax.yaxis.set_major_formatter(FormatStrFormatter("%.0f"))
    ax.zaxis.set_major_formatter(FormatStrFormatter("%.0f"))

    ax.xaxis.set_major_locator(MaxNLocator(5))
    ax.yaxis.set_major_locator(MaxNLocator(5))
    ax.zaxis.set_major_locator(MaxNLocator(5))

    ax.legend()


plot_attractor_with_vectors_ax(
    "1",
    axs[0],
    P1,
    [v1_P1, v2_re_P1, v2_im_P1, v3_im_P1],
    r"Rössler attractor with $P_{1}$'s eigen vectors",
)
plot_attractor_with_vectors_ax(
    "2",
    axs[1],
    P2,
    [v1_P2, v2_re_P2, v2_im_P2, v3_im_P2],
    r"Rössler attractor with $P_{2}$'s eigen vectors",
)

plt.tight_layout()
plt.savefig("../images/eigen-vectors.png", bbox_inches="tight", dpi=500)
