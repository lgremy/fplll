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

  virtual ~LLLReductionInterface();

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
  virtual void print_params() = 0;

  // Set the status of the computation and print message if verbose
  // TODO: this function is quite the same for HLLLReduction and LLLReduction. Can we try to
  // factorize the code at this level?
  virtual inline bool set_status(int new_status) = 0;
};

template <class ZT, class FT> inline LLLReductionInterface<ZT, FT>::~LLLReductionInterface() {}

FPLLL_END_NAMESPACE

#endif /* LLL_INTERFACE_H */
