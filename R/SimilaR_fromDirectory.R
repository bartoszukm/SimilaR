##    SimilaR package for R
##    Copyright (C) 2018-2020 by M. Bartoszuk, M. Gagolewski
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
#' Quantify the Similarity of Pairs of R Functions
#'
#' @description
#' An implementation of the SimilaR algorithm - a method to quantify
#' the similarity of R functions based on Program Dependence Graphs.
#' Possible use cases include detection of code clones for improving
#' software quality and of plagiarism among students' homework assignments.
#'
#' \code{SimilaR_fromDirectory} scans for function definitions in all \code{*.R}
#' source files in a given directory and performs pairwise comparisons.
#'
#' \code{SimilaR_fromTwoFunctions} compares the code-base of two function objects.
#'
#' @param function1 a first function object to compare
#' @param function2 a second function object to compare
#' @param functionNames optional functions' names to be included in the output
#' @param dirname path to a directory with  source files named \code{*.R}
#' @param returnType \code{"data.frame"} or \code{"matrix"}; indicates the output object type
#' @param fileTypes \code{"function"} or \code{"file"}; indicates which pairs
#' of functions extracted from the source files in \code{dirname} should be compared;
#' \code{"function"} compares each function against every other function;
#' \code{"file"} compares only the functions defined in different source files
#' @param aggregation \code{"sym"}, \code{"tnorm"}, or \code{"both"};
#' specifies which model of similarity asymmetry should be used;
#' \code{"sym"} means that one (overall) similarity degree is computed;
#' \code{"both"} evaluates and returns the degree to which the first function
#' in a function pair is similar ("contained in", "is subset of") to the
#' second one, and, separately, the extent to which the second function is
#' similar to the first one;
#' \code{"tnorm"} computes two similarity values and aggregates them
#' to a single number
#'
#' @details
#' Note that, depending on the \code{"aggregation"} argument, the
#' method may either return a single value, representing the overall
#' (symmetric) similarity between a pair of functions, or
#' or two different values, measuring the (non-symmetric) degrees of "subsethood".
#' The user might possibly wish to aggregate these two values by means of some
#' custom aggregation function.
#'
#' @return
#' If \code{returnType} is equal to "data.frame", a data frame
#' that gives the information about the similarity of the inspected
#' pairs of functions, row by row, is returned.
#' The data frame has the following columns:
#' \itemize{
#'      \item \code{name1} - the name of the first function in a pair
#'      \item \code{name2} - the name of the second function in a pair
#'      \item \code{SimilaR} - values in the [0,1] interval as returned by the SimilaR algorithm;
#'           1 denotes that the functions are equivalent, while 0 means that they are totally dissimilar;
#'           if \code{aggregation} is equal to \code{"both"}, two similarity values are given:
#'           the one with suffix \code{"12"} quantifies the degree to which the first function is a subset of the second,
#'           and the another one with suffix \code{"21"} measures the extent to which the second function is a subset of the first one
#'      \item \code{decision} - 0 or 1; 1 means that two functions are classified as similar and 0 otherwise.
#' }
#' Rows in the data frame are sorted with respect to the \code{SimilaR}
#' column (descending). Of course, \code{SimilaR_fromTwoFunctions} gives
#' a data frame with only one row.
#'
#' If \code{returnType} is equal to \code{"matrix"}, a square matrix
#' is returned. The element at index (i,j) equals to the similarity degree
#' between the i-th and the j-th function.
#' When \code{aggregation} is equal to \code{"sym"} or \code{"tnorm"},
#' the matrix is symmetric.
#' Column names and row names of the matrix are generated from the names
#' of the functions being compared.
#'
#' @examples
#' f1 <- function(x) {x*x}
#' f2 <- function(x,y) {x+y}
#'
#' ## A data frame is returned: 1 row, 4 columns
#' SimilaR_fromTwoFunctions(f1,
#'                          f2,
#'                          returnType = "data.frame",
#'                          aggregation = "tnorm")
#'
#' ## Custom names in the returned data frame
#' SimilaR_fromTwoFunctions(f1,
#'                          f2,
#'                          functionNames = c("first", "second"),
#'                          returnType = "data.frame",
#'                          aggregation = "tnorm")
#'
#' ## A data frame is returned: 1 row, 5 columns
#' SimilaR_fromTwoFunctions(f1,
#'                          f2,
#'                          returnType = "data.frame",
#'                          aggregation = "both")
#'
#' ## A non-symmetric square matrix is returned,
#' ## with 2 rows and 2 columns
#' SimilaR_fromTwoFunctions(f1,
#'                          f2,
#'                          returnType = "matrix",
#'                          aggregation = "both")
#'
#'
#' ## Typical example, where we wish to compare the functions from different files,
#' ## but we do not want to compare the functions from the same file.
#' ## There will be one value describing the overall similarity level.
#' SimilaR_fromDirectory(system.file("testdata","data",package="SimilaR"),
#'                                  returnType = "data.frame",
#'                                  fileTypes="file",
#'                                  aggregation = "sym")
#'
#' ## In this example we want to compare every pair of functions: even those
#' ## defined in the same file. Two (non-symmetric) similarity degrees
#' ## are reported.
#' SimilaR_fromDirectory(system.file("testdata","data2",package="SimilaR"),
#'                       returnType = "data.frame",
#'                       fileTypes="function",
#'                       aggregation = "both")
#'
#'
#' @references
#' Bartoszuk M., A source code similarity assessment system for functional
#' programming languages based on machine learning and data aggregation methods,
#' Ph.D. thesis, Warsaw University of Technology, Warsaw, Poland, 2018.
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
#' @family SimilaR
#' @rdname SimilaR_fromDirectory
SimilaR_fromDirectory <- function(dirname,
                    returnType = c("data.frame","matrix"),
                    fileTypes=c("function", "file"),
                    aggregation = c("tnorm", "sym", "both"))
{
  returnType <- match.arg(returnType)
  fileTypes <- match.arg(fileTypes)
  aggregation <- match.arg(aggregation)

  submissionType <- switch(fileTypes,
                          "function" = 1,
                          "file" = 2,
                          "directory" = 3)

  parsesSumsFunctionNames <- readFilesAndGetFunctionNames(dirname, submissionType=submissionType, rewriteFiles=FALSE)
  parses<-parsesSumsFunctionNames$parses
  functionNames<-parsesSumsFunctionNames$functionNames
  sums <- parsesSumsFunctionNames$sums
  parsess<- parses$parses

  SimilaR_general(parsess, sums, functionNames, returnType, aggregation)
}





#' @export
#' @rdname SimilaR_fromDirectory
SimilaR_fromTwoFunctions <- function(function1,
                                     function2,
                                     functionNames,
                                     returnType = c("data.frame","matrix"),
                                     aggregation = c("tnorm", "sym", "both")
                                     )
{
  if(!is.function(function1))
    stop("The first argument is not a function.")
  if(!is.function(function2))
    stop("The second argument is not a function.")
  if(!missing(functionNames))
  {
    stopifnot(is.character(functionNames))
    stopifnot(length(functionNames) == 2)
    functionNames <- make.names(functionNames)
  }

  returnType <- match.arg(returnType)
  aggregation <- match.arg(aggregation)

  if(missing(functionNames))
  {
    functionNames<-c(make.names(deparse(substitute(function1))), make.names(deparse(substitute(function2))))
  }

  parsess <- list(parse(text=c(stri_paste(functionNames[[1]], "<-"), deparse(function1))),
                  parse(text=c(stri_paste(functionNames[[2]], "<-"), deparse(function2))))

  sums<-c(0,1,2)

  SimilaR_general(parsess, sums, functionNames, returnType, aggregation)
}
