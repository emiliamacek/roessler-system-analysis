import numpy as np
from sympy import symbols, solve, Eq, simplify
from math import sqrt


def fixed_points(a, b, c):
    delta = c**2 - 4 * a * b
    sqrt_delta = np.sqrt(delta)

    point1 = (
        (c + sqrt_delta) / (2 * a),
        (-c - sqrt_delta) / (2 * a),
        (c + sqrt_delta) / (2 * a),
    )
    point2 = (
        (c - sqrt_delta) / (2 * a),
        (-c + sqrt_delta) / (2 * a),
        (c - sqrt_delta) / (2 * a),
    )

    return point1, point2


def eigen_data_at_point(x, y, z, a, c):
    J = np.array([[0, -1, -1], [1, a, 0], [z, 0, x - c]])

    values, vectors = np.linalg.eig(J)
    return values, vectors


def rossler_fixed_points(a: float, b: float, c: float):
    """
    0 = -y - z
    0 = x + a*y
    0 = b + z*(x - c)
    """

    # z = -y  => y = -z
    # x = -a*y => x = a*z
    # z*(x - c) = -b => z*(a*z - c) = -b

    # a*z^2 - c*z + b = 0
    A = a
    B = -c
    C = b

    D = B**2 - 4 * A * C  # delta

    if D < 0:
        return []

    z1 = (-B + sqrt(D)) / (2 * A)
    z2 = (-B - sqrt(D)) / (2 * A)

    points = []
    for z in [z1, z2]:
        y = -z
        x = a * z
        points.append((x, y, z))

    return points


def rossler_fixed_point_formulas():
    a, b, c = symbols("a b c", real=True)
    x, y, z = symbols("x y z", real=True)

    eq1 = Eq(-y - z, 0)
    eq2 = Eq(x + a * y, 0)
    eq3 = Eq(b + z * (x - c), 0)

    solutions = solve([eq1, eq2, eq3], (x, y, z), dict=True)

    # uproszczenie wyników
    simplified = []
    for sol in solutions:
        simplified.append(
            {"x": simplify(sol[x]), "y": simplify(sol[y]), "z": simplify(sol[z])}
        )

    return simplified


def main():
    a = b = 0.2
    c = 5.7

    # print(fixed_points(a, b, c))

    point1 = (5.6929737951659, -28.464868975829496, 28.464868975829496)
    point2 = (0.007026204834100103, -0.03513102417050051, 0.03513102417050051)

    fixed = [point1, point2]

    i = 1
    for x, y, z in fixed:
        print("punkt P", i, ": ", x, ", ", y, ", ", z)
        vals, vecs = eigen_data_at_point(x, y, z, a, c)
        print()
        print("Eigenvalues:", vals)
        print()
        print("Eigenvectors:", vecs)
        i = i + 1
    # points = rossler_fixed_points(a=0.2, b=0.2, c=5.7)
    # for i, p in enumerate(points, 1):
    #     print(f"P{i} = {p}")
    #
    # print()
    # sols = rossler_fixed_point_formulas()
    # for i, s in enumerate(sols, 1):
    #     print(f"P{i}: x = {s['x']}, y = {s['y']}, z = {s['z']}")


if __name__ == "__main__":
    main()
