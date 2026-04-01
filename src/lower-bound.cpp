#include "../headers/utils.hpp"

void pw_lower_bound(interval &lower_bound, interval &left, interval &right,
                    IVector &x, int N, interval eps, int period) {
  interval mid;
  while ((right - left) >= eps) {
    mid = (left + right) / interval(2.0);
    RosslerSystem rossler(mid);
    if (rossler.is_periodic_and_stable(x, N, period, false)) {
      lower_bound = mid;
      right = mid;
    } else
      left = mid;
  }
}

int main() {
  std::cout.precision(16);

  int N = 300;
  double eps_val = 1e-9;

  try {
    std::cout << std::boolalpha;
    int input;

    do {
      std::cout
          << "==========================================================\n";
      std::cout << "|| RÖSSLER SYSTEM LOWER BOUND ANALYSIS \n";
      std::cout << "|| Current Iterations (N): " << N << "\n";
      std::cout << "|| Current Tolerance (eps): " << eps_val << "\n";
      std::cout
          << "==========================================================\n";
      std::cout << "|| 1. Measure Window I   (Period 6) [4.380, 4.381]\n";
      std::cout << "|| 2. Measure Window II  (Period 5) [4.694, 4.695]\n";
      std::cout << "|| 3. Measure Window III (Period 3) [5.185, 5.186]\n";
      std::cout << "|| 4. Change Iterations (N)\n";
      std::cout << "|| 5. Change Tolerance (eps)\n";
      std::cout << "|| 0. Exit\n";
      std::cout
          << "==========================================================\n";
      std::cout << "Your choice: ";
      std::cin >> input;
      std::cout << '\n';

      // Convert our double eps back to a strict CAPD interval for the math
      interval eps = interval(eps_val);

      switch (input) {
      case 1: {
        IVector p_close_to_6_periodic(
            {-7.459016042357787, 0.03638246580414306});
        double period = 6;
        interval lower_bound;
        interval left = interval(4.38);
        interval right = interval(4.381);

        std::cout << "[~] Calculating Window I... Please wait.\n";
        pw_lower_bound(lower_bound, left, right, p_close_to_6_periodic, N, eps,
                       period);

        RosslerSystem rossler(lower_bound);
        if (rossler.is_periodic_and_stable(p_close_to_6_periodic, N, period,
                                           true)) {
          std::cout << "\n[+] SUCCESS! Lower bound: " << lower_bound << "\n\n";
        } else {
          std::cout << "\n[-] Lower bound couldn't be established.\n\n";
        }
        break;
      }
      case 2: {
        IVector p_close_to_5_periodic(
            {-8.723283653276134, 0.03382484879611583});
        double period = 5;
        interval lower_bound;
        interval left = interval(4.694);
        interval right = interval(4.695);

        std::cout << "[~] Calculating Window II... Please wait.\n";
        pw_lower_bound(lower_bound, left, right, p_close_to_5_periodic, N, eps,
                       period);

        RosslerSystem rossler(lower_bound);
        if (rossler.is_periodic_and_stable(p_close_to_5_periodic, N, period,
                                           true)) {
          std::cout << "\n[+] SUCCESS! Lower bound: " << lower_bound << "\n\n";
        } else {
          std::cout << "\n[-] Lower bound couldn't be established.\n\n";
        }
        break;
      }
      case 3: {
        IVector p_close_to_3_periodic(
            {-3.562081704612412, 0.03491621098889878});
        double period = 3;
        interval lower_bound;
        interval left = interval(5.185);
        interval right = interval(5.186);

        std::cout << "[~] Calculating Window III... Please wait.\n";
        pw_lower_bound(lower_bound, left, right, p_close_to_3_periodic, N, eps,
                       period);

        RosslerSystem rossler(lower_bound);
        if (rossler.is_periodic_and_stable(p_close_to_3_periodic, N, period,
                                           true)) {
          std::cout << "\n[+] SUCCESS! Lower bound: " << lower_bound << "\n\n";
        } else {
          std::cout << "\n[-] Lower bound couldn't be established.\n\n";
        }
        break;
      }
      case 4: {
        std::cout << "|| Enter new number of iterations (N) [Current: " << N
                  << "]: ";
        std::cin >> N;
        if (N <= 0)
          N = 300; // Safety check
        std::cout << "\n[+] Iterations updated to " << N << "!\n\n";
        break;
      }
      case 5: {
        std::cout << "|| Enter new tolerance (e.g., 1e-9, 1e-12) [Current: "
                  << eps_val << "]: ";
        std::cin >> eps_val;
        if (eps_val <= 0)
          eps_val = 1e-9; // Safety check
        std::cout << "\n[+] Tolerance updated to " << eps_val << "!\n\n";
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
/*
|| 1. Window I

A stationary point for P^6 is in {[-7.459190184940876,
-7.459190184870349],[0.03638234203829317, 0.03638234203926301]} proven.
The periodic orbit is stable. Period 6 is the fundamental period of the point.
Lower bound: [4.380015412330626, 4.380015412330629]

==========================================================
Time: 190.742115967 s
==========================================================
|| 2. Window II

A stationary point for P^5 is in {[-8.723071020384911,
-8.723071020360603],[0.03382506844478626, 0.03382506844516819]} proven.
The periodic orbit is stable. Period 5 is the fundamental period of the point.
Lower bound: [4.694093659400938, 4.694093659400941]

==========================================================
Time: 174.264060796 s
==========================================================

|| 3. Window III

A stationary point for P^3 is in {[-3.565362529861338,
-3.565362529849111],[0.03491467693622938, 0.03491467693636346]} proven.
The periodic orbit is stable. Period 3 is the fundamental period of the point.
Lower bound: [5.185248633384702, 5.185248633384707]

==========================================================
Time: 114.074553072 s
=========================================================

*/
