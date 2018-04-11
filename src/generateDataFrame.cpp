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

#include <Rcpp.h>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <algorithm>
using namespace std;
using namespace Rcpp;

// [[Rcpp::export]]
DataFrame  generateDataFrame_one_rcpp(
                             NumericMatrix matKernel2,
                             CharacterVector functionNames
                             ) {
  size_t n = functionNames.size();
  size_t m = (n*n - n)/2;

  CharacterVector names1 = CharacterVector(m);
  CharacterVector names2 = CharacterVector(m);
  NumericVector kernel2 = NumericVector(m);

  int df_index = 0;

  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < n; j++) {

      if(j <= i)
      {
        continue;
      }

      names1[df_index] = functionNames[i];
      names2[df_index] = functionNames[j];

      kernel2[df_index] = matKernel2(i,j);

      df_index++;
    }
  }

  return DataFrame::create( Named("name1") = names1,
                            _["name2"] = names2,
                            _["SimilaR"] = kernel2
                           );
}

// [[Rcpp::export]]
DataFrame  generateDataFrame_one_both_rcpp(
                             NumericMatrix matKernel2,
                             CharacterVector functionNames
                             ) {
  size_t n = functionNames.size();
  size_t m = (n*n - n)/2;

  CharacterVector names1 = CharacterVector(m);
  CharacterVector names2 = CharacterVector(m);

  NumericVector kernel212 = NumericVector(m);
  NumericVector kernel221 = NumericVector(m);

  int df_index = 0;

  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < n; j++) {

      if(j <= i)
      {
        continue;
      }

      names1[df_index] = functionNames[i];
      names2[df_index] = functionNames[j];

      kernel212[df_index] = matKernel2(i,j); //min(matKernel2(i,j),matKernel2(j,i));
      kernel221[df_index] = matKernel2(j,i); //max(matKernel2(i,j),matKernel2(j,i));

      df_index++;
    }
  }

  return DataFrame::create( Named("name1") = names1,
                            _["name2"] = names2,
                            _["SimilaR12"] = kernel212,
                            _["SimilaR21"] = kernel221
                           );
}
