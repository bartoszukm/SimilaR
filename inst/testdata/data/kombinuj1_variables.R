 kombinuj1 <- function(l_65731)
{
   stopifnot(is.list(l_65731) && length(l_65731) > 0)
   stopifnot(any(lapply(l_65731,is.numeric) == 1))
   stopifnot(length(l_65731[[1]]) > 0)
   stopifnot(all(lapply(l_65731, length) == length(l_65731[[1]])) )
   
   n_65731 <- length(l_65731[[1]])
   k_65731 <- length(l_65731)
   new_65731 <- vector_65731("list", n)
   
   for (i_65731 in 1:n_65731)
      for (j_65731 in 1:k_65731)
         new_65731[[i_65731]][j_65731] <- l_65731[[j_65731]][i_65731]
   
   new_65731
} 
