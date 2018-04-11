is.prime2<-function(x){
   
   stopifnot(is.numeric(x) , all( x > 0 )) # wieksze od zera bo zakladam ze 0 nie 
                                    #jest liczba naturalna
   x<-as.integer(x)
   y<-x
   f<-sqrt(x)
   for ( i in seq_along(x) ){
      
      x[which(x %% 2 == 0)]<-0 #dla skrocenia rachunkow 'usuwam' l. parzyste
      x[which(floor(f)==1)] <- 1 # jako TRUE ustawiam liczby ktorych 
      # podloga z pierwiastka jest rowna 1 bo wtedy nie ma co sprawdzac
      if (length(which((x[i] %% (2:floor(f[i]))) ==0) ) >0) x[i]<-0
      
   }
   x[which(y==1)]<-0 # ustawiam z gory ze 1 nie jest liczba pierwsza
   as.logical(x)
}
