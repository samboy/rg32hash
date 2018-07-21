#!/bin/sh

# Use tinyrg32 to generate a password; tinyrg32 source at the end of
# this script

SECRET=$( cat $HOME/.mhash_prefix2 )
if [ -z "$SECRET" ] ; then
	echo Could not find $HOME/.mhash_prefix2
	exit 1
fi

INDEX="$1"
if [ -z "$INDEX" ] ; then
	echo Usage: $0 {index} {site}
	echo Where {index} is the password number index for this site
	exit 1
fi

SITE="$2"
if [ -z "$SITE" ] ; then
	echo Usage: $0 {index} {site}
	echo Where {site} is the website to generate a password for
	exit 1
fi

TINYRG32="tinyrg32"

if ! command -v tinyrg32 > /dev/null 2>&1 ; then
	cat > tinyrg32-$$.c << EOF
#include <stdio.h> // cc -o tinyrg32 tinyrg32.c /////// Public domain code
#include <stdint.h> // ./tinyrg32 --hex --numbers 'A tiny list of numbers'
#define b(z) for(c=0;c<(z);c++) // ./tinyrg32 --binary-stream 'Es la niña'
uint32_t c,e[40],f[40],g=19,h=13,r,s,t,n[40],i,k,z=0;void m(){int c,j=0;b(
12)f[c+c%3*h]^=e[c+1];b(40){j=(c+j)&31;i=c*7%g;k=e[i++];k^=e[i%g]|~e[(i+1)
%g];n[c]=k>>j|k<<(32-j);}for(c--;c--;f[c+1]=f[c])e[c]=n[c]^n[(c+1)%g]^n[(c
+4)%g];*e^=1;b(3)e[c+h]^=f[c*h]=f[c*h+h];}int main(int p,char**v){char *q=
v[--p],y=0;b(40)f[c]=e[c]=0;for(;;m()){b(3){for(r=0;r<4;){f[c*h]^=k=(*q?*q
&255:1)<<8*r++;e[16+c]^=k;if(!*q++){b(17)m();b(p?8*p:8){if(~t&1)m();s=e[(t
&1)+1];r=(p&3)-2?c:1;b(4){i=s;if(p&4){y=1;z=z?z:*v[p-1]%8;i&=31;i+=i<8?50:
89;}printf(p==2||p&4?"%c":"%02x",255&i);s>>=8;}c=r;if(!y&&(++t%8==0||(p&22
)==2)&&p-2){puts("");}if(y&&!--z)puts("_Aa1");}y?puts(""):1;return 0;}}}}}
EOF
	gcc -o tinyrg32-$$ tinyrg32-$$.c
	TINYRG32=./tinyrg32-$$
fi

# If you need an index above ten, change this command line
$TINYRG32 --make --ten --passwords 3 "$SECRET:$SITE" | head -$INDEX | tail -1 

rm -f ./tinyrg32-$$ ./tinyrg32-$$.c ./tinyrg32-$$.exe

