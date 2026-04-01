#include "../headers/utils.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

using namespace capd;

struct Config {
  double a_min;
  double a_max;
  int default_resolution;
  std::string out_a_file;
  std::string out_x_file;
};

void bf_diagram_data(const std::vector<double> &a_range, DVector x, int N,
                     const std::string &a_file, const std::string &x_file) {
  std::ofstream a_out(a_file), x_out(x_file);

  // Safety check for the output folder
  if (!a_out.is_open() || !x_out.is_open()) {
    std::cout << "\n[-] ERROR: Could not open files for saving.\n";
    std::cout << "[-] Did you forget to create the 'output' directory?\n\n";
    return;
  }

  std::cout
      << "\n[~] Generating Poincare map data... This might take a moment.\n";

  for (double a : a_range) {
    PoincareMap poincare(a);
    DVector current_x = x; // Reset to the initial condition for each 'a'

    for (int t = 0; t < N; t++) {
      current_x = poincare.map(current_x);
      // Skip the first 250 iterations to allow the trajectory to settle on the
      // attractor (transient cutoff)
      if (t > 250) {
        a_out << a << ",";
        x_out << current_x[1] << ",";
      }
    }
  }

  a_out.close();
  x_out.close();
  std::cout << "[+] Done! Data successfully saved to " << a_file << " and "
            << x_file << "\n\n";
}

int main() {
  std::cout.precision(16);

  // Default parameters
  int N = 1000;
  double x0 = 0.0, y0 = -5.0, z0 = 0.03;
  double initial_cond[] = {x0, y0, z0};
  DVector x(3, initial_cond);

  // 0 means "use the default resolution for the selected region"
  int custom_resolution = 0;

  std::unordered_map<std::string, Config> configs = {
      {"whole", {2.5, 5.7, 2000, "output/bif-a.csv", "output/bif-x.csv"}},
      {"first",
       {4.379, 4.4, 350, "output/fpw-bif-a.csv", "output/fpw-bif-x.csv"}},
      {"second",
       {4.693, 4.725, 1000, "output/spw-bif-a.csv", "output/spw-bif-x.csv"}},
      {"third",
       {5.17, 5.55, 1000, "output/tpw-bif-a.csv", "output/tpw-bif-x.csv"}}};

  try {
    std::cout << std::boolalpha;
    int input;

    do {
      std::cout
          << "==========================================================\n";
      std::cout << "|| RÖSSLER BIFURCATION DIAGRAM GENERATOR \n";
      std::cout << "|| Current Point:      {" << x0 << ", " << y0 << ", " << z0
                << "}\n";
      std::cout << "|| Current Iterations: " << N << "\n";
      std::cout << "|| Custom Resolution:  "
                << (custom_resolution > 0
                        ? std::to_string(custom_resolution) + " points"
                        : "Default per region")
                << "\n";
      std::cout
          << "==========================================================\n";
      std::cout << "|| 1. Generate Whole Diagram [2.5, 5.7]\n";
      std::cout << "|| 2. Generate Window I      [4.379, 4.400]\n";
      std::cout << "|| 3. Generate Window II     [4.693, 4.725]\n";
      std::cout << "|| 4. Generate Window III    [5.170, 5.550]\n";
      std::cout << "|| 5. Change Initial Point\n";
      std::cout << "|| 6. Change Iterations (N)\n";
      std::cout << "|| 7. Override Scan Resolution\n";
      std::cout << "|| 0. Exit\n";
      std::cout
          << "==========================================================\n";
      std::cout << "Your choice: ";
      std::cin >> input;
      std::cout << '\n';

      std::string choice_key = "";

      switch (input) {
      case 1:
        choice_key = "whole";
        break;
      case 2:
        choice_key = "first";
        break;
      case 3:
        choice_key = "second";
        break;
      case 4:
        choice_key = "third";
        break;
      case 5: {
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
        continue; // Skip the generation block and redraw menu
      }
      case 6: {
        std::cout << "|| Enter new number of iterations (N) [Current: " << N
                  << "]: ";
        std::cin >> N;
        if (N <= 250) { // Safety check because we skip the first 250 in
                        // bf_diagram_data
          N = 1000;
          std::cout << "[-] N must be greater than 250. Resetting to 1000.\n";
        }
        std::cout << "\n[+] Iterations updated to " << N << "!\n\n";
        continue;
      }
      case 7: {
        std::cout
            << "|| Enter new resolution (enter 0 to restore region defaults): ";
        std::cin >> custom_resolution;
        if (custom_resolution < 0)
          custom_resolution = 0;
        std::cout << "\n[+] Resolution updated!\n\n";
        continue;
      }
      case 0:
        std::cout << "Exiting...\n";
        continue;
      default:
        std::cout << "[-] Invalid choice.\n\n";
        continue;
      }

      // If we reach here, it means the user selected option 1, 2, 3, or 4
      if (choice_key != "") {
        Config cfg = configs[choice_key];
        int active_resolution = (custom_resolution > 0)
                                    ? custom_resolution
                                    : cfg.default_resolution;

        std::cout << "Generating " << active_resolution << " points for the '"
                  << choice_key << "' region...\n";

        std::vector<double> a_range =
            linspace<double>(cfg.a_min, cfg.a_max, active_resolution);
        bf_diagram_data(a_range, x, N, cfg.out_a_file, cfg.out_x_file);
      }

    } while (input != 0);

  } catch (std::exception &e) {
    std::cout << "\n\nException: " << e.what() << std::endl;
  }
  return 0;
}
