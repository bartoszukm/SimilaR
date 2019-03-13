##    SimilaR package for R
##    Copyright (C) 2018-2019 by M. Bartoszuk, M. Gagolewski
##
##    This program is free software: you can redistribute it and/or modify
##    it under the terms of the GNU General Public License as published by
##    the Free Software Foundation, either version 3 of the License, or
##    (at your option) any later version.
##
##    This program is distributed in the hope that it will be useful,
##    but WITHOUT ANY WARRANTY; without even the implied warranty of
##    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##    GNU General Public License for more details.
##
##    You should have received a copy of the GNU General Public License
##    along with this program.  If not, see <http://www.gnu.org/licenses/>.

PACKAGE_NAME <- "SimilaR"

myprint <- function(x)
{
  capture.output(print(substitute(x)))
}

makeMMatrix <- function(matrix)
{
  outMatrix <- matrix
  for(iter1 in 1:nrow(matrix))
  {
    for(iter2 in 1:ncol(matrix))
    {
      if(iter1<iter2)
      {
        x <- matrix[iter1,iter2]
        y <- matrix[iter2,iter1]
        outMatrix[iter1, iter2] <- outMatrix[iter2, iter1] <- (x+y)/2
      }
    }
  }
  outMatrix
}

makePrettyMatrix <- function(matrix_, functionNames)
{
  kernel2_matrix <-  matrix_
  d_names <- unlist(functionNames)
  dimnames(kernel2_matrix) <- list(d_names,d_names)
  kernel2_matrix[kernel2_matrix==-1.0] <- NA
  kernel2_matrix
}
