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

library("testthat")
library("SimilaR")
context("algorithm")

test_that("return", {
  scaling <- function(x)
  {
    stopifnot(is.numeric(x))
    stopifnot(length(x) > 1)
    
    if(abs(max(x)-min(x)) < 1e-5)
      NULL
    else
    {
      y <- (x - min(x))/(max(x)-min(x))
      y
    }
  }
  
  scaling2 <- function(x)
  {
    stopifnot(is.numeric(x))
    stopifnot(length(x) > 1)
    
    if(abs(max(x)-min(x)) < 1e-5)
      return(NULL)
    else
    {
      y <- (x - min(x))/(max(x)-min(x))
      return(y)
    }
  }
  
  res <- SimilaR_fromTwoFunctions(scaling, 
                                  scaling2, returnType = "data.frame", aggregation = "both")
  
  expect_true(is.data.frame(res))
  expect_equal(res[1, 3], 1)
  expect_equal(res[1, 4], 1)
  expect_equal(res[1, 5], 1)
})

test_that("aliases", {
  scaling <- function(x)
  {
    stopifnot(is.numeric(x))
    stopifnot(length(x) > 1)
    
    if(abs(max(x)-min(x)) < 1e-5)
      NULL
    else
    {
      y <- (x - min(x))/(max(x)-min(x))
      y
    }
  }
  
  scaling2 <- function(x)
  {
    longName <- x
    longName2 <- min
    stopifnot(is.numeric(longName))
    stopifnot(length(longName) > 1)
    
    if(abs(max(longName)-longName2(longName)) < 1e-5)
      NULL
    else
    {
      y <- (longName - longName2(longName))/(max(longName)-longName2(longName))
      y
    }
  }
  
  res <- SimilaR_fromTwoFunctions(scaling, 
                                  scaling2, returnType = "data.frame", aggregation = "both")
  
  expect_true(is.data.frame(res))
  expect_equal(res[1, 3], 1)
  expect_equal(res[1, 4], 1)
  expect_equal(res[1, 5], 1)
})

test_that("function_combining", {
  scaling <- function(x)
  {
    stopifnot(is.numeric(x))
    stopifnot(length(x) > 1)
    
    if(abs(max(x)-min(x)) < 1e-5)
      NULL
    else
    {
      y <- (x - min(x))/(max(x)-min(x))
      y
    }
  }
  
  scaling2 <- function(x)
  {
    stopifnot(is.numeric(x))
    
    len_x <- length(x) 
    is_len_x_greater_1 = len_x > 1
    stopifnot(is_len_x_greater_1)
    
    
    max_x <- max(x)
    min_x <- min(x)
    abs_max_min <- abs(max_x-min_x)
    if(abs_max_min < 1e-5)
      NULL
    else
    {
      y <- (x - min(x))/(max(x)-min(x))
      y
    }
  }
  
  res <- SimilaR_fromTwoFunctions(scaling, 
                                  scaling2, returnType = "data.frame", aggregation = "both")
  
  expect_true(is.data.frame(res))
  expect_equal(res[1, 3], 1)
  expect_equal(res[1, 4], 1)
  expect_equal(res[1, 5], 1)
})

test_that("multiple_calls", {
  scaling <- function(x)
  {
    stopifnot(is.numeric(x))
    stopifnot(length(x) > 1)
    
    if(abs(max(x)-min(x)) < 1e-5)
      NULL
    else
    {
      y <- (x - min(x))/(max(x)-min(x))
      y
    }
  }
  
  
  scaling2 <- function(x)
  {
    stopifnot(is.numeric(x))
    len_x <- length(x)
    stopifnot(len_x > 1)
    
    min_x <- min(x)
    max_x <- max(x)
    
    if(abs(max_x-min_x) < 1e-5)
      NULL
    else
    {
      y <- (x - min_x)/(max_x-min_x)
      y
    }
  }
  
  res <- SimilaR_fromTwoFunctions(scaling, 
                           scaling2, returnType = "data.frame", aggregation = "both")
  
  expect_true(is.data.frame(res))
  expect_equal(res[1, 3], 1)
  expect_equal(res[1, 4], 1)
  expect_equal(res[1, 5], 1)
})

test_that("stopifnot", {
  scaling <- function(x)
  {
    stopifnot(is.numeric(x))
    stopifnot(length(x) > 1)
    
    if(abs(max(x)-min(x)) < 1e-5)
      NULL
    else
    {
      y <- (x - min(x))/(max(x)-min(x))
      y
    }
  }
  
  scaling2 <- function(x)
  {
    stopifnot(is.numeric(x), length(x) > 1)
    
    if(abs(max(x)-min(x)) < 1e-5)
      NULL
    else
    {
      y <- (x - min(x))/(max(x)-min(x))
      y
    }
  }
  
  scaling3 <- function(x)
  {
    stopifnot(is.numeric(x) && length(x) > 1)
    
    if(abs(max(x)-min(x)) < 1e-5)
      NULL
    else
    {
      y <- (x - min(x))/(max(x)-min(x))
      y
    }
  }
  
  res <- rbind(
    SimilaR_fromTwoFunctions(scaling, 
                             scaling2, returnType = "data.frame", aggregation = "both"),
    SimilaR_fromTwoFunctions(scaling, 
                             scaling3, returnType = "data.frame", aggregation = "both"),
    SimilaR_fromTwoFunctions(scaling2, 
                             scaling3, returnType = "data.frame", aggregation = "both"))
  
  expect_true(is.data.frame(res))
  expect_equal(res[1, 3], 1)
  expect_equal(res[1, 4], 1)
  expect_equal(res[1, 5], 1)
  expect_equal(res[2, 3], 1)
  expect_equal(res[2, 4], 1)
  expect_equal(res[2, 5], 1)
  expect_equal(res[3, 3], 1)
  expect_equal(res[3, 4], 1)
  expect_equal(res[3, 5], 1)
})

