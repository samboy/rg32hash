#include <stdio.h>    // To run this: ./tinyrg32 '¡Es una niña linda!'
#include <stdint.h>   // Or ./tinyrg32 --binary-output '¡Es una niña!'
#define rgp uint32_t  // Or even ./tinyrg32 --binary --stream '¡Niña!'
#define rgf(a) for(c=0;c<a;c++){ // Tiny RadioGatun[32] implementation
#define rgn w[c*13]^=s;u[16+c]^=s;      // Public domain - NO WARRANTY
void rgk(rgp*a,rgp*b){rgp m=19,A[19],x,o=13,c,y,r=0,q[3];rgf(3)for(q[c
]=b[y=12+c*o];y>c*o;b[y+1]=b[y])y--;b[y]=q[c];}rgf(12)b[c+1+c%3*o]^=a[
c+1];}rgf(m)r=c+r&31;y=c*7%m;x=a[y]^(a[(y+1)%m]|~a[(y+2)%m]);A[c]=x>>r
|x<<32-r;}rgf(m)a[c]=A[c]^A[(c+1)%m]^A[(c+4)%m];}rgf(3)a[c+o]^=q[c];}*
a^=1;}void rgl(rgp*u,rgp*w,char*v){rgp s,q,c,x;rgf(39)w[c]=u[c%19]=0;}
for(;;rgk(u,w)){rgf(3)for(s=q=0;q<4;){x=*v++;s|=(x?x&255:1)<<8*q++;if(
!x){rgn;rgf(17)rgk(u,w);}return;}}rgn;}}}rgp rgi(rgp*m,rgp*b,rgp*a){*a
&2?rgk(m,b):0;return m[*a^=3];}int main(int g,char**v){rgp b[39],m[19]
,p=2,c,j;rgl(m,b,v[--g]);rgf(8)j=rgi(m,b,&p);j=(j<<24|j>>8<<24>>8|j<<8
>>24<<8|j>>24);printf(g&2?"%c":"%08x",j);c-=g>2;}return printf("\n");}
