 kwantyle1<-function(x,p){
if (is.numeric(x) && is.numeric(p) && 0 <= p && p <=1)
{n=length(x);
h<-(n-1)*p+1;
return (sort(x)[floor(h)]+(h-floor(h))*(sort(x)[floor(h)+1]-sort(x)[floor(h)]));}
else
{NA}}
