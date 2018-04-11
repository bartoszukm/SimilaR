 kwantyle1<-function(x,p){
if (is.numeric(x) && is.numeric(p) && 0 <= p && p <=1)
{n=length(x);
h<-(n-1)*p+1;
return (sort(x)[floor(h)]+(h-floor(h))*(sort(x)[floor(h)+1]-sort(x)[floor(h)]));}
else
{NA}}

 kwantyle1_2<-function(x_5467784,p_5467784){
if (is.numeric(x_5467784) && is.numeric(p_5467784) && 0 <= p_5467784 && p_5467784 <=1)
{n_5467784=length(x_5467784);
h_5467784<-(n_5467784-1)*p_5467784+1;
return (sort(x_5467784)[floor(h_5467784)]+(h_5467784-floor(h_5467784))*(sort(x_5467784)[floor(h_5467784)+1]-sort(x_5467784)[floor(h_5467784)]));}
else
{NA}} 