test_that("order", {
  scaling <- function(x)
  {
    stopifnot(is.numeric(x))
    stopifnot(length(x) > 1)
    
    if(abs(max(x)-min(x)) < 1e-5)
      NULL
    else
    {
      y <- (x - min(x))/(max(x)-min(x))
      y
    }
  }
  
  scaling2 <- function(x)
  {
    stopifnot(length(x) > 1)
    stopifnot(is.numeric(x))
    
    if(abs(max(x)-min(x)) >= 1e-5)
    {
      y <- (x - min(x))/(max(x)-min(x))
      y
    }
    else
      NULL
  }
  
  res <- SimilaR_fromTwoFunctions(scaling, 
                                  scaling2, returnType = "data.frame", aggregation = "both")
  
  expect_true(is.data.frame(res))
  expect_equal(res[1, 3], 1)
  expect_equal(res[1, 4], 1)
  expect_equal(res[1, 5], 1)
})

test_that("dead_code", {
  scaling <- function(x)
  {
    stopifnot(is.numeric(x))
    stopifnot(length(x) > 1)
    
    if(abs(max(x)-min(x)) < 1e-5)
      NULL
    else
    {
      y <- (x - min(x))/(max(x)-min(x))
      y
    }
  }
  
  scaling2 <- function(x)
  {
    stopifnot(is.numeric(x))
    stopifnot(length(x) > 1)
    
    z1 <- runif(100, -5, 5)
    z2 <- 2*z1+3+rnorm(100, 0, 0.2)
    
    beta <- sum((z1 - mean(z1))*(z2-mean(z2)))/(sum((z1-mean(z1))^2))
    alpha <- mean(z2) - beta*mean(z1)
    
    if(abs(max(x)-min(x)) < 1e-5)
    {
      NULL
    }
    else
    {
      y <- (x - min(x))/(max(x)-min(x))
      y
    }
  }
  
  res <- SimilaR_fromTwoFunctions(scaling, 
                           scaling2, returnType = "data.frame", aggregation = "both")
  
  expect_true(is.data.frame(res))
  expect_equal(res[1, 3], 1)
  expect_equal(res[1, 4], 1)
  expect_equal(res[1, 5], 1)
})


test_that("magrittr1", {
  f1 <- function(x)
  {
    sum(min(x), max(x))
  }
  
  f2 <- function(x)
  {
    x %>% min() %>% sum(max(x))
  }
  
  
  res <- SimilaR_fromTwoFunctions(f1, 
                                  f2, returnType = "data.frame", aggregation = "both")
  
  expect_true(is.data.frame(res))
  expect_equal(res[1, 3], 1)
  expect_equal(res[1, 4], 1)
  expect_equal(res[1, 5], 1)
})

test_that("magrittr2", {
  f1 <- function(x)
  {
    sum(min(x), max(x))
  }
  
  f2 <- function(x)
  {
    x %>% min %>% sum(max(x))
  }
  
  
  res <- SimilaR_fromTwoFunctions(f1, 
                                  f2, returnType = "data.frame", aggregation = "both")
  
  expect_true(is.data.frame(res))
  expect_equal(res[1, 3], 1)
  expect_equal(res[1, 4], 1)
  expect_equal(res[1, 5], 1)
})

test_that("magrittr3", {
  f1 <- function(x)
  {
     stopifnot(length(x) > 0, sum(x) > 0)
     sum(min(x), max(x))
  }
  
  f2 <- function(x)
  {
    (length(x) > 0) %>% stopifnot
    stopifnot(sum(x) > 0)
    x %>% min %>% sum(max(x))
  }
  
  f3 <- function(x)
  {
    (length(x) > 0) %>% stopifnot()
    stopifnot(sum(x) > 0)
    x %>% min %>% sum(max(x))
  }
  
  res <- SimilaR_fromTwoFunctions(f1, 
                                  f2, returnType = "data.frame", aggregation = "both")
  expect_true(is.data.frame(res))
  expect_equal(res[1, 3], 1)
  expect_equal(res[1, 4], 1)
  expect_equal(res[1, 5], 1)
  
  res <- SimilaR_fromTwoFunctions(f1, 
                                  f3, returnType = "data.frame", aggregation = "both")
  expect_true(is.data.frame(res))
  expect_equal(res[1, 3], 1)
  expect_equal(res[1, 4], 1)
  expect_equal(res[1, 5], 1)
  
  res <- SimilaR_fromTwoFunctions(f2, 
                                  f3, returnType = "data.frame", aggregation = "both")
  expect_true(is.data.frame(res))
  expect_equal(res[1, 3], 1)
  expect_equal(res[1, 4], 1)
  expect_equal(res[1, 5], 1)
})

