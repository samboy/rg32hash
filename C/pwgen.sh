#!/bin/sh

# Use tinyrg32 to generate a password; tinyrg32 source at the end of
# this script

# Note that this version of pwgen.sh is not compatible with passwords
# generated with the older pwgen.sh; this in mind, there is pwgen-old.sh
# for people who need the older passwords.

# Change this to something else to get a bit more security
SUFFIX="_Aa9"

SECRET=$( cat $HOME/.mhash_prefix2 )
if [ -z "$SECRET" ] ; then
	echo Could not find $HOME/.mhash_prefix2
	exit 1
fi

SITE="$1"
if [ -z "$SITE" ] ; then
	echo Usage: $0 {site} 
	echo Where {site} is the website to generate a password for
	exit 1
fi

# As a general rule, *most* sites require: 8-16 characters, at least one:
# a) lower case letter
# b) upper case letter
# c) number
# d) punctuation, where _ is considered punctuation
# The tinyrg32 program adds _Aa1 to the end of each password which covers
# *most* sites.  However, this being the internet, some sites have dumb
# password rules we have to make exceptions for.  This script covers
# most of them

# Some sites will not allow a '_' character in their passwords.  That
# in mind, we can optionally zap the '_' character.
# Real world offender: timewarnercable.com
ZAP='~'

# Some sites do not consider _ punctuation.  
# Make $CHANGE something besides '_' to change the '_' to another symbol
# Real world offender: southwest.com
CHANGE='_'

# Some sites require passwords to be changed frequently.  This is actually
# a reasonable security practice, and we support it
INDEX=1

# Some sites might require a password to be longer than 16 characters, or
# to be shorter. The password length is 4 + (4 * LEN), so for a site which
# can only handle passwords 12 characters long, make LEN 2; for a site which
# can only handle 8 character passwords, make LEN 1.  For someone who needs
# 24 chars in the password, make LEN 5
LEN=3 # 16 character password

##### SITE SPECIFIC RULES GO HERE #####
if [ "$SITE" = "timewarnercable.com" ] ; then
	ZAP='_'
fi
if [ "$SITE" = "southwest.com" ] ; then
	CHANGE=':'
fi
if [ "$SITE" = "paypal.com" ] ; then
	INDEX=2
fi
if [ "$SITE" = "idiot.example.com" ] ; then
	LEN=2
fi
### END SITE SPECIFIC RULES ###

# Make sure tinyrg32 is in the path; if not then compile and run it
TINYRG32="tinyrg32"

if ! command -v tinyrg32 > /dev/null 2>&1 ; then
	cat > tinyrg32-$$.c << EOF
#include <stdio.h> // cc -o tinyrg32 tinyrg32.c /////// Public domain code
#include <stdint.h> // ./tinyrg32 --hex --numbers 'A tiny list of numbers'
#define b(z) for(c=0;c<(z);c++) // ./tinyrg32 --binary-stream 'Es la niña'
uint32_t c,e[40],f[40],g=19,h=13,r,s,t,n[40],i,k,y,z;void m(){int c,j=0;b(
12)f[c+c%3*h]^=e[c+1];b(g){j=(c+j)&31;i=c*7%g;k=e[i++];k^=e[i%g]|~e[(i+1)%
g];n[c]=k>>j|k<<(32-j);}for(c=39;c--;f[c+1]=f[c])e[c]=n[c]^n[(c+1)%g]^n[(c
+4)%g];*e^=1;b(3)e[c+h]^=f[c*h]=f[c*h+h];}int main(int p,char**v){char *q=
v[--p],*x=0;for(;;m()){b(3){for(r=0;r<4;){f[c*h]^=k=(*q?*q&255:1)<<8*r++;e
[16+c]^=k;if(!*q++){b(17)m();b(p<3?8:9973){if(~t&1)m();s=e[(t&1)+1];r=(p&3
)-2?c:1;b(4){i=s;if(p&4){x=v[p-2];y=z=z?z:*v[p-1]%16;i&=31;i+=i<8?50:89;}s
>>=8;printf(p==2||p&4?"%c":"%02x",255&i);}if((++t%8==0||(p&22)==2)&&p-2&&!
y){puts("");}c=r;if(y&&!--z)puts(x?x:"");}if(x&&t%8)puts(x);return 0;}}}}}
EOF
	cc -o tinyrg32-$$ tinyrg32-$$.c
	TINYRG32=./tinyrg32-$$
fi

# If you need an index above ten, change this command line
$TINYRG32 --make --ten $SUFFIX $LEN "$SECRET:$SITE" | head -$INDEX | \
	tail -1 | tr -d "$ZAP" | tr '_' "$CHANGE"

rm -f ./tinyrg32-$$ ./tinyrg32-$$.c ./tinyrg32-$$.exe

