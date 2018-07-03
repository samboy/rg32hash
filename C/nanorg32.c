#include <stdio.h>
#include <stdint.h>
#define b(z) for(c=0;c<z;c++)
uint32_t c,e[19],f[40],g=19,h=13,n[19],i,k;void m(){int c,j=0;b(12)f
[c+c%3*h]^=e[c+1];b(g){j=(c+j)&31;i=c*7%g;k=e[i++];k^=e[i%g]|~e[(i+1
)%g];n[c]=k>>j|k<<(32-j);}for(i=39;i--;f[i+1]=f[i])e[i%g]=n[i%g]^n[(
i+1)%g]^n[(i+4)%g];*e^=1;b(3)e[c+h]^=f[c*h]=f[c*h+h];}int main(int p
,char**v){char *q=v[--p];b(40)f[c]=e[c%g]=0;for(;;m()){b(3){for(p=0;
p<4;){i=*q++;f[c*h]^=k=(i?255&i:1)<<8*p++;e[16+c]^=k;if(!i){b(17)m()
;p=2;b(8){if(p&2)m();i=c;k=e[p^=3];b(4){printf("%02x",k&255);k>>=8;}
c=i;}return puts("");}}}}}