test_that("stopifnot1", {
  f1 <- function(x)
  {
    stopifnot(length(x) > 0, sum(x) > 0)
    sum(min(x), max(x))
  }
  
  f2 <- function(x)
  {
    stopifnot(length(x) > 0)
    stopifnot(sum(x) > 0)
    x %>% min %>% sum(max(x))
  }
  
  f3 <- function(x)
  {
    stopifnot(length(x) > 0 && sum(x) > 0)
    x %>% min %>% sum(max(x))
  }
  
  f4 <- function(x)
  {
    stopifnot(length(x) > 0 & sum(x) > 0)
    x %>% min %>% sum(max(x))
  }
  
  res <- SimilaR_fromTwoFunctions(f1, 
                                  f2, returnType = "data.frame", aggregation = "both")
  expect_true(is.data.frame(res))
  expect_equal(res[1, 3], 1)
  expect_equal(res[1, 4], 1)
  expect_equal(res[1, 5], 1)
  
  res <- SimilaR_fromTwoFunctions(f2, 
                                  f3, returnType = "data.frame", aggregation = "both")
  expect_true(is.data.frame(res))
  expect_equal(res[1, 3], 1)
  expect_equal(res[1, 4], 1)
  expect_equal(res[1, 5], 1)
  
  res <- SimilaR_fromTwoFunctions(f3, 
                                  f4, returnType = "data.frame", aggregation = "both")
  expect_true(is.data.frame(res))
  expect_equal(res[1, 3], 1)
  expect_equal(res[1, 4], 1)
  expect_equal(res[1, 5], 1)
})

test_that("magrittr4", {
  f1 <- function(x)
  {
    a <- min(x)
    y + 5+a
    z <- 6-a
    y-6+z
  }
  
  f2 <- function(x)
  {
    a <- x %>% min
    y + 5+a
    z <- 6-a
    y-6+z
  }
  
  
  res <- SimilaR_fromTwoFunctions(f1, 
                                  f2, returnType = "data.frame", aggregation = "both")
  res
  
  expect_true(is.data.frame(res))
  expect_equal(res[1, 3], 1)
  expect_equal(res[1, 4], 1)
  expect_equal(res[1, 5], 1)
})

test_that("magrittr5", {
  f1 <- function(x)
  {
    return(x-min(x))
  }
  
  f2 <- function(x)
  {
    x-min(x)
  }
  
  f3 <- function(x)
  {
    (x-min(x)) %>% return
  }
  
  
  res <- SimilaR_fromTwoFunctions(f1, 
                                  f2, returnType = "data.frame", aggregation = "both")
  expect_true(is.data.frame(res))
  expect_equal(res[1, 3], 1)
  expect_equal(res[1, 4], 1)
  expect_equal(res[1, 5], 1)
  
  res <- SimilaR_fromTwoFunctions(f2, 
                                  f3, returnType = "data.frame", aggregation = "both")
  expect_true(is.data.frame(res))
  expect_equal(res[1, 3], 1)
  expect_equal(res[1, 4], 1)
  expect_equal(res[1, 5], 1)
  
  res <- SimilaR_fromTwoFunctions(f1, 
                                  f3, returnType = "data.frame", aggregation = "both")
  expect_true(is.data.frame(res))
  expect_equal(res[1, 3], 1)
  expect_equal(res[1, 4], 1)
  expect_equal(res[1, 5], 1)
})

test_that("return1", {
  f1 <- function(x)
  {
    return(x-min(x))
  }
  
  f2 <- function(x)
  {
    x-min(x)
  }
  
  f3 <- function(x)
  {
    y <- x-min(x)
    return(y)
  }
  
  f4 <- function(x)
  {
    y <- x-min(x)
    y
  }
  
  
  res <- SimilaR_fromTwoFunctions(f1, 
                                  f2, returnType = "data.frame", aggregation = "both")
  expect_true(is.data.frame(res))
  expect_equal(res[1, 3], 1)
  expect_equal(res[1, 4], 1)
  expect_equal(res[1, 5], 1)
  
  res <- SimilaR_fromTwoFunctions(f2, 
                                  f3, returnType = "data.frame", aggregation = "both")
  expect_true(is.data.frame(res))
  expect_equal(res[1, 3], 1)
  expect_equal(res[1, 4], 1)
  expect_equal(res[1, 5], 1)
  
  res <- SimilaR_fromTwoFunctions(f1, 
                                  f3, returnType = "data.frame", aggregation = "both")
  expect_true(is.data.frame(res))
  expect_equal(res[1, 3], 1)
  expect_equal(res[1, 4], 1)
  expect_equal(res[1, 5], 1)
  
  res <- SimilaR_fromTwoFunctions(f1, 
                                  f4, returnType = "data.frame", aggregation = "both")
  expect_true(is.data.frame(res))
  expect_equal(res[1, 3], 1)
  expect_equal(res[1, 4], 1)
  expect_equal(res[1, 5], 1)
})

test_that("return2", {
  f1 <- function(x)
  {
    if(x > 0)
      return(x-min(x))
    else
      return(-x+min(x))
  }
  
  f2 <- function(x)
  {
    if(x > 0)
      x-min(x)
    else
      -x+min(x)
  }
  
  f3 <- function(x)
  {
    if(x > 0)
    {
      y <- x-min(x)
      return(y)
    }
    else
    {
      y <- -x+min(x)
      return(y)
    }
  }
  
  
  res <- SimilaR_fromTwoFunctions(f1, 
                                  f2, returnType = "data.frame", aggregation = "both")
  expect_true(is.data.frame(res))
  expect_equal(res[1, 3], 1)
  expect_equal(res[1, 4], 1)
  expect_equal(res[1, 5], 1)
  
  res <- SimilaR_fromTwoFunctions(f2, 
                                  f3, returnType = "data.frame", aggregation = "both")
  expect_true(is.data.frame(res))
  expect_equal(res[1, 3], 1)
  expect_equal(res[1, 4], 1)
  expect_equal(res[1, 5], 1)
  
  res <- SimilaR_fromTwoFunctions(f1, 
                                  f3, returnType = "data.frame", aggregation = "both")
  expect_true(is.data.frame(res))
  expect_equal(res[1, 3], 1)
  expect_equal(res[1, 4], 1)
  expect_equal(res[1, 5], 1)
})

