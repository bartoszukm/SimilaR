is.prime2<-function(x){
   asd1<-is.numeric(x)
   asd2<-all( x > 0 )
   stopifnot(asd1 , asd2) # wieksze od zera bo zakladam ze 0 nie 
                                    #jest liczba naturalna
   x<-as.integer(x)
   y<-x
   f<-sqrt(x)
   asd3<-seq_along(x)
   for ( i in asd3 ){
      asd4<-which(x %% 2 == 0)
      x[asd4]<-0 #dla skrocenia rachunkow 'usuwam' l. parzyste
	  asd5<-floor(f)
	  asd6<-which(asd5==1)
      x[asd6] <- 1 # jako TRUE ustawiam liczby ktorych 
      # podloga z pierwiastka jest rowna 1 bo wtedy nie ma co sprawdzac
	  asd8<-floor(f[i])
	  asd7<-which((x[i] %% (2:asd8)) ==0)
      if (length(asd7)>0) x[i]<-0
      
   }
   x[which(y==1)]<-0 # ustawiam z gory ze 1 nie jest liczba pierwsza
   as.logical(x)
}
