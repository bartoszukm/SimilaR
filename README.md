# SimilaR: R Code Clone and Plagiarism Detection

Third-party software for assuring source code quality is becoming increasingly
popular. Tools that evaluate the coverage of unit tests,
perform static code analysis, or inspect run-time memory use
are crucial in the software development life cycle.
More sophisticated methods allow for performing meta-analyses
of large software repositories, e.g., to discover abstract topics they relate to
or common design patterns applied by their developers.
They may be  useful in gaining a better understanding of the component
interdependencies, avoiding cloned code as well as detecting plagiarism
in programming classes.

A meaningful **measure of  similarity of computer programs** often forms
the basis of such tools. While there are a few noteworthy instruments
for similarity assessment, none of them turns out particularly suitable
for analysing R code chunks. Existing solutions rely on rather simple
techniques and heuristics and fail to provide a user with
the kind of sensitivity and specificity required for working with R scripts.
In order to fill this gap, the **SimilaR** package implements a new algorithm
based on a Program Dependence Graph.
It can serve as a tool not only for **improving R code quality**
but also for **detecting plagiarism**, even when it has been masked
by applying some obfuscation techniques or imputing dead code.
