#include "../headers/utils.hpp"

static const char* menu =
    "======================================================"
    "====\n"
    "|| Choose a window to measure: \n"
    "|| 1. Window I  \n"
    "|| 2. Window II \n"
    "|| 3. Window III \n"
    "======================================================"
    "====\n";

static const char* prompt = "Your choice ";

DVector random_unit_vector() {
	static thread_local std::mt19937 gen(std::random_device{}());
	static thread_local std::normal_distribution<> d(0, 1);
	DVector vec(3);
	vectalg::EuclLNorm<DVector, DMatrix> norm;

	for (int i = 0; i < 3; i++) {
		vec[i] = d(gen);
	}

	return vec / norm(vec);
}

double lyapunov_exponent(double a, DVector x, int N) {
	PoincareMap poincare(a);
	static vectalg::EuclLNorm<DVector, DMatrix> norm;

	DVector u = random_unit_vector();
	double sum_log_norms = 0;

	for (int i = 0; i < N; ++i) {
		DMatrix monodromy_matrix(3, 3);
		DVector P = poincare.map(x, monodromy_matrix);
		DMatrix DP = poincare.map.computeDP(P, monodromy_matrix);

		x = P;
		u = DP * u;

		double norm_u = norm(u);
		sum_log_norms += std::log(norm_u);
		u /= norm_u;
	}

	return sum_log_norms / N;
}

void find_upper_bound(
    std::vector<double>& a_range,
    DVector x,
    int N,
    double eps) {
	const int stable_count = 10;  // how many positive LE

	double prev_a = a_range[0];
	double prev_le = lyapunov_exponent(a_range[0], x, N);

	for (size_t i = 1; i < a_range.size(); ++i) {
		double a = a_range[i];
		double le = lyapunov_exponent(a, x, N);

		// traverse from positive to negative
		if (prev_le < 0 && le >= eps) {
			bool stable = true;

			// check positive LE stsbility for the next stable_count points
			for (int j = 1; j <= stable_count && (i + j) < a_range.size(); ++j) {
				double le_check = lyapunov_exponent(a_range[i + j], x, N);

				if (le_check < eps) {
					stable = false;
					break;
				}
			}

			if (stable) {
				std::cout << "\n\n UPPER BOUND: " << prev_a << " LE: " << prev_le
				          << "\n\n";
				return;
			}
		}

		prev_le = le;
		prev_a = a;
	}
}

void plot_data(std::vector<double>& a_range, DVector x, int N) {
	std::ofstream a_param("output/fpw-le-a.csv"), lambda1("output/fpw-le.csv");

	for (double a : a_range) {
		a_param << a << '\n';
		lambda1 << lyapunov_exponent(a, x, N) << '\n';
	}

	a_param.close();
	lambda1.close();
}

int main() {
	int N = 6000;
	double initial_cond[] = {0., -7, 0.03};
	DVector x(3, initial_cond);
	double eps = 1e-5;
	cout.precision(16);
	try {
		cout << boolalpha;

		int input;
		do {
			cout << menu;
			cout << prompt;
			cin >> input;
			cout << '\n';

			switch (input) {
				case 1: {
					std::vector<double> a_range = linspace<double>(4.395, 4.396, 500);

					find_upper_bound(a_range, x, N, eps);

					// ======
					// std::vector<double> a_range =
					//     linspace<double>(4.375, 4.4, 5000);
					// plot_data(a_range, x, N);

					break;
				};
				case 2: {
					std::vector<double> a_range = linspace(4.72, 4.722, 500);

					find_upper_bound(a_range, x, N, eps);

					// ======
					// std::vector<double> a_range =
					//     linspace<double>(4.39, 4.726, 5000);
					// plot_data(a_range, x, N);
					break;
				}
				case 3: {
					std::vector<double> a_range = linspace(5.49, 5.51, 500);

					find_upper_bound(a_range, x, N, eps);

					// ======
					// std::vector<double> a_range =
					//     linspace<double>(5.15, 5.525, 5000);
					// plot_data(a_range, x, N);
					break;
				}
				default:
					break;
			}
		} while (input == 1 || input == 2 || input == 3);

	} catch (exception& e) {
		cout << "\n\nException: " << e.what() << endl;
	}
	return 0;
}

int main1() {
	std::vector<double> a_range = linspace(4.379, 4.4, 10000);
	int N = 1000;

	double initial_cond[] = {0., -7, 0.03};
	DVector x(3, initial_cond);
	plot_data(a_range, x, N);
	return 0;
}

//  UPPER BOUND: 4.39562124248497 LE: -0.0009426722763948911
//  UPPER BOUND: 4.720761523046092 LE: -0.003559942437847776
//  UPPER BOUND: 5.497374749498998 LE: -0.003205464020819981