test_that("return3", {
  f1 <- function(x)
  {
    x %>% min()
  }
  
  f2 <- function(x)
  {
    min(x)
  }
  
  f3 <- function(x)
  {
    y <- x
    y %>% min
  }
  
  f4 <- function(x)
  {
    y <- x+6
    sum(y, 6)
  }
  
  
  res <- SimilaR_fromTwoFunctions(f1, 
                                  f2, returnType = "data.frame", aggregation = "both")
  expect_true(is.data.frame(res))
  expect_equal(res[1, 3], 1)
  expect_equal(res[1, 4], 1)
  expect_equal(res[1, 5], 1)
  
  res <- SimilaR_fromTwoFunctions(f2, 
                                  f3, returnType = "data.frame", aggregation = "both")
  expect_true(is.data.frame(res))
  expect_equal(res[1, 3], 1)
  expect_equal(res[1, 4], 1)
  expect_equal(res[1, 5], 1)
  
  res <- SimilaR_fromTwoFunctions(f1, 
                                  f3, returnType = "data.frame", aggregation = "both")
  expect_true(is.data.frame(res))
  expect_equal(res[1, 3], 1)
  expect_equal(res[1, 4], 1)
  expect_equal(res[1, 5], 1)
  
  res <- SimilaR_fromTwoFunctions(f1, 
                                  f4, returnType = "data.frame", aggregation = "both")
  expect_true(is.data.frame(res))
  expect_lt(res[1, 3], 1)
  expect_lt(res[1, 4], 1)
  expect_lt(res[1, 5], 1)
})


test_that("negation1", {
  f1 <- function(x)
  {
    if(x > 0)
      x-min(x)
    else
      -x+min(x)
  }
  
  f2 <- function(x)
  {
    if(x <= 0)
      -x+min(x)
    else
      x-min(x)
  }
  
  f3 <- function(x)
  {
    if(!(x > 0))
      -x+min(x)
    else
      x-min(x)
  }
  
  res <- SimilaR_fromTwoFunctions(f1, 
                                  f2, returnType = "data.frame", aggregation = "both")
  expect_true(is.data.frame(res))
  expect_equal(res[1, 3], 1)
  expect_equal(res[1, 4], 1)
  expect_equal(res[1, 5], 1)
  
  res <- SimilaR_fromTwoFunctions(f2, 
                                  f3, returnType = "data.frame", aggregation = "both")
  expect_true(is.data.frame(res))
  expect_equal(res[1, 3], 1)
  expect_equal(res[1, 4], 1)
  expect_equal(res[1, 5], 1)
  
  res <- SimilaR_fromTwoFunctions(f1, 
                                  f3, returnType = "data.frame", aggregation = "both")
  expect_true(is.data.frame(res))
  expect_equal(res[1, 3], 1)
  expect_equal(res[1, 4], 1)
  expect_equal(res[1, 5], 1)
})

test_that("negation2", {
  f1 <- function(x, y)
  {
    if(x > 0 && y < 0)
      x-min(x)
    else
      -x+min(x)
  }
  
  f2 <- function(x, y)
  {
    if(x <= 0 || y >= 0)
      -x+min(x)
    else
      x-min(x)
  }
  
  f3 <- function(x, y)
  {
    if(!(x > 0 && y < 0))
      -x+min(x)
    else
      x-min(x)
  }
  
  f4 <- function(x, y)
  {
    if(x <= 0 || !(y < 0))
      -x+min(x)
    else
      x-min(x)
  }
  
  res <- SimilaR_fromTwoFunctions(f1, 
                                  f2, returnType = "data.frame", aggregation = "both")
  expect_true(is.data.frame(res))
  expect_equal(res[1, 3], 1)
  expect_equal(res[1, 4], 1)
  expect_equal(res[1, 5], 1)
  
  res <- SimilaR_fromTwoFunctions(f2, 
                                  f3, returnType = "data.frame", aggregation = "both")
  expect_true(is.data.frame(res))
  expect_equal(res[1, 3], 1)
  expect_equal(res[1, 4], 1)
  expect_equal(res[1, 5], 1)
  
  res <- SimilaR_fromTwoFunctions(f1, 
                                  f3, returnType = "data.frame", aggregation = "both")
  expect_true(is.data.frame(res))
  expect_equal(res[1, 3], 1)
  expect_equal(res[1, 4], 1)
  expect_equal(res[1, 5], 1)
  
  res <- SimilaR_fromTwoFunctions(f2, 
                                  f4, returnType = "data.frame", aggregation = "both")
  expect_true(is.data.frame(res))
  expect_equal(res[1, 3], 1)
  expect_equal(res[1, 4], 1)
  expect_equal(res[1, 5], 1)
})

