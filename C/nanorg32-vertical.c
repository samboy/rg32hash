#include <stdio.h>
#include <stdint.h>
#define b(z) for(c=0;c<z;c++)
#define d f[c*13]^=s;e[16+c]^=s;
uint32_t c,e[19],f[40],g=19,h=13
,r,s,t,n[19],i,k;void m(){int c,
j=0;b(12)f[c+c%3*h]^=e[c+1];b(g)
{j=(c+j)&31;i=c*7%g;k=e[i++];k^=
e[i%g]|~e[(i+1)%g];n[c]=k>>j|k<<
(32-j);}for(i=39;i--;f[i+1]=f[i]
)e[i%g]=n[i%g]^n[(i+1)%g]^n[(i+4
)%g];*e^=1;b(3)e[c+h]^=f[c*h]=f[
c*h+h];}int main(int p,char**v){
char *q=v[--p];b(40)f[c]=e[c%19]
=0;for(;;m()){b(3){for(s=r=0;r<4
;){t=*q++;s|=(t?t&255:1)<<8*r++;
if(!t){d;b(17)m();t=2;b(8){if(t&
2)m();r=c;s=e[t^=3];b(4){printf(
"%02x",s&255);s>>=8;}c=r;}return
puts("");}}d;}}}
