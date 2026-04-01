#pragma once

#include "capd/capdAlglib.h"
#include "capd/capdlib.h"
#include "capd/covrel/HSet2D.h"
#include "capd/covrel/HSetMD.h"
#include "capd/dynsys/DynSysMap.h"
#include "capd/intervals/lib.h"
#include "capd/poincare/BasicPoincareMap.h"
#include "capd/poincare/lib.h"
#include "capd/vectalg/Norm.h"
#include "capd/vectalg/lib.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <functional>
#include <future>
#include <iomanip>
#include <iostream>
#include <map>
#include <mutex>
#include <omp.h>
#include <optional>
#include <random>
#include <sstream>
#include <thread>
#include <type_traits>
#include <vector>

using namespace std;
using namespace capd;
using namespace capd::alglib;
using namespace capd::vectalg;
using namespace capd::matrixAlgorithms;

typedef capd::covrel::HSet2D<DMatrix, IMatrix> HSet2D;
typedef capd::dynsys::DynSysMap<IMap> DynSysMap;
typedef capd::covrel::HSetMD<DMatrix, IMatrix> HSet;
typedef capd::covrel::GridSet<IMatrix> GridSet;

struct PoincareMap {
  DMap rossler;
  DOdeSolver solver;
  DCoordinateSection section;
  DPoincareMap map;

  PoincareMap(double a);
};

class SectionMap {
public:
  IPoincareMap *P;
  SectionMap(IPoincareMap &Q) { P = &Q; }
  SectionMap() {}

  IVector image(IVector &x, IMatrix &DP, int iter = 1);
  IVector image(IVector &x, int iter = 1);
  IMatrix derivative(IVector &x, int iter = 1);

  IVector operator()(IVector &x, IMatrix &DP, int iter = 1) {
    return image(x, DP, iter);
  }
  IVector operator()(IVector &x, int iter = 1) { return image(x, iter); }
  IMatrix operator[](IVector &x) { return derivative(x); }
};

struct RosslerSystem {
  int order;
  IMap rossler;
  ICoordinateSection section;
  IOdeSolver solver;
  IPoincareMap P;
  SectionMap poincare_2d;
  int iteration;

  RosslerSystem(interval a)
      : order(20),
        rossler("par:a,b; var:x,y,z; fun: -y - z, x + b*y, b + z*(x - a);"),
        section(3, 0), solver(rossler, order),
        P(solver, section, poincare::MinusPlus), poincare_2d(P) {
    rossler.setParameter("a", a);
    rossler.setParameter("b", interval(0.2));
  }

  bool has_smaller_iter(IVector &point, int iter);
  bool is_periodic_and_stable(IVector &initial_guess, int N, int iter,
                              bool is_final_check);
};

template <typename T> std::vector<T> linspace(T start, T end, int num) {
  std::vector<T> result;
  if (num <= 0)
    return result;
  if (num == 1) {
    result.push_back(start);
    return result;
  }

  T step = (end - start) / static_cast<T>(num - 1);
  for (int i = 0; i < num; ++i) {
    result.push_back(start + step * i);
  }

  return result;
}
template <typename Func>
void measure_time(const std::string &label, Func func) {
  auto start = std::chrono::high_resolution_clock::now();

  func();

  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration<double>(end - start).count();

  std::cout << label << ": " << duration << " sekund\n";
}
