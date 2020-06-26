# (C) 2020 Marek Gagolewski, https://www.gagolewski.com


.PHONY: r r-check testthat r-build

all: r


r:
	Rscript -e 'Rcpp::compileAttributes()'
	R CMD INSTALL .
	# AVOID ADDING THE -O0 flag!!!
	Rscript -e 'roxygen2::roxygenise(roclets=c("rd", "collate", "namespace", "vignette"), load_code=roxygen2::load_installed)'
	R CMD INSTALL . --install-tests

r-check: r
	Rscript -e 'devtools::check()'

testthat: r
	Rscript -e 'options(width=120); source("tests/testthat.R")'


r-build:
	Rscript -e 'Rcpp::compileAttributes()'
	Rscript -e 'roxygen2::roxygenise(roclets=c("rd", "collate", "namespace", "vignette"))'
	R CMD INSTALL . --preclean
	R CMD build .
