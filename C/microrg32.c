#include <stdio.h> // cc -o microrg32 microrg32.c ; WORK=3 ; LEN=4 ######
#include <stdint.h> // SECRET="Something random like qhohxks5mx9elv6ujg3"
#include <stdlib.h> // export P="$LEN:$SECRET:x.org" # Public domain code
#define b(z) for(c=0;c<(z);c++) // ./microrg32 $WORK $LEN | head -1| tail
uint32_t c,e[42],f[42],g=19,h=13,r,s,n[45],i,k;char*q,y;void m(){int c,j=
0;b(12)f[c+c%3*h]^=e[c+1];b(g){j+=c;i=c*7%g;k=e[i++];k^=e[i%g]|~e[(i+1)%g
];n[c]=n[c+g]=k>>j|k<<-j%32;}for(c=39;c--;f[c+1]=f[c])e[c]=n[c]^n[c+1]^n[
c+4];*e^=1;b(3)e[c+h]^=f[c*h]=f[c*h+h];}int main(int p,char**v){q=getenv(
"P");if(q&&p>2){for(;;m()){b(3){for(r=0;r<4;){f[c*h]^=k=(*q?*q&255:1)<<8*
r++;e[16+c]^=k;if(!*q++){b(17+(1<<*v[1]%32))m();b(983){s=e[1+c%2];r=c;if(
c%2)m();b(4){y=y>0?y:*v[2]%16;i=s;s>>=8;if(y){i&=31;i+=i<8?50:89;}printf(
y?"%c":"%02x",i&255);}c=r;if(--y<1||c>981)puts("");}return 0;}}}}}}/*--*/
