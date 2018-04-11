/*    SimilaR package for R
 *    Copyright (C) 2018 M. Bartoszuk, M. Gagolewski
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */   

#ifndef __DEFS_H
#define __DEFS_H

#define VERBOSE 0

#define KERNELSUBTREE_VERBOSE 2

#if VERBOSE > 0
#define STOPIFNOT(EXPR) { if (!(EXPR)) Rprintf("\a*** Assert failed: " #EXPR " at %s, line %d ***\n", __FILE__, __LINE__); }
#else
#define STOPIFNOT(EXPR) { }
#endif

#if VERBOSE > 0
#define RCOUT(msg, verlvl) if ((verlvl) <= VERBOSE) Rcpp::Rcout << msg << endl;
#else
#define RCOUT(msg, verlvl) { }
#endif

/*
 * example use:
 *  int a = 5;
 *  RCOUT("a=" << a, 0)
 *
 */

#endif