test_that("negation3", {
  f1 <- function(x, y)
  {
    if(x > 0 && y < 0)
      x-min(x)
    else
      -x+min(x)
  }
  
  f2 <- function(x, y)
  {
    if(x <= 0 || predykat(y))
      -x+min(x)
    else
      x-min(x)
  }
  
  f3 <- function(x, y)
  {
    if(x <= 0)
      -x+min(x)
    else
      x-min(x)
  }
  
  
  
  res <- SimilaR_fromTwoFunctions(f1, 
                                  f2, returnType = "data.frame", aggregation = "both")
  expect_true(is.data.frame(res))
  expect_lt(res[1, 3], 1)
  expect_lt(res[1, 4], 1)
  
  res <- SimilaR_fromTwoFunctions(f2, 
                                  f3, returnType = "data.frame", aggregation = "both")
  expect_true(is.data.frame(res))
  expect_lt(res[1, 3], 1)
  expect_lt(res[1, 4], 1)
  
  res <- SimilaR_fromTwoFunctions(f1, 
                                  f3, returnType = "data.frame", aggregation = "both")
  expect_true(is.data.frame(res))
  expect_lt(res[1, 3], 1)
  expect_lt(res[1, 4], 1)

})


test_that("negation4", {
  f1 <- function(x, y, z)
  {
    if(x > 0 && y < 0 && z > 0)
      x-min(x)
    else
      -x+min(x)
  }
  
  f2 <- function(x, y, z)
  {
    if(x <= 0 || !(y<0) || z <= 0)
      -x+min(x)
    else
      x-min(x)
  }
  
  f3 <- function(x, y)
  {
    if(x <= 0  || y >= 0)
      -x+min(x)
    else
      x-min(x)
  }
  
  res <- SimilaR_fromTwoFunctions(f1, 
                                  f2, returnType = "data.frame", aggregation = "both")
  expect_true(is.data.frame(res))
  expect_equal(res[1, 3], 1)
  expect_equal(res[1, 4], 1)
  expect_equal(res[1, 5], 1)
  
  res <- SimilaR_fromTwoFunctions(f2, 
                                  f3, returnType = "data.frame", aggregation = "both")
  expect_true(is.data.frame(res))
  expect_lt(res[1, 3], 1)
  expect_lt(res[1, 4], 1)
  
  res <- SimilaR_fromTwoFunctions(f1, 
                                  f3, returnType = "data.frame", aggregation = "both")
  expect_true(is.data.frame(res))
  expect_lt(res[1, 3], 1)
  expect_lt(res[1, 4], 1)
  
})


test_that("ifs1", {
  f0 <- function(x)
  {
    if(x >= 0)
      x
     else 
       -x
  }
  
  f1 <- function(x)
  {
    if(x >= 0)
      return(x)
    else
      return(-x)
  }
  
  f2 <- function(x)
  {
    if(x >= 0)
      return(x)
    return(-x)
  }
  
  f3 <- function(x)
  {
    if(x >= 0)
      return(x)
    -x
  }
  
  funs <- list(f0, f1, f2, f3)
  
  res <- NULL
  for (i in 1:(length(funs)-1))
    for (j in (i+1):length(funs))
    {
      res <- rbind(res, SimilaR_fromTwoFunctions(funs[[i]], 
                                                 funs[[j]],
                                                 functionNames=as.character(c(i,j)),
                                                 aggregation="both"))
      
    }
  expect_true(is.data.frame(res))
  expect_equal(res[, 3], rep(1, nrow(res)))
  expect_equal(res[, 4], rep(1, nrow(res)))
  expect_equal(res[, 5], rep(1, nrow(res)))
  
#   fun_text1 <- "f0 <- function(x)
#   {
#   if(x >= 0)
#   x
#   else
#   -x
# }"
# 
#   fun_text2 <- "f3 <- function(x)
#   {
#   if(x >= 0)
#   return(x)
#   else
#   return(-x)
#   }"
# 
#   fun_text3 <- "f2 <- function(x)
#   {
#   if(x<0)
#   {
#     a <- f(x, h(x))
#     g(a)
#   }
#   else
#   {
#     for(i in 1:5)
#     {
#       print(i)
#       print(i)
#     }
#   }
#   }"
# 
# 
#   SimilaR::SimilaR_drawGraph(fun_text1)
#   SimilaR::SimilaR_drawGraph(fun_text2)
#   SimilaR::SimilaR_drawGraph(fun_text3)
# 
#   fun_tree1 <- parse(text=fun_text1)
#   AST <- fun_tree1[[1]]
#   g1 <- createGraphFromMatrix(makePDG_cpp(AST,FALSE,TRUE))
#   plot(g1)
})

test_that("ifs2", {
  f1 <- function(x, y)
  {
    if(y < 0)
    {
      if(x < 0)
        return(-x)
      else
        return(x)
    }
    else
      return(y)
  }
  
  f2 <- function(x, y)
  {
    if(y < 0)
    {
      return(y)  
    }
    
    if(x < 0)
      return(-x)
    else
      return(x)
  }
  
  f3 <- function(x, y)
  {
    if(y < 0)
    {
      return(y)
      
    }
    if(x >= 0)
      return(x)
    return(-x)
    
  }
  
  res <- SimilaR_fromTwoFunctions(f1, 
                                  f2, returnType = "data.frame", aggregation = "both")
  expect_true(is.data.frame(res))
  expect_equal(res[1, 3], 1)
  expect_equal(res[1, 4], 1)
  expect_equal(res[1, 5], 1)
  
  res <- SimilaR_fromTwoFunctions(f2, 
                                  f3, returnType = "data.frame", aggregation = "both")
  expect_true(is.data.frame(res))
  expect_equal(res[1, 3], 1)
  expect_equal(res[1, 4], 1)
  expect_equal(res[1, 5], 1)
  
  res <- SimilaR_fromTwoFunctions(f1, 
                                  f3, returnType = "data.frame", aggregation = "both")
  expect_true(is.data.frame(res))
  expect_equal(res[1, 3], 1)
  expect_equal(res[1, 4], 1)
  expect_equal(res[1, 5], 1)
  
})

