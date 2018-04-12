##    SimilaR package for R
##    Copyright (C) 2018 M. Bartoszuk, M. Gagolewski
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

#' @title
#' SimilaR_fromTwoFunctions
#'
#' @description
#' An implementation of the SimilaR algorithm - a novel R code similarity evaluation algorithm based on program dependence graphs. 
#' This version gets two function objects and compare them. It can be used e.g. for testing purposes.
#'
#' @param function1 a function object to compare
#' @param function2 a function object to compare
#' @param returnType \code{"data.frame"} or \code{"matrix"}; indicates the output object type (see below).
#' @param aggregation \code{"sym"}, \code{"tnorm"}, or \code{"both"}; specifies which model of similarity asymmetry should be used. \code{"sym"} means
#' that one value of similarity is computed. \code{"tnorm"} means that two values are obtained: one means how much
#' the first function is a subset of the second, and the other one means how much the second function is a subset of the first.
#' After that they are aggregated to one value using the average. \code{"both"} means these
#' two values are not aggregated.
#'
#' @details
#' Note that depending on the \code{"aggregation"} argument, the implemented method may either return a single value, representing the overall similarity between some pair functions (like a distance between them),
#' or two different values, one measuring how much the first function is a subset of the second, and the another one
#' evaluating how much the second function is a subset of the first one. The user may possibly wish to aggregate these two values by some custom aggregation function.
#'
#' @return
#' If \code{returnType} is equal to "data.frame", a data frame is returned, with just one row that gives the information about the similarity of a given pair of functions.
#' Columns of the data frame are as follows:
#' \itemize{
#'      \item \code{name1} - name of the first function in a pair. Name is obtained by substitute() function
#'      \item \code{name2} - name of the second function in a pair. Name is obtained by substitute() function
#'      \item \code{SimilaR} - values from [0,1] interval returned by SimilaR code similarity algorithm. 
#'                                                                    1 means identical function, 0 means totally dissimilar functions. 
#'                                                                    If \code{aggregation} is equal "both", two columns are returned:
#'                                                                    the one with suffix "12", which means how much the first function is a subset of the second,
#'                                                                    and the another one with suffix "21" which means how much the second function is a subset of the first one
#'      \item \code{decision} - binary value, 0 or 1. 1 means that these two functions are similar, while 0 means otherwise.
#'      
#' }
#' 
#' If \code{returnType} is equal to "matrix", a square matrix is returned. (i,j) position equals a similarity measure between i-th and j-th function. 
#' When \code{aggregation} is equal to "sym" or "tnorm", the matrix is symmetric. For "both" it is not symmetric and (i,j) means how much the i-th function is a subset of the second,
#' while (j,i) means how much the j-th function is a subset of the i-th. Colnames and rownames of the matrix are names of compared functions, similarly to columns \code{name1} and 
#' \code{name2} in a dataframe. Obviously in this version of function the matrix has 2 rows and 2 columns.
#'
#' @examples
#' f1 <- function(x) {x*x}
#' f2 <- function(x,y) {x+y}
#'
#' ## A dataframe is returned: 1 row, 4 columns
#' SimilaR_fromTwoFunctions(f1,
#'                          f2,
#'                          returnType = "data.frame",
#'                          aggregation = "tnorm")
#'                          
#' ## A dataframe is returned: 1 row, 5 columns
#' SimilaR_fromTwoFunctions(f1,
#'                          f2,
#'                          returnType = "data.frame",
#'                          aggregation = "both")     
#'                          
#' ## A not symmetric square matrix is returned, 
#' ## with 2 rows and 2 columns
#' SimilaR_fromTwoFunctions(f1,
#'                          f2,
#'                          returnType = "matrix",
#'                          aggregation = "both")                     
#'                          
#' @references
#' Bartoszuk M., Ph.D. thesis, in preparation, Warsaw University of Technology, Warsaw, Poland, 2018.
#' 
#' Bartoszuk M., Gagolewski M., \emph{Binary aggregation functions in software plagiarism detection}, 
#' In: \emph{Proc. FUZZ-IEEE'17}, IEEE, 2017.
#'
#' Bartoszuk M., Beliakov G., Gagolewski M., James S., \emph{Fitting aggregation functions to data: Part II - Idempotentization},
#' In: Carvalho J.P. et al. (Eds.),
#' \emph{Information Processing and Management of Uncertainty in Knowledge-Based Systems,
#' Part II (Communications in Computer and Information Science 611)},
#' Springer, 2016, pp. 780-789. doi:10.1007/978-3-319-40581-0_63.
#' 
#' Bartoszuk M., Beliakov G., Gagolewski M., James S., \emph{Fitting aggregation functions to data: Part I - Linearization and regularization},
#' In: Carvalho J.P. et al. (Eds.), \emph{Information Processing and Management of Uncertainty in Knowledge-Based Systems, Part II 
#' (Communications in Computer and Information Science 611)},
#' Springer, 2016, pp. 767-779. doi:10.1007/978-3-319-40581-0_62.
#'
#' Bartoszuk M., Gagolewski M., \emph{Detecting similarity of R functions via a
#' fusion of multiple heuristic methods}, In: Alonso J.M., Bustince H., Reformat
#' M. (Eds.), \emph{Proc. IFSA/EUSFLAT 2015}, Atlantis Press, 2015, pp. 419-426.
#'
#' Bartoszuk M., Gagolewski M., \emph{A fuzzy R code similarity detection algorithm}, 
#' In: Laurent A. et al. (Eds.), \emph{Information Processing and Management of Uncertainty in Knowledge-Based Systems, 
#' Part III (CCIS 444)}, Springer-Verlag, Heidelberg, 2014, pp. 21-30.
#'
#' @export
SimilaR_fromTwoFunctions <- function(function1, 
                                     function2,
                                     returnType = c("data.frame","matrix"),
                                     aggregation = c("sym","tnorm", "both"))
{
  if(!is.function(function1))
    stop("The first argument is not a function.")
  if(!is.function(function2))
    stop("The second argument is not a function.")
  
  functionNames<-c(as.character(substitute(function1)), as.character(substitute(function2)))
  parsess <- list(parse(text=c(stri_paste(functionNames[[1]], "<-"), deparse(function1))),
                  parse(text=c(stri_paste(functionNames[[2]], "<-"), deparse(function2))))
  sums<-c(0,1,2)
  
  SimilaR_general(parsess, sums, functionNames, returnType, aggregation)
}
