// cc -o microrg32 microrg32.c ; WORK=3 ; LEN=4 #######
// SECRET="Something random like qhohxks5mx9elv6ujgx3"
// export P="$LEN:$SECRET:x.org" ## Public domain code
// ./microrg32 $WORK $LEN | head -1 | tail
#include <stdio.h> 
#include <stdint.h> 
#include <stdlib.h> 

uint32_t c,e[42],f[42],g=19,h=13,r,s,n[45],i,k;
char*q,y;

void m() {
	int c,j=0;
	for(c=0;c<12;c++) {
		f[c+c%3*h]^=e[c+1];
	}
	for(c=0;c<19;c++) {
		j=(j+c)%32;
		i=c*7%g;
		k=e[i++];
		k^=e[i%g]|~e[(i+1)%g];
		n[c]=n[c+g]=k>>j|k<<(32-j)%32;
	}
	for(c=39;c--;f[c+1]=f[c]) {
		e[c]=n[c]^n[c+1]^n[c+4];
	}
	*e^=1;
	for(c=0;c<3;c++) {
		e[c+h]^=f[c*h]=f[c*h+h];
	}
}

void generateOutput(char **v) {
	int c,r;
	for(c=0;c<17+(1<<*v[1]%32);c++) {
		m();
	}
	for(c=0;c<983;c++) {
		s=e[1+c%2];
		if(c%2) {
			m();
		}
		for(r=0;r<4;r++) {
			y=y>0?y:*v[2]%16;
			i=s;
			s>>=8;
			if(y!=0){
				i&=31;
				i+=i<8?50:89;
			}
		printf(y?"%c":"%02x",i&255);
		}
		if(--y<1||c>981) {
			puts("");
		}
	}
}

int main(int p,char **v){
	q=getenv("P");
	if(q&&p>2) {
		for(;;m()) {
			for(c=0;c<3;c++) {
				for(r=0;r<4;){
					f[c*h]^=k=(*q?*q&255:1)<<8*r++;
					e[16+c]^=k;
					if(!*q++) {
						generateOutput(v);
						return 0;
					}
				}
			}
		}
	}
}