test_that("ifs3", {
  f0 <- function(x)
  {
    if(warunek1(x))
      return(f(x))
    
    if(warunek2(x))
      return(x-2)
    
    return(x+3)
  }
  
  f1 <- function(x)
  {
    if(warunek2(x))
      return(x-2)
    
    if(warunek1(x))
      return(f(x))
    
    return(x+3)
  }
  
  f2 <- function(x)
  {
    if(warunek2(x))
      return(x-2)
    else
    {
      if(warunek1(x))
        return(f(x))
      else
        return(x+3)
    }
    
  }
  
  f3 <- function(x)
  {
    if(!warunek2(x))
    {
      if(warunek1(x))
        return(f(x))
      else
        return(x+3)
    }
    else
    {
      return(x-2)
    }
  }
  
  f4 <- function(x)
  {
    if(!warunek2(x))
    {
      if(warunek1(x))
        f(x)
      else
        x+3
    }
    else
    {
      x-2
    }
  }
  
  funs <- list(f0, f1, f2, f3, f4)
  
  res <- NULL
  for (i in 1:(length(funs)-1))
    for (j in (i+1):length(funs))
    {
      res <- rbind(res, SimilaR_fromTwoFunctions(funs[[i]], 
                                                 funs[[j]],
                                                 functionNames=as.character(c(i,j)),
                                                 aggregation="both"))
      
    }
  expect_true(is.data.frame(res))
  expect_equal(res[, 3], rep(1, nrow(res)))
  expect_equal(res[, 4], rep(1, nrow(res)))
  expect_equal(res[, 5], rep(1, nrow(res)))
})

test_that("ifs4", {
  f0 <- function(x)
  {
    if(warunek1(x))
      stop("asd")
    
    if(warunek2(x))
      stop("dsa")
    
    return(x+3)
  }
  
  f1 <- function(x)
  {
    if(warunek2(x))
      stop("dsa")
    
    if(warunek1(x))
      stop("asd")
    
    return(x+3)
  }
  
  f2 <- function(x)
  {
    if(warunek2(x))
      stop("dsa")
    else
    {
      if(warunek1(x))
        stop("asd")
      else
        return(x+3)
    }
    
  }
  
  f3 <- function(x)
  {
    if(!warunek2(x))
    {
      if(warunek1(x))
        stop("asd")
      else
        return(x+3)
    }
    else
    {
      stop("dsa")
    }
  }
  
  funs <- list(f0, f1, f2, f3)
  
  res <- NULL
  for (i in 1:(length(funs)-1))
    for (j in (i+1):length(funs))
    {
      res <- rbind(res, SimilaR_fromTwoFunctions(funs[[i]], 
                                                 funs[[j]],
                                                 functionNames=as.character(c(i,j)),
                                                 aggregation="both"))
      
    }
  expect_true(is.data.frame(res))
  expect_equal(res[, 3], rep(1, nrow(res)))
  expect_equal(res[, 4], rep(1, nrow(res)))
  expect_equal(res[, 5], rep(1, nrow(res)))
})

test_that("ifs5", {
  f0 <- function(x)
  {
    if(warunek1(x))
      print("asd")
    
    if(warunek2(x))
      print("dsa")
    
    return(x+3)
  }
  
  f1 <- function(x)
  {
    if(warunek2(x))
      print("dsa")
    
    if(warunek1(x))
      print("asd")
    
    return(x+3)
  }
  
  f2 <- function(x)
  {
    if(warunek2(x))
      print("dsa")
    else
    {
      if(warunek1(x))
        print("asd")
      else
        return(x+3)
    }
    
  }
  
  f3 <- function(x)
  {
    if(!warunek2(x))
    {
      if(warunek1(x))
        print("asd")
      else
        return(x+3)
    }
    else
    {
      print("dsa")
    }
  }
  
  funs <- list(f0, f1, f2, f3)
  
  res <- NULL
  for (i in 1:(length(funs)-1))
    for (j in (i+1):length(funs))
    {
      res <- rbind(res, SimilaR_fromTwoFunctions(funs[[i]], 
                                                 funs[[j]],
                                                 functionNames=as.character(c(i,j)),
                                                 aggregation="both"))
      
    }
  expect_true(is.data.frame(res))
  expect_equal(sum(res[2:5, 3]< rep(1, nrow(res)-2)), length(rep(1, nrow(res)-2)))
  expect_equal(sum(res[2:5, 4]< rep(1, nrow(res)-2)), length(rep(1, nrow(res)-2)))
  expect_equal(sum(res[2:5, 5]< rep(1, nrow(res)-2)), length(rep(1, nrow(res)-2)))
})

test_that("loops1", {
  f1 <- function(x)
  {
    ret <- lapply(x, function(y){y+5})
    return(ret)
  }
  
  f2 <- function(x)
  {
    ret <- vector("list", length(x))
    for (i in 1:length(x))
      ret[[i]] <- x[[i]]+5
    return(ret)
  }
  
  f3 <- function(x)
  {
    ret <- vector("list", length(x))
    i <- 1
    while (i <= length(x))
    {
      ret[[i]] <- x[[i]]+5
      i <- i + 1
    }
    return(ret)
  }
  
  funs <- list(f1, f2, f3)
  
  res <- NULL
  for (i in 1:(length(funs)-1))
    for (j in (i+1):length(funs))
    {
      res <- rbind(res, SimilaR_fromTwoFunctions(funs[[i]], 
                                                 funs[[j]],
                                                 functionNames=as.character(c(i,j)),
                                                 aggregation="both"))
      
    }
  expect_true(is.data.frame(res))
  expect_equal(sum(res[1:3, 3] == rep(1, nrow(res))), length(rep(1, nrow(res))))
  expect_equal(sum(res[1:3, 4] == rep(1, nrow(res))), length(rep(1, nrow(res))))
  expect_equal(sum(res[1:3, 5] == rep(1, nrow(res))), length(rep(1, nrow(res))))

})

