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

readFilesAndGetFunctionNames <-function(dirname, submissionType=2, rewriteFiles=TRUE)
{
  filesNames <- getFileNamesWithPattern(dirname,"\\.R$|\\.r$")
  if(length(filesNames) <= 1 && submissionType==2)
  {
    stop(stri_paste("Number of files named *.R in directory '", dirname, "' is less less than or equal 1 while fileTypes is equal to 'file'."))
  }
  if(length(filesNames) == 0)
  {
    stop(stri_paste("There are no files with extension *.R in directory '", dirname, "'"))
  }
  files <- file.path(dirname,filesNames)
  parses <- getFunctionsFromListFiles(files,filesNames,submissionType, rewriteFiles = rewriteFiles)
  sums <- cumsum(lapply(parses$parses,length))
  sums <- c(0,sums)
  functionNames<-getFunctionNames(parses$filesNamesRepeated,sums,unlist(parses$parses))

  list(parses=parses, sums=sums, functionNames=functionNames)
}

getFileNamesWithPattern <- function(dirname,pattern)
{
   if(!file.exists(dirname))
   {
     stop(stri_paste("Directory '", dirname, "' does not exist."))
   }
   filesNamesList <- list.files(path = dirname, pattern = pattern,include.dirs = FALSE,recursive=FALSE,full.names=TRUE)
   filesNamesList <- filesNamesList[!file.info(filesNamesList)$isdir]
   filesNamesListShort <- basename(filesNamesList)
   return(filesNamesListShort)
}

getFunctionsFromListFiles <- function(files,filesNames, submissionType, rewriteFiles=TRUE) #1 - functions, 2-files, 3-groups of files
{
   filesText <- lapply(files,function(x){lapply(x,stri_read_lines)})
   #print(files)
   if(rewriteFiles)
   {
      mapply(function(text,file){stri_write_lines(str = text, fname = file)},unlist(filesText,recursive = FALSE),unlist(files))
   }

   errorText <- character(0)
   parses <- lapply(filesText,function(x){
   lapply(x,function(el){tryCatch(parse(text=el,encoding="UTF-8"),
    error=function(e) {
         errorText <<- c(errorText,paste(e,"\n"))
         return(NA)
         })})})


   notParsed <- unlist(filesNames)[unlist(lapply(parses,is.na))]
   if(length(notParsed) != 0)
   {
      filesNotParsed <- paste(notParsed, collapse=", ")
      errorText <- paste(notParsed,errorText,collapse="\n")
      errorText <- paste(errorText,"\nFailed to parse the following files: ",filesNotParsed)
      stop(stri_paste("Files not parsed: ", errorText))
   }

   if(length(errorText)==0) errorText <- ""

   for (i in seq_along(parses))
   {
      for(j in seq_along(parses[[i]]))
      {
         parses[[i]][[j]] <- parses[[i]][[j]][unlist(lapply(parses[[i]][[j]],length))==3]
         parses[[i]][[j]] <- parses[[i]][[j]][unlist(lapply(parses[[i]][[j]],function(x){as.character(x[[1]])=="<-" | as.character(x[[1]])=="="}))] # sprawdzic!
         parses[[i]][[j]] <- parses[[i]][[j]][unlist(lapply(parses[[i]][[j]],function(x){
            if(typeof(x[[3]])!="language")
               FALSE
            else
               as.character(x[[3]][[1]])=="function"
         }))]
      }
   }

   if(submissionType == 2) # Every file against the rest
   {
      parses <- unlist(parses, recursive = FALSE)
      filesNamesRepeated <- unlist(filesNames)
      return(list(parses = parses, errorText = errorText,filesNamesRepeated=filesNamesRepeated))
   }
   if(submissionType == 1) # Every function against the rest
   {
      parses <- unlist(parses, recursive = FALSE)
      parsesUnlist <- unlist(parses)
      parsesUnlist <- lapply(parsesUnlist, function(x) as.expression(x))
      filesNamesRepeated <- rep(unlist(filesNames), unlist(lapply(parses,length)))
      return(list(parses = parsesUnlist, errorText = errorText, filesNamesRepeated=filesNamesRepeated))
   }
   if(submissionType == 3) # Every group of files against the rest
   {
      return(list(parses = parses, errorText = errorText, filesNamesRepeated = filesNames))
   }

}

getFunctionNames <- function(filesNamesListShort,sums,parses)
{
   indices <- seq_along(parses)
   functionNames <- mapply(function(parse,index,sums,filesNamesListShort){
      paste(filesNamesListShort[max(which(index > sums))]," ",deparse(parse[[2]]))
   },parses,indices,MoreArgs=list(filesNamesListShort=filesNamesListShort,sums=sums))
   functionNames
}
