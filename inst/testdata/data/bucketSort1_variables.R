bucketsort1<-function(v){
   
   # nie wiedzialam jak sobie poradzic, wiec przyjelam, ze bede zaokraglac
   # liczby do czterech miejsc po przecinku
   v_1<-round(v,digits=-4) 
   v2<-v*(10^4) # robie tak, zeby dostac liczby calkowite
   v3<-max(v2) # jako k wybiram najwieksza z liczb
   v4<-f(v3,v2) # stosuje funkcje z poprzedniego zadania
   v5<-NULL # poczatkowy wektor wynikowy jest pusty
   for(i in 1:v3)
      if (v4[i]!=0){
         v5<-c(v5,rep(i,v4[i]))
      }
   v5<-v5/(10^4) # dziele przez 10^4 bo na poczatku przez tyle pomnozylam
   v5
}
