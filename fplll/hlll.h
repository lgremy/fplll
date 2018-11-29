/* Copyright (C) 2005-2008 Damien Stehle.
   Copyright (C) 2007 David Cade.
   Copyright (C) 2011 Xavier Pujol.
   Copyright (C) 2017-1018 Laurent Grémy.

   This file is part of fplll. fplll is free software: you
   can redistribute it and/or modify it under the terms of the GNU Lesser
   General Public License as published by the Free Software Foundation,
   either version 2.1 of the License, or (at your option) any later version.

   fplll is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with fplll. If not, see <http://www.gnu.org/licenses/>. */

#ifndef FPLLL_HLLL_H
#define FPLLL_HLLL_H

#include "householder.h"
#include "lll_interface.h"
#include <cmath>

FPLLL_BEGIN_NAMESPACE

template <class ZT, class FT> class HLLLReduction : public LLLReductionInterface<ZT, FT>
{
public:
  /**
   * Constructor.
   * The precision of FT must be defined before creating an instance of the
   * class and must remain the same until the object is destroyed (or no longer
   * needed).
   */
  HLLLReduction(MatHouseholder<ZT, FT> &arg_m, double delta, double eta, double theta, double c,
                int flags)
      : LLLReductionInterface<ZT, FT>(delta, eta, flags), m(arg_m)
  {
    this->theta = theta;
    this->c     = c;
    sr          = pow(2.0, -(double)m.get_d() * c);
    dR.resize(m.get_d());
    eR.resize(m.get_d());
  }

  /**
    @brief Househorder inside LLL reduction.
    */
  bool lll();

  /**
     @brief Size reduction.

     Perform size reduction for all vectors between `kappa_start` and `kappa_end`.

     @param kappa_min start index
     @param kappa_end end index (exclusive)
     @param size_reduction_start only perform size reductions using vectors starting at this index
     @return success or failure (due to numerical instability)
  */
  inline bool size_reduction(int kappa_min = 0, int kappa_end = -1, int size_reduction_start = 0);

private:
  // Paramters to (delta, eta, theta) hlll-reduce the basis b in m.
  using LLLReductionInterface<ZT, FT>::delta;
  using LLLReductionInterface<ZT, FT>::eta;
  FT theta;
  MatHouseholder<ZT, FT> &m;

  // Arbitraty c > 0
  FT c;
  // Multiplicative coefficient used to check if a vector is size-reduced or not.
  FT sr;
  // Verbose mode.
  using LLLReductionInterface<ZT, FT>::verbose;

  // Temporary variables
  FT ftmp0, ftmp1, ftmp2;
  long expo0, expo1, expo2;

  using LLLReductionInterface<ZT, FT>::status;

  /**
     @brief Size reduction.

     Perform size reduction of b[kappa]. Reduce b[kappa] with
     b[size_reduction_start..size_reduction_end-1].

     @param kappa index of the vector
  */
  bool weak_size_reduction(int kappa, int size_reduction_end, int size_reduction_start = 0);

  /**
   * In verbose mode, print informations to reproduce the computation (parameters, enable features)
   */
  inline void print_params();

  // Precompute dR[k] * 2^(2*row_expo[k]) = delta_ * R(k, k)^2
  vector<FT> dR;

  // Compute dR[k]
  inline void compute_dR(int k);

  // Set the status of the computation and print message if verbose
  inline bool set_status(int new_status);

  // Precompute eR[k] * 2^row_expo[k] = eta * R(k, k)
  vector<FT> eR;

  // Compute eR[k]
  inline void compute_eR(int k);

  // Verify if b[k] is is correctry size reduced
  bool verify_weak_size_reduction(int kappa);

  // Test if delta * R(k - 1, k - 1)^2 <= ||b[k]||^2 - sum_{i = 0}^{i < k - 1}R[k][i]^2 (depending
  // on the way ftmp1 is computed, this test can be slightly different, but the purpose keeps the
  // same)
  bool lovasz_test(int k);
};

