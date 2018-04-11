#' @title
#' SimilaR
#'
#' @description
#' An implementation of the SimilaR algorithm - a novel R code similarity evaluation algorithm based on program dependence graphs
#'
#' @param dirname path to a directory with R source files to be compared.
#' @param returnedType \code{"dataframe"} or \code{"matrix"}; indicates the output object type (see below).
#' @param fileTypes \code{"function"} or \code{"file"}; indicates which pairs of functions extracted from the source files in \code{dirname} should be compared. 
#' \code{"function"} means that every function should be compared against every other function, even if both of them
#' are defined in the same file. \code{"file"} means that only the functions defined in different files should be compared.
#' @param aggregation \code{"sym"}, \code{"tnorm"}, or \code{"both"}; specifies which model of similarity asymmetry should be used. \code{"sym"} means
#' that one value of similarity is computed. \code{"tnorm"} means that two values are obtained: one means how much
#' the first function is a subset of the second, and the other one means how much the second function is a subset of the first.
#' After that they are aggregated to one value using t-norm or average, depending on a code similarity method. \code{"both"} means these
#' two values are not aggregated.
#'
#' @details
#' Note that depending on the \code{"aggregation"} argument, the implemented method may either return a single value, representing the overall similarity between some pair functions (like a distance between them),
#' or two different values, one measuring how much the first function is a subset of the second, and the another one
#' evaluating how much the second function is a subset of the first one. The user may possibly wish to aggregate these two values by some custom aggregation function.
#'
#' @return
#' If \code{returnedType} is equal to "dataframe", a data frame is returned, where every row gives the information about the similarity of a different pair of functions.
#' Columns of the data frame are as follows:
#' \itemize{
#'      \item \code{name1} - name of the first function in a pair. Name is constructed as follows: fileName.R functionName
#'      \item \code{name2} - name of the second function in a pair. Name is constructed as follows: fileName.R functionName
#'      \item \code{SimilaR} - values from [0,1] interval returned by SimilaR code similarity algorithm. 
#'                                                                    1 means identical function, 0 means totally dissimilar functions. 
#'                                                                    If \code{aggregation} is equal "both", two columns are returned:
#'                                                                    the one with suffix "12", which means how much the first function is a subset of the second,
#'                                                                    and the another one with suffix "21" which means how much the second function is a subset of the first one
#'      \item \code{decision} - binary value, 0 or 1. 1 means that these two functions are similar, while 0 means otherwise.
#'      
#' }
#' Rows in the dataframe are sorted by column \code{SimilaR}, not increasingly.
#' 
#' If \code{returnedType} is equal to "matrix", a square matrix is returned. (i,j) position equals a similarity measure between i-th and j-th function. 
#' When \code{aggregation} is equal to "sym" or "tnorm", the matrix is symmetric. For "both" it is not symmetric and (i,j) means how much the i-th function is a subset of the second,
#' while (j,i) means how much the j-th function is a subset of the i-th. Colnames and rownames of the matrix are names of compared functions, similarly to columns \code{name1} and 
#' \code{name2} in a dataframe.
#'
#' @examples
#' \dontrun{
#' dirname = ... # path to directory with files containing function definitions to compare
#'
#' dataFrameWithAssessment <- SimilaR_fromDirectory(dirname,
#'                                                  returnedType = "dataframe",
#'                                                  fileTypes="file",
#'                                                  aggregation = "sym")
#' head(results)
#' }
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
SimilaR_fromDirectory <- function(dirname,
                    returnedType = c("dataframe","matrix"),
                    fileTypes=c("function", "file", "directory"),
                    aggregation = c("sym","tnorm", "both"))
{
  submissionType <- switch(fileTypes,
                          "function" = 1,
                          "file" = 2,
                          "directory" = 3)
  
  parsesSumsFunctionNames <- readFilesAndGetFunctionNames(dirname, submissionType=submissionType, rewriteFiles=FALSE)
  parses<-parsesSumsFunctionNames$parses
  functionNames<-parsesSumsFunctionNames$functionNames
  sums <- parsesSumsFunctionNames$sums
  parsess<- parses$parses
  
  kernel2_ret <- checkPlagiarism_gplag_cpp_list_kernel2_test(parsess, h=2, sums)
  kernel2_matrix <- makePrettyMatrix(kernel2_ret[[ifelse(aggregation=="sym", 2, 1)]], functionNames)
  if(aggregation=="tnorm")
  {
    kernel2_matrix <- makeMMatrix(kernel2_matrix)
  }
  
  if(returnedType=="matrix")
  {
    return(kernel2_matrix)
  }
  
  functionNames <- colnames(kernel2_matrix)
  
  if(aggregation=="sym" || aggregation=="tnorm")
  {
    dataFrameWithoutAssessment <- generateDataFrame_one_rcpp(
      kernel2_matrix,
      functionNames
                                          )
  }
  else if(aggregation=="both")
  {
    dataFrameWithoutAssessment <- generateDataFrame_one_both_rcpp(
      kernel2_matrix,
      functionNames
    )
  }
  
  SimilarVector <- if(aggregation=="sym" || aggregation=="tnorm"){dataFrameWithoutAssessment$SimilaR}else{dataFrameWithoutAssessment$SimilaR12 + dataFrameWithoutAssessment$SimilaR21}
  threshold <- ifelse(aggregation=="sym" || aggregation=="tnorm", 0.71, 1.42)
  
  dataFrameWithAssessment <- cbind(dataFrameWithoutAssessment,decision=ifelse(SimilarVector >= threshold, 1, 0))
  dataFrameWithAssessment <- dataFrameWithAssessment[!is.na(dataFrameWithoutAssessment[,3]),]
  SimilarVector <- SimilarVector[!is.na(dataFrameWithoutAssessment[,3])]
  dataFrameWithAssessment <- dataFrameWithAssessment[order(SimilarVector,decreasing = TRUE),]
  dataFrameWithAssessment
}
