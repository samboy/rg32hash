#include <stdio.h>
#include <stdint.h>
#define b(z) for(c=0;c<z;c++)
#define d f[c*13]^=s;e[16+c]^=s;
int verboseLevel = 9;
uint32_t c,e[19],f[40],g=19,h=13,r,s,t,n[19],i,k;
void show() {
int aa;
printf("\n\n    Mill       Belt Row 1 Belt Row 2 Belt Row 3\n");
for(aa = 0; aa < 19; aa++) {
  printf(" ");
  if(aa < 10) {printf(" ");}
  if(aa < 13) {
   printf("%d 0x%08x 0x%08x 0x%08x 0x%08x\n",aa,e[aa],f[aa],f[aa+13],f[aa+26]);
  } else {
   printf("%d 0x%08x\n",aa,e[aa]);
  }
}
printf("\n\n\n");}
void showX(uint32_t *aa) {
int ab;
puts("");
for(ab = 0; ab < 19; ab++) {
  printf(" ");
  if(ab < 10) {printf(" ");}
  printf("%d 0x%08x\n",ab,aa[ab]);
}
printf("\n\n");}

void m(){
int c
,j=0;

if(verboseLevel > 0) {
puts("");
puts("Current mill state");
show();
}

b(12)f[c+c%3*h]^=e[c+1];
if(verboseLevel > 2) {
puts("Mill to belt feedforward performed");
show();}

for(i=39;i--;f[i+1]=f[i]);
b(3)f[c*h]=f[c*h+h];
if(verboseLevel > 2) {
puts("Belt rotation performed");
show();
}

b(g){j=(c+j)&31;i=c*7%g;k=e[i++];k
^=e[i%g]|~e[(i+1)%g];n[c]=k>>j|k<<(32-j);}
if(verboseLevel > 1) {
showX(n);// Show them the numbers which we put in to the mill again
}
for(i=19;i--;)e[i%g]=n[i%g]^n[(i+1)%g]^n[(i+4)%g];

if(verboseLevel > 1) {
puts("Mill core function performed");
show();}

*e^=1;
if(verboseLevel > 3) {
puts("Iota step done");
show();
}

b(3)e[c+h]^=f[c*h];
if(verboseLevel > 4) {
puts("Belt to mill exclusive or done");
show();}

}
int main(int p,char**v){char *q=v[--p];b(40)f[c]=e[
c%19]=0;for(;;m()){b(3){for(s=r=0;r<4;){t=*q++;s|=(t?t&255:1)<<
8*r++;if(!t){d;b(17)m();t=2;b(9){printf("- ");if(t&2)m();r=c;s=e[t^=3];b(4){
printf("%02x",s&255);s>>=8;}c=r;}return puts("");}}d;}}}