test_that("loops2", {
  
  f1 <- function(x)
  {
    s <- 0
    i <- 1
    while(i <= length(x))
    {
      s<- s + x[[i]]  
      i <- i + 1
    }
    s
  }
  
  f2 <- function(x)
  {
    s <- 0
    for(i in 1:length(x))
    {
      s<- s + x[[i]]  
    }
    s
  }
  
  
  res <- SimilaR_fromTwoFunctions(f1, 
                                  f2, returnType = "data.frame", aggregation = "both")
  expect_true(is.data.frame(res))
  expect_equal(res[1, 3], 1)
  expect_equal(res[1, 4], 1)
  expect_equal(res[1, 5], 1)
  
})

test_that("loops3", {
  
  f1 <- function(x, y)
  {
    ret <- vector("list", length(x))
    for(i in 1:length(x))
    {
      ret[[i]] <- x[[i]] + y[[i]]
    } 
    ret
  }
  
  f2 <- function(x, y)
  {
    ret <- vector("list", length(x))
    i <- 1
    while(i <= length(x))
    {
      ret[[i]] <- x[[i]] + y[[i]]
      i <- i + 1
    } 
    ret
  }
  
  f3 <- function(x, y)
  {
    mapply(function(a,b) {a+b}, x, y)
  }
  
  funs <- list(f1, f2, f3)
  
  res <- NULL
  for (i in 1:(length(funs)-1))
    for (j in (i+1):length(funs))
    {
      res <- rbind(res, SimilaR_fromTwoFunctions(funs[[i]], 
                                                 funs[[j]],
                                                 functionNames=as.character(c(i,j)),
                                                 aggregation="both"))
      
    }
  expect_true(is.data.frame(res))
  expect_equal(sum(res[1:3, 3] >= rep(0.9, nrow(res))), length(rep(1, nrow(res))))
  expect_equal(sum(res[1:3, 4] >= rep(0.9, nrow(res))), length(rep(1, nrow(res))))
  expect_equal(sum(res[1:3, 5] == rep(1, nrow(res))), length(rep(1, nrow(res))))
  
})


test_that("loops4", {
  
  f1 <- function(x)
  {
    ret <- vector("list", length(x))
    for(i in 1:length(x))
    {
      if(x[[i]] %% 2 == 1)
      {
        d <- exp(x[[i]])
        ret[[i]] <- 6 + d
      }
      else
      {
        ret[[i]] <- log(x[[i]])
      }
    } 
    ret
  }
  
  f2 <- function(x)
  {
    ret <- vector("list", length(x))
    i <- 1
    while(i <= length(x))
    {
      if(x[[i]] %% 2 == 1)
      {
        d <- exp(x[[i]])
        ret[[i]] <- 6 + d
      }
      else
      {
        ret[[i]] <- log(x[[i]])
      }
      i <- i + 1
    } 
    ret
  }
  
  f3 <- function(x)
  {
    lapply(x, function(a)
    {
      if(a %% 2 == 1)
      {
        d <- exp(a)
        6 + d
      }
      else
      {
        log(a)
      }
    }) 
  }
  
  funs <- list(f1, f2, f3)
  
  res <- NULL
  for (i in 1:(length(funs)-1))
    for (j in (i+1):length(funs))
    {
      res <- rbind(res, SimilaR_fromTwoFunctions(funs[[i]], 
                                                 funs[[j]],
                                                 functionNames=as.character(c(i,j)),
                                                 aggregation="both"))
      
    }
  expect_true(is.data.frame(res))
  expect_equal(sum(res[1:3, 3] >= rep(0.9, nrow(res))), length(rep(1, nrow(res))))
  expect_equal(sum(res[1:3, 4] >= rep(0.9, nrow(res))), length(rep(1, nrow(res))))
  expect_equal(sum(res[1:3, 5] == rep(1, nrow(res))), length(rep(1, nrow(res))))
  
})

test_that("loops5", {
  
  f1 <- function(x)
  {
    s <- 0
    for(i in 1:length(x))
    {
      s <- s + x[[i]]  
    } 
    s
  }
  
  f2 <- function(x)
  {
    s <- 0
    for(e in x)
    {
      s <- s + e
    } 
    s
  }
  
  funs <- list(f1, f2)
  
  res <- NULL
  for (i in 1:(length(funs)-1))
    for (j in (i+1):length(funs))
    {
      res <- rbind(res, SimilaR_fromTwoFunctions(funs[[i]], 
                                                 funs[[j]],
                                                 functionNames=as.character(c(i,j)),
                                                 aggregation="both"))
      
    }
  expect_true(is.data.frame(res))
  expect_equal(sum(res[1, 3] >= rep(1, nrow(res))), length(rep(1, nrow(res))))
  expect_equal(sum(res[1, 4] >= rep(1, nrow(res))), length(rep(1, nrow(res))))
  expect_equal(sum(res[1, 5] == rep(1, nrow(res))), length(rep(1, nrow(res))))
  
})

