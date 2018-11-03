#include <stdio.h>
#include <stdint.h>
#define b(z) for(c=0;c<z;c++)
uint32_t c,e[40],f[40],g=19,h=13,n[45],i,k;void m(){int c,j=0;b
(12)f[c+c%3*h]^=e[c+1];b(g){j+=c;i=c*7%g;k=e[i++];k^=e[i%g]|~e[
(i+1)%g];n[c]=n[c+g]=k>>j%32|k<<(32-j)%32;}for(c=39;c--;f[c+1]=
f[c])e[c]=n[c]^n[c+1]^n[c+4];*e^=1;b(3)e[c+h]^=f[c*h]=f[c*h+h];
}int main(int p,char**v){char*q=v[--p];for(;;m()){b(3){for(p=0;
p<4;){f[c*h]^=k=(*q?255&*q:1)<<8*p++;e[16+c]^=k;if(!*q++){p=2;b
(17)m();b(8){if(p&2)m();i=c;k=e[p^=3];b(4){printf("%02x",255&k)
;k>>=8;}c=i;}return puts("");}}}}}
