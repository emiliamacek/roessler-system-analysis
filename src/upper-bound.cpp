#include "../headers/utils.hpp"
#include <string>

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

void find_upper_bound(std::vector<double> &a_range, DVector x, int N,
                      double eps) {
  const int stable_count = 10; // how many positive LE

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

void plot_data(std::vector<double> &a_range, DVector x, int N,
               const std::string &prefix) {
  std::string dir = "output/";
  std::ofstream a_param(dir + prefix + "-a.csv");
  std::ofstream lambda1(dir + prefix + ".csv");

  if (!a_param.is_open() || !lambda1.is_open()) {
    std::cout << "\n[-] ERROR: Could not open files for saving.\n";
    std::cout << "[-] Did you forget to create the 'output' directory?\n\n";
    return;
  }

  std::cout << "\n[~] Saving data to " << dir << prefix << "... Please wait.\n";

  for (double a : a_range) {
    a_param << a << '\n';
    lambda1 << lyapunov_exponent(a, x, N) << '\n';
  }

  a_param.close();
  lambda1.close();
  std::cout << "[+] Data successfully saved!\n\n";
}

int main() {
  int N = 6000;
  double eps = 1e-5;

  // Default values
  double x0 = 0.0, y0 = -7.0, z0 = 0.03;
  double initial_cond[] = {x0, y0, z0};
  DVector x(3, initial_cond);

  int num_points = 500;
  bool save_to_file = false;

  std::cout.precision(16);
  try {
    std::cout << std::boolalpha;
    int input;

    do {
      // The Dynamic Menu
      std::cout
          << "==========================================================\n";
      std::cout
          << "|| RÖSSLER SYSTEM UPPER BOUND AND LYAPUNOV EXPONENT ANALYSIS\n";
      std::cout << "|| Current Point:      (" << x0 << ", " << y0 << ", " << z0
                << ")\n";
      std::cout << "|| Current Resolution: " << num_points << " points\n";
      std::cout << "|| Save to CSV:        "
                << (save_to_file ? "[ON]" : "[OFF]") << "\n";
      std::cout
          << "==========================================================\n";
      std::cout << "|| 1. Measure Window I   [4.395, 4.396]\n";
      std::cout << "|| 2. Measure Window II  [4.720, 4.722]\n";
      std::cout << "|| 3. Measure Window III [5.490, 5.510]\n";
      std::cout << "|| 4. Change Initial Point\n";
      std::cout << "|| 5. Change Resolution\n";
      std::cout << "|| 6. Toggle Save to File\n";
      std::cout << "|| 0. Exit\n";
      std::cout
          << "==========================================================\n";
      std::cout << "Your choice: ";
      std::cin >> input;
      std::cout << '\n';

      switch (input) {
      case 1: {
        std::vector<double> a_range =
            linspace<double>(4.395, 4.396, num_points);
        find_upper_bound(a_range, x, N, eps);
        if (save_to_file)
          plot_data(a_range, x, N, "fpw-le"); // First Periodic Window
        break;
      }
      case 2: {
        std::vector<double> a_range = linspace<double>(4.72, 4.722, num_points);
        find_upper_bound(a_range, x, N, eps);
        if (save_to_file)
          plot_data(a_range, x, N, "spw-le"); // Second Periodic Window
        break;
      }
      case 3: {
        std::vector<double> a_range = linspace<double>(5.49, 5.51, num_points);
        find_upper_bound(a_range, x, N, eps);
        if (save_to_file)
          plot_data(a_range, x, N, "tpw-le"); // Third Periodic Window
        break;
      }
      case 4: {
        std::cout << "|| Enter new x0: ";
        std::cin >> x0;
        std::cout << "|| Enter new y0: ";
        std::cin >> y0;
        std::cout << "|| Enter new z0: ";
        std::cin >> z0;

        initial_cond[0] = x0;
        initial_cond[1] = y0;
        initial_cond[2] = z0;
        x = DVector(3, initial_cond);
        std::cout << "\n[+] Point updated!\n\n";
        break;
      }
      case 5: {
        std::cout << "|| Enter new number of points: ";
        std::cin >> num_points;
        if (num_points <= 1)
          num_points = 500; // Safety fallback
        std::cout << "\n[+] Resolution updated!\n\n";
        break;
      }
      case 6: {
        save_to_file = !save_to_file; // Flips OFF to ON, or ON to OFF
        std::cout << "\n[+] File saving is now "
                  << (save_to_file ? "ON" : "OFF") << "!\n\n";
        break;
      }
      case 0:
        std::cout << "Exiting...\n";
        break;
      default:
        std::cout << "[-] Invalid choice.\n\n";
        break;
      }
    } while (input != 0);

  } catch (std::exception &e) {
    std::cout << "\n\nException: " << e.what() << std::endl;
  }
  return 0;
}

//  UPPER BOUND: 4.39562124248497 LE: -0.0009426722763948911
//  UPPER BOUND: 4.720761523046092 LE: -0.003559942437847776
//  UPPER BOUND: 5.497374749498998 LE: -0.003205464020819981