test_that("loops6", {
  
  f1 <- function(x, y)
  {
    ret <- vector("list", length(x))
    for(i in 1:length(x))
    {
      for(j in 1:length(y))
      {
        ret[[i]] <- x[[i]] + y[[j]]
      }
    } 
    ret
  }
  
  f2 <- function(x, y)
  {
    ret <- vector("list", length(x))
    i <- 1
    while(i <= length(x))
    {
      j <- 1
      while(j <= length(y))
      {
        ret[[i]] <- x[[i]] + y[[j]]
        j <- j + 1
      }
      i <- i + 1
    } 
    ret
  }
  
  funs <- list(f1, f2)
  
  res <- NULL
  for (i in 1:(length(funs)-1))
    for (j in (i+1):length(funs))
    {
      res <- rbind(res, SimilaR_fromTwoFunctions(funs[[i]], 
                                                 funs[[j]],
                                                 functionNames=as.character(c(i,j)),
                                                 aggregation="both"))
      
    }
  expect_true(is.data.frame(res))
  expect_equal(sum(res[1, 3] >= rep(1, nrow(res))), length(rep(1, nrow(res))))
  expect_equal(sum(res[1, 4] >= rep(1, nrow(res))), length(rep(1, nrow(res))))
  expect_equal(sum(res[1, 5] == rep(1, nrow(res))), length(rep(1, nrow(res))))
  
})

test_that("loops7", {
  
  f1 <- function(x, y)
  {
    s <- 0
    for(i in 1:length(x))
    {
      for(j in 1:length(y))
      {
        s <- s + x[[i]] + y[[j]]
      }
    } 
    s
  }
  
  f2 <- function(x, y)
  {
    s <- 0
    for(i in x)
    {
      for(j in y)
      {
        s <- s + i + j
      }
    } 
    s
  }
  
  f3 <- function(x, y)
  {
    ret <- vector("list", length(x))
    s <- 0
    i <- 1
    while(i <= length(x))
    {
      j <- 1
      while(j <= length(y))
      {
        s <- s + x[[i]] + y[[j]]
        j <- j + 1
      }
      i <- i + 1
    } 
    s
  }
  
  funs <- list(f1, f2, f3)
  
  res <- NULL
  for (i in 1:(length(funs)-1))
    for (j in (i+1):length(funs))
    {
      res <- rbind(res, SimilaR_fromTwoFunctions(funs[[i]], 
                                                 funs[[j]],
                                                 functionNames=as.character(c(i,j)),
                                                 aggregation="both"))
      
    }
  expect_true(is.data.frame(res))
  expect_equal(sum(res[1:3, 3] >= rep(1, nrow(res))), length(rep(1, nrow(res))))
  expect_equal(sum(res[1:3, 4] >= rep(1, nrow(res))), length(rep(1, nrow(res))))
  expect_equal(sum(res[1:3, 5] == rep(1, nrow(res))), length(rep(1, nrow(res))))
  
})

test_that("loops8", {
  
  f1 <- function(x, y)
  {
    lapply(x, function(a, b){a+b})
  }
  
  f2 <- function(x, y)
  {
    lapply(x, function(){x+y})
  }
  
  
  
  funs <- list(f1, f2)
  
  res <- NULL
  for (i in 1:(length(funs)-1))
    for (j in (i+1):length(funs))
    {
      res <- rbind(res, SimilaR_fromTwoFunctions(funs[[i]], 
                                                 funs[[j]],
                                                 functionNames=as.character(c(i,j)),
                                                 aggregation="both"))
      
    }
  expect_true(is.data.frame(res))
  
})

test_that("dplyr-apply", {
  clamp_vectorized1a <- function(x) {
    lapply(x, function(y) {
      if (max(y)-min(y) < 1e-5) {
        {{{{NULL}}}}
      } else {
        {{{{(y - min(y))/(max(y)-min(y))}}}}
      }
    })
  }
  
  
  
  clamp_vectorized1b <- function(x) {
    x %>% lapply(function(y) {
      if (max(y)-min(y) < 1e-5) {
        {{{{NULL}}}}
      } else {
        {{{{(y - min(y))/(max(y)-min(y))}}}}
      }
    })
  }
  
  res <- SimilaR_fromTwoFunctions(clamp_vectorized1a, 
                                  clamp_vectorized1b, returnType = "data.frame", aggregation = "both")
  
  expect_true(is.data.frame(res))
  expect_equal(res[1, 3], 1)
  expect_equal(res[1, 4], 1)
  expect_equal(res[1, 5], 1)
})

test_that("braces-dead code", {
  f1 <- function(x) {
    a <- x + 2
    b <- a - 2
    b
  }
  
  
  
  f2 <- function(x) {
    a <- x + 2
    z <- sum(x)^2
    b <- a - 2
    b
  }
  
  f3 <- function(x) {
    a <- x + 2
    z <- {sum(x)^2}
    b <- a - 2
    b
  }
  
  funs <- list(f1, f2, f3)
  
  res <- NULL
  for (i in 1:(length(funs)-1))
    for (j in (i+1):length(funs))
    {
      res <- rbind(res, SimilaR_fromTwoFunctions(funs[[i]], 
                                                 funs[[j]],
                                                 functionNames=as.character(c(i,j)),
                                                 aggregation="both"))
      
    }
  expect_true(is.data.frame(res))
  expect_equal(sum(res[1:3, 3] >= rep(1, nrow(res))), length(rep(1, nrow(res))))
  expect_equal(sum(res[1:3, 4] >= rep(1, nrow(res))), length(rep(1, nrow(res))))
  expect_equal(sum(res[1:3, 5] == rep(1, nrow(res))), length(rep(1, nrow(res))))
})


