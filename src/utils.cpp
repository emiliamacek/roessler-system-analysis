#include "../headers/utils.hpp"
PoincareMap::PoincareMap(double a)
    : rossler("par:a,b;var:x,y,z;fun:-(y+z),x+b*y,b+z*(x-a);"),
      solver(rossler, 20),
      section(3, 0),
      map(solver, section, poincare::MinusPlus) {
	rossler.setParameter("a", a);
	rossler.setParameter("b", 2. / 10.);
}

// return image of P^iter and saves differential in DP
IVector SectionMap::image(IVector& x, IMatrix& DP, int iter) {
	IVector X({0., 0., 0.});
	X[1] = x[0];
	X[2] = x[1];

	C1Rect2Set Q(X);
	IMatrix DP3(3, 3);

	IVector Y = (*P)(Q, DP3, iter);
	DP3 = P->computeDP(Y, DP3, iter);

	IVector y(2);
	y[0] = Y[1];
	y[1] = Y[2];
	return y;
}

// calculates only image
IVector SectionMap::image(IVector& x, int iter) {
	IVector X({0., 0., 0.});
	X[1] = x[0];
	X[2] = x[1];

	C0Rect2Set Q(X);

	IVector Y = (*P)(Q, iter);

	IVector y(2);
	y[0] = Y[1];
	y[1] = Y[2];

	return y;
}

// calculates only derivative
IMatrix SectionMap::derivative(IVector& x, int iter) {
	IMatrix DP(2, 2);
	image(x, DP, iter);
	return DP;
}

vector<interval> eigenvalues2x2(IMatrix& M) {
	interval a = M[0][0];
	interval b = M[0][1];
	interval c = M[1][0];
	interval d = M[1][1];

	interval trace = a + d;
	interval det = a * d - b * c;
	interval discriminant = sqrt(sqr(trace) - 4 * det);

	interval lambda1 = (trace + discriminant) / 2;
	interval lambda2 = (trace - discriminant) / 2;

	return {lambda1, lambda2};
}

// checks whether "iter"-period is fundamental period for "point"
bool RosslerSystem::has_smaller_iter(IVector& point, int iter) {
	for (int d = 1; d < iter; ++d) {
		if (iter % d == 0) {
			IVector image = poincare_2d(point, d);
			if (subset(image, point)) {
				cout << "The point has a smaller fundamental "
				        "period: "
				     << d << '\n';
				return true;
			}
		}
	}

	cout << "Period " << iter << " is the fundamental period of the point.\n";
	return false;
}

bool RosslerSystem::is_periodic_and_stable(
    IVector& initial_guess,
    int N,
    int iter,
    bool is_final_check) {
	IVector current_guess = initial_guess;
	IVector poincare_image, prev_guess, verified_point(2);
	IMatrix Id = IMatrix({{1., 0.}, {0., 1.}});
	IMatrix dp(2, 2);

	int steps = 0;
	try {
		// initial iterations needed for the trajectory to stabilized
		while (true) {
			current_guess = midVector(current_guess);
			prev_guess = current_guess;

			poincare_image = poincare_2d(current_guess, dp, iter);
			current_guess =
			    current_guess - gauss(dp - Id, poincare_image - current_guess);

			++steps;
			if (subset(prev_guess, current_guess) || steps >= N)
				break;
		}

		// if point is the right candidate use Newton method
		if (steps < N) {
			double delta = 1e-9;
			IVector small_box(2);
			small_box[0] = current_guess[0] + delta * interval(-1.0, 1.0);
			small_box[1] = current_guess[1] + delta * interval(-1.0, 1.0);

			poincare_2d(small_box, dp, iter);

			verified_point =
			    current_guess -
			    gauss(dp - Id, poincare_2d(current_guess, iter) - current_guess);

			// if the verified point is a subset of the interior
			if (subsetInterior(verified_point, small_box)) {
				// check stability
				poincare_2d(verified_point, dp, iter);
				auto eigenvalues = eigenvalues2x2(dp);

				bool is_stable = true;
				for (const auto& eig : eigenvalues) {
					if (abs(mid(eig)) >= 1.0) {
						is_stable = false;
						break;
					}
				}

				// check whether smaller period exist
				if (is_final_check) {
					std::cout << "A stationary point for P^" << iter << " is in "
					          << verified_point << " proven. \n";

					std::cout << "The perioic orbit is "
					          << (is_stable ? "stable." : "unstable.") << '\n';

					return !has_smaller_iter(verified_point, iter);
				}

				return is_stable;
			}
		} else {
		}
	} catch (const exception& e) {
		cout << "Exception caught for initial guess " << initial_guess << ": "
		     << e.what() << endl;
	}

	return false;
}