template <class ZT, class FT> inline void HLLLReduction<ZT, FT>::print_params()
{
  cerr << "Entering HLLL" << endl
       << "delta = " << delta << endl
       << "eta = " << eta << endl
       << "theta = " << theta << endl
       << "c = " << c << endl
       << "precision = " << FT::get_prec() << endl
       << "row_expo = " << static_cast<int>(m.is_enable_row_expo()) << endl
       << "long_in_size_reduction = " << static_cast<int>(m.is_row_op_force_long()) << endl;

#ifndef HOUSEHOLDER_PRECOMPUTE_INVERSE
  cerr << "householder_precompute_inverse = 0" << endl;
#else   // HOUSEHOLDER_PRECOMPUTE_INVERSE
  cerr << "householder_precompute_inverse = 1" << endl;
#endif  // HOUSEHOLDER_PRECOMPUTE_INVERSE

#ifndef HOUSEHOLDER_USE_SIZE_REDUCTION_TEST
  // Condition to break incomplete size reduction: ||b[k]||^2 > 0.1 * t
  cerr << "householder_use_size_reduction_test = 0" << endl;
#else   // HOUSEHOLDER_USE_SIZE_REDUCTION_TEST
  // Condition to break incomplete size reduction: ||b[k]||^2 > sr * t
  cerr << "householder_use_size_reduction_test = 1" << endl;
#endif  // HOUSEHOLDER_USE_SIZE_REDUCTION_TEST

#ifndef HOUSEHOLDER_VERIFY_SIZE_REDUCTION_HPLLL
  cerr << "householder_verify_size_reduction_hplll = 0" << endl;
#else   // HOUSEHOLDER_VERIFY_SIZE_REDUCTION_HPLLL
  cerr << "householder_verify_size_reduction_hplll = 1" << endl;
#endif  // HOUSEHOLDER_VERIFY_SIZE_REDUCTION_HPLLL
}

template <class ZT, class FT> inline void HLLLReduction<ZT, FT>::compute_dR(int k)
{
  m.get_R(dR[k], k, k);
  dR[k].mul(dR[k], dR[k]);
  dR[k].mul(delta, dR[k]);  // dR[k] = delta * R(k, k)^2
}

template <class ZT, class FT> inline void HLLLReduction<ZT, FT>::compute_eR(int k)
{
  m.get_R(eR[k], k, k);
  eR[k].mul(delta, eR[k]);  // eR[k] = eta * R(k, k)
}

template <class ZT, class FT> inline bool HLLLReduction<ZT, FT>::set_status(int new_status)
{
  if (verbose)
  {
    if (new_status == RED_SUCCESS)
      cerr << "End of HLLL: success" << endl;
    else
      cerr << "End of HLLL: failure: " << RED_STATUS_STR[new_status] << endl;
  }

  return LLLReductionInterface<ZT, FT>::set_status(new_status);
}

/* TODO: beware, this function is classically used in the context of BKZ. Since
 * it is under developpement to integrate HLLL inside BKZ, this function may be
 * not sufficient to guarantee its expected properties in such a context.
 *
 * NOTA: this function was never tested in any context for now. The goal of this
 * function is to allow to provide something that can be the equivalent of
 * size_reduction in LLLReduction in ordre to provide a common interface between
 * HLLLReduction and LLLReduction to integrate HLLL inside BKZ.
 */
template <class ZT, class FT>
inline bool HLLLReduction<ZT, FT>::size_reduction(int kappa_min, int kappa_end,
                                                  int size_reduction_start)
{
  if (kappa_end == -1)
    kappa_end = m.get_d();

  // Size reduced b[0] is not allowed, then go to b[1]
  if (kappa_min == 0)
    kappa_min++;

  for (int k = kappa_min; k < kappa_end; k++)
  {
    if (!weak_size_reduction(k, k, size_reduction_start))
      return false;

    m.update_R_last(k);
  }
  return set_status(RED_SUCCESS);
}

template <class ZT, class FT>
int is_hlll_reduced(MatHouseholder<ZT, FT> &m, double delta, double eta, double theta);

FPLLL_END_NAMESPACE

#endif /* FPLLL_HLLL_H */
