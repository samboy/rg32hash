#include <stdio.h>
#include <stdint.h>
#define b(z) for(c=0;c<z;c++)
uint32_t c,e[40],f[40],g=19,h
=13,n[45],i,k;void m(){int c,
j=0;b(12)f[c+c%3*h]^=e[c+1];b
(g){i=c*7%g;k=e[i++];j=(c+j)&
31;k^=e[i%g]|~e[(i+1)%g];n[c]
=n[c+g]=k>>j|k<<(32-j);}for(i
=39;i--;f[i+1]=f[i])e[i]=n[i]
^n[i+1]^n[i+4];b(3)e[c+h]^=f[
c*h]=f[c*h+h];*e^=1;}int main
(int p,char**v){char*q=v[--p]
;for(;;m()){b(3){for(p=0;p<4;
){f[c*h]^=k=(*q?255&*q:1)<<8*
p++;e[c+16]^=k;if(!*q++){b(17
)m();b(32){if(c%8==0){q=(char
*)e+4;m();}printf("%02x",*q++
&255);}return puts("");}}}}}