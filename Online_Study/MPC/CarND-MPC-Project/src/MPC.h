#ifndef MPC_H
#define MPC_H

#include <cppad/cppad.hpp>
#include <cppad/ipopt/solve.hpp>
#include <vector>
#include "Eigen-3.3/Eigen/Core"
typedef CPPAD_TESTVECTOR(double) Dvector;

using namespace std;

class MPC {
 public:
  Dvector  vars_lowerbound;
  Dvector  vars_upperbound;
  Dvector  constraints_lowerbound;
  Dvector  constraints_upperbound;
  
  std::vector<double> future_x;
  std::vector<double> future_y;

  MPC();

  virtual ~MPC();

  // Solve the model given an initial state and polynomial coefficients.
  // Return the first actuatotions.
  vector<double> Solve(Eigen::VectorXd state, Eigen::VectorXd coeffs);
};

#endif /* MPC_H */
