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

SimilaR_general <- function(parsess,
                            sums,
                            functionNames,
                            returnType = c("data.frame", "matrix"),
                            aggregation = c("sym", "tnorm", "both"))
{
  kernel2_ret <- checkPlagiarism_gplag_cpp_list_kernel2_test(parsess, h=2, sums)
  kernel2_matrix <- makePrettyMatrix(kernel2_ret[[ifelse(aggregation=="sym", 2, 1)]], functionNames)
  if(aggregation=="tnorm")
  {
    kernel2_matrix <- makeMMatrix(kernel2_matrix)
  }

  if(returnType=="matrix")
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
