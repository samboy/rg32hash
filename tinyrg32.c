#include <stdio.h> // tinyrg32.c Compile with cc -o tinyrg32 tinyrg32.c
#include <stdint.h>// Then, to run it: ./tinyrg32 '¡Es una niña linda!'
#define p uint32_t // Or perhaps ./tinyrg32 --binary-stream '¡La niña!'
#define f(a) for(c=0;c<a;c++) // Or ./tinyrg32 --hex --numbers '¡Niña!'
#define n w[c*13]^=s;u[16+c]^=s; // This is public domain - NO WARRANTY
void k(p*a,p*b){p m=19,A[19],x,o=13,c,y,r=0,q[3];f(12)b[c+c%3*o]^=a[c+1
];f(3){for(q[c]=b[y=c*o+12];y>c*o;b[y+1]=b[y])y--;b[y]=q[c];}f(m){r=(c+
r)&31;y=c*7%m;x=a[y++];x^=a[y%m]|~a[(y+1)%m];A[c]=x>>r|x<<(32-r);}f(m)a
[c]=A[c]^A[(c+1)%m]^A[(c+4)%m];f(3)a[c+o]^=q[c];*a^=1;}void l(char*v,p*
u,p*w){p s,q,c,x;f(39)w[c]=u[c%19]=0;for(;;k(u,w)){f(3){for(s=q=0;q<4;)
{x=*v++;s|=(x?x&255:1)<<8*q++;if(!x){n;f(17)k(u,w);return;}}n;}}}p i(p*
m,p*b,p*a){*a&2?k(m,b):0;return m[*a^=3];}int main(int g,char**v){p c,j
,b[39],m[19],q=2,z;l(v[--g],m,b);f(8){j=i(m,b,&q);z=g&1?c:1;f(4){printf
(g-2?"%02x":"%c",j&255);j>>=8;}c=g?z:7;if(g>2||c>6)puts("");}return 0;}
