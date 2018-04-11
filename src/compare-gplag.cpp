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

#include "common.h"
#include "PDG.h"
#include "pdgmykernelcomparator.h"
//EXPRSXP

using namespace Rcpp;
using namespace std;
using namespace boost;

vector<vector<GraphType> > createPDGMatrix(List parses,
                                           NumericVector sums,
                                           int n)
{
    vector<vector<GraphType> > PDGs(n);

    PDGMaker pdgmaker;

    for(int i = 0; i<n; i++)
    {
        PDGs[i] = vector<GraphType>(sums[i+1]-sums[i]);
    }

    for(int i = 0; i<n; ++i)
    {
        List list = (List)parses[i];
        for(int j = 0; j<sums[i+1]-sums[i]; j++)
        {
            if (MASTER_OR_SINGLE_THREAD) Rcpp::checkUserInterrupt(); // may throw an exception, fast op, not thread safe
            PDGs[i][j] = pdgmaker.MakePDG(list[j]);
        }
    }
    return PDGs;
}

vector<vector<double> > makeResultsCppMatrix(NumericVector sums, int n)
{
    vector<vector<double> > results(sums[n]);

    for(size_t i = 0; i<results.size(); ++i)
        results[i] = vector<double>(sums[n]);

    for(int i = 0; i<sums[n]; ++i)
    {
        for(int j = 0; j<sums[n]; ++j)
        {
            results[i][j] = -1.0;
        }
    }
    return results;
}

NumericMatrix copyResultsToR(NumericVector sums, int n,
                             const vector<vector<double> >& results)
{
    NumericMatrix resultsR(sums[n],sums[n]);
    for(int i = 0; i<sums[n]; ++i)
    {
        for(int j = 0; j<sums[n]; ++j)
        {
            resultsR(i,j) = results[i][j];
        }
    }
    return resultsR;
}

List parse2pdg2compare_test(
    List parses,
    NumericVector sums,
    PDGComparator* pdgcomparator
    )
{
  int n = parses.size();
  vector<vector<GraphType> > PDGs = createPDGMatrix(parses, sums, n);

  vector<vector<double> > results_asym = makeResultsCppMatrix(sums, n);
  vector<vector<double> > results_sym = makeResultsCppMatrix(sums, n);

  for(int i = 0; i<n; ++i)
  {
    for(int j = i+1; j<n; ++j)
    {
      if(i == j) continue;

      for(int k = 0; k<sums[i+1]-sums[i]; k++)
      {
        for(int l = 0; l<sums[j+1]-sums[j]; l++)
        {
          if (MASTER_OR_SINGLE_THREAD) Rcpp::checkUserInterrupt(); // may throw an exception, fast op, not thread safe
          int index1 = sums[i]+k;
          int index2 = sums[j]+l;
          vector<double> res = pdgcomparator->compare(PDGs[i][k],PDGs[j][l], true);

          results_asym[index1][index2] = res[0];
          results_asym[index2][index1] = res[1];
          results_sym[index1][index2] = res[2];
          results_sym[index2][index1] = res[2];
        }

      }

    }
  }

  NumericMatrix mat_asym = copyResultsToR(sums, n, results_asym);
  NumericMatrix mat_sym = copyResultsToR(sums, n, results_sym);
  List ret(2);
  ret[0] = mat_asym;
  ret[1] = mat_sym;
  return ret;
}

// [[Rcpp::export]]
List checkPlagiarism_gplag_cpp_list_kernel2_test(
    List parses,
    int h,
    NumericVector sums
)
{
  PDGMyKernelComparator* pdgmykernelcomparator  =    new PDGMyKernelComparator(h);
  List ret = parse2pdg2compare_test(parses,
                                    sums,
                                    pdgmykernelcomparator
  );
  delete pdgmykernelcomparator;
  return ret;
}
