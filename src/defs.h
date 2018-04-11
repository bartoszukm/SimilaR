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