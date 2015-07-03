#include <stdio.h>     // To run this: ./tinyrg32 '¡Es una niña linda!'
#include <stdint.h>    // Or ./tinyrg32 --binary-output '¡Es una niña!'
#define p uint32_t     // Or even ./tinyrg32 --binary --stream '¡Niña!'
#define f(a) for(c=0;c<a;c++)     // Tiny RadioGatun[32] implementation
#define n w[c*13]^=s;u[16+c]^=s;         // Public domain - NO WARRANTY
void k(p*a,p*b){p m=19,A[19],x,o=13,c,y,r=0,q[3];f(3){for(q[c]=b[y=c*o+
12];y>c*o;b[y+1]=b[y])y--;b[y]=q[c];}f(12)b[c+1+c%3*o]^=a[c+1];f(m){r=c
+r&31;y=c*7%m;x=a[y++];x^=a[y%m]|~a[(y+1)%m];A[c]=x>>r|x<<(32-r);}f(m)a
[c]=A[c]^A[(c+1)%m]^A[(c+4)%m];f(3)a[c+o]^=q[c];*a^=1;}void l(char*v,p*
u,p*w){p s,q,c,x;f(39)w[c]=u[c%19]=0;for(;;k(u,w)){f(3){for(s=q=0;q<4;)
{x=*v++;s|=(x?x&255:1)<<8*q++;if(!x){n;f(17)k(u,w);return;}}n;}}}p i(p*
m,p*b,p*a){*a&2?k(m,b):0;return m[*a^=3];}int main(int g,char**v){p c,j
,b[39],m[19],q=2;l(v[--g],m,b);f(8){j=i(m,b,&q);j=(j<<24|j>>24|j>>8<<24
>>8|j<<8>>24<<8);printf(g&2?"%c":"%08x",j);c-=g>2;}puts("");return 0;}
