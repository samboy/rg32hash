#include <stdio.h> // cc -o microrg32 microrg32.c ; WORK=3 ; LEN=4 #######
#include <stdint.h> // SECRET="Something random like qhohxks5mx9el9v6ujg3"
#include <stdlib.h> // export P="$LEN:$SECRET:x.org" ## Public domain code
#define b(z) for(c=0;c<(z);c++) // ./microrg32 $WORK $LEN | head -1 | tail
uint32_t c,e[40],f[40],g=19,h=13,r,s,t=2,n[45],i,k,y,z;void m(){int c,j=0;
b(12)f[c+c%3*h]^=e[c+1];b(g){j=(c+j)&31;i=c*7%g;k=e[i++];k^=e[i%g]|~e[(i+1
)%g];n[c]=n[c+g]=k>>j|k<<(32-j)%32;}for(c=39;c--;f[c+1]=f[c])e[c]=n[c]^n[c
+1]^n[c+4];*e^=1;b(3)e[c+h]^=f[c*h]=f[c*h+h];}int main(int p,char**v){char
*q;q=getenv("P");if(q&&p>2){for(;;m()){b(3){for(r=0;r<4;){f[c*h]^=k=(*q?*q
&255:1)<<r++*8;e[16+c]^=k;if(!*q++){b(16+(1<<*v[1]%32))m();b(983){if(~t&1)
{m();}s=e[t^=3];r=c;b(4){q=v[p-1];z=i=z?z:*v[2]%16;i=s;i&=31;i+=i<8?50:89;
s>>=8;printf("%c",255&i);}c=r;if(!--z)puts("");}puts("__");return 0;}}}}}}
