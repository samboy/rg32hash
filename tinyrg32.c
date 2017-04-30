#include <stdio.h> // cc -o tinyrg32 tinyrg32.c ; ./tinyrg32 'Test vector'
#include <stdint.h> // ./tinyrg32 --hex --numbers 'A tiny list of numbers'
#define b(z) for(c=0;c<z;c++) // ./tinyrg32 --binary-stream '¡Es la niña!'
#define d f[c*13]^=s;e[16+c]^=s; // This is public domain, but NO WARRANTY
uint32_t c,e[19],f[40],g=19,h=13,r,s,t,n[19],i,k;void m(){int c,j=0;b(12)f
[c+c%3*h]^=e[c+1];b(g){j=(c+j)&31;i=c*7%g;k=e[i++];k^=e[i%g]|~e[(i+1)%g];n
[c]=k>>j|k<<(32-j);}for(i=39;i--;f[i+1]=f[i])e[i%g]=n[i%g]^n[(i+1)%g]^n[(i
+4)%g];*e^=1;b(3)e[c+h]^=f[c*h]=f[c*h+h];}int main(int p,char**v){char *q=
v[--p];b(40)f[c]=e[c%19]=0;for(;;m()){b(3){for(s=r=0;r<4;){t=*q++;s|=(t?t&
255:1)<<8*r++;if(!t){d;b(17)m();b(7+p){if(~t&1)m();s=e[(t&1)+1];r=(p&3)-2?
c:1;b(4){i=s;if(p&4){i&=31;i+=i<8?50:57;}printf(p==2||p&4?"%c":"%02x",255&
i);s>>=8;}c=p?r:7;if((++t%8==0||(p&22)==2)&&p-2)puts("");}return 0;}}d;}}}
