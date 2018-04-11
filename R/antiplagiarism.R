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
