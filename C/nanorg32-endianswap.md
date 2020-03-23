## nanorg32-endianswap.c

This is a variant of RadioGatún[32] where the 32-bit words in the 
output are endian-swapped.  This is otherwise identical to RadioGatún[32]
and has the same level of security as RadioGatún[32].

This version of RadioGatún[32] fits in under 512 bytes.

Here is the code:

```c
#include<stdio.h>
#include<stdint.h>
#define b(z) for(c=0;c<z;c++)
uint32_t c,e[42],f[42],g=19,h=13,n[45],i,j,k;void m(){j=0;
b(12)f[c+c%3*h]^=e[c+1];b(g){i=c*7%g;k=e[i++];k^=e[i%g]|~e
[(i+1)%g];j+=c;n[c]=n[c+g]=k>>j%32|k<<-j%32;}for(i=39;i--;
f[i+1]=f[i])e[i]=n[i]^n[i+1]^n[i+4];b(3)e[c+h]^=f[c*h]=f[c
*h+h];*e^=1;}int main(int c,char**v){char*q=v[--c];for(;;m
()){b(3){for(j=0;j<4;){f[c*h]^=k=(*q?255&*q:1)<<8*j++;e[c+
16]^=k;if(!*q++){b(18)m();b(8){printf("%08x",e[1+c%2]);if(
c%2)m();}return puts("");}}}}}
```

To use:

```
cc -o nanorg32-endianswap nanorg32-endianswap.c
./nanorg32-endianswap '1234' | awk '{
for(a=0;a<64;a+=8){for(b=7;b>=1;b-=2){printf("%s",substr($0,a+b,2))}}print ""}'
```

This AWK script corrects the endian in nanorg32-endianswap to generate
correct RadioGatún[32] test vectors.
