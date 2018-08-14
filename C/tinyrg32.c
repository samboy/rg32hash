#include <stdio.h> // cc -o tinyrg32 tinyrg32.c /////// Public domain code
#include <stdint.h> // ./tinyrg32 --hex --numbers 'A tiny list of numbers'
#define b(z) for(c=0;c<(z);c++) // ./tinyrg32 --binary-stream 'Es la niña'
uint32_t c,e[40],f[40],g=19,h=13,r,s,t,n[45],i,k,y,z;void m(){int c,j=0;b(
12)f[c+c%3*h]^=e[c+1];b(g){j=(c+j)&31;i=c*7%g;k=e[i++];k^=e[i%g]|~e[(i+1)%
g];n[c]=n[c+g]=k>>j|k<<((32-j)&31);}for(c=39;c--;f[c+1]=f[c])e[c]=n[c]^n[c
+1]^n[c+4];*e^=1;b(3)e[c+h]^=f[c*h]=f[c*h+h];}int main(int p,char**v){char
*q=v[--p],*x=0;for(;;m()){b(3){for(r=0;r<4;){f[c*h]^=k=(*q?*q&255:1)<<r++*
8;e[16+c]^=k;if(!*q++){b(17)m();b(p<3?8:89*p){if(~t&1)m();s=e[(t&1)+1];r=(
p&3)-2?c:1;b(4){i=s;if(p&4){x=v[p-2];y=z=z?z:*v[p-1]%16;i&=31;i+=i<8?50:89
;}s>>=8;printf(p==2||p&4?"%c":"%02x",255&i);}if(((++t%8==0||(p&22)==2)&&p-
2&&!y)||r+1>=89*p){puts("");}c=r;if(!--z)puts(*x==95?x:"");}return 0;}}}}}
