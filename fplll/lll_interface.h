/* Copyright (C) 2005-2008 Damien Stehle.
   Copyright (C) 2007 David Cade.
   Copyright (C) 2011 Xavier Pujol.
   Copyright (C) 2018 Laurent Grémy.

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

#ifndef LLL_INTERFACE_H
#define LLL_INTERFACE_H

#include "nr/nr.h"

FPLLL_BEGIN_NAMESPACE

template <class ZT, class FT> class LLLReductionInterface
{
public:
  LLLReductionInterface(double arg_delta, double arg_eta, int flags)
      : delta(arg_delta), eta(arg_eta), status(RED_SUCCESS)
  {
    verbose = flags & LLL_VERBOSE;
  }

  virtual ~LLLReductionInterface() {}

  /**
     @brief Size reduction.

     Perform size reduction for all vectors between `kappa_start` and `kappa_end`.

     @param kappa_min start index
     @param kappa_end end index (exclusive)
     @param size_reduction_start only perform size reductions using vectors starting at this index
     @return success or failure (due to numerical instability)
  */

  virtual inline bool size_reduction(int kappa_min, int kappa_end, int size_reduction_start) = 0;

  // Get the status of the computation
  inline int get_status() { return status; }

protected:
  // Paramters to (delta, eta) LLL-reduce or (delta, eta, theta) HLLL-reduce
  FT delta;
  FT eta;

  // Verbose mode.
  bool verbose;

  int status;

  /**
   * In verbose mode, print informations to reproduce the computation (parameters, enable features)
   */
  virtual inline void print_params() = 0;

  // Set the status of the computation and print message if verbose
  inline bool set_status(int new_status);
};

template <class ZT, class FT> inline bool LLLReductionInterface<ZT, FT>::set_status(int new_status)
{
  status = new_status;
  return status == RED_SUCCESS;
}

FPLLL_END_NAMESPACE

#endif /* LLL_INTERFACE_H */
