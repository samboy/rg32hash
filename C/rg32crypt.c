// This is a public domain pashword hasher using RadioGatún[32]
// This is only for diverse systems where it is not feasible to
// implement Argon2 site wide; most commonly mixed shops where
// there is no site-wide standardization of the language or
// environment used.
//
// This is reasonably secure; the cost is not specified in the hash,
// but is 2 ^ 15 (32768) blank rounds of RagioGatún[32].  This is harder
// to crack, but should be fast enough to be usable across various 
// languages (The pure Python implementation is dog slow, but a pure
// PHP implementation is reasonably fast)
//
// The hashed password is 32 characters long.  This is so the hashed
// password can fit in a database with a 32-character fixed width field
// (yes, I have seen in the wild databases which required the hashed 
//  password to be 32 characters or shorter, since the original designer
//  used simple MD5 for the password hash with no knowledge of the 
//  negative security consequences).
//
// The salt has 48 bits of entropy; the format of the password is:
// “~~” (2 characters), followed by a 8-character salt, followed by 
// a 22 character password hash with 132 bits of entropy (base-64)
//
// The base-64 encoding is non-standard, as follows:
// "0123456789@ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz."
//
// Again, **USE ARGON2 IF POSSIBLE** but this is “good enough” (the
// same way Solar Designer’s mid-oughts phpass using multiple passes
// of MD5 with salting is still “good enough” here in the 2020s)

#include <stdint.h> // Public domain password hashing
#include <string.h> // Binary string support
#include <stdio.h> // File open support
#include <stdlib.h> // exit() support

void rg32_f(uint32_t*a,uint32_t*b){
	uint32_t m=19,A[45],x,o=13,c,y,r=0;
	for(c=0;c<12;c++)b[c+c%3*o]^=a[c+1];
	for(c=0;c<m;c++){
		r=(c+r)&31;y=c*7;x=a[y++%m];x^=a[y%m]|~a[(y+1)%m];
		A[c]=x>>r|x<<(32-r)%32;
	}for(y=39;y--;b[y+1]=b[y])a[y%m]=A[y]^A[(y+1)%m]^A[(y+4)%m];*a^=1;
	for(c=0;c<3;c++)a[c+o]^=b[c*o]=b[c*o+o];}
void rg32_seed(uint32_t*u,uint32_t*w,char*v,uint32_t len,
	uint32_t cost){
	uint32_t s,q,c;
	for(c=0;c<40;c++)w[c]=u[c%19]=0;
	for(;;rg32_f(u,w)){
		for(c=0;c<3;c++){
			for(q=0;q<4;){	
				if(len > 0) { w[c*13]^=s=(255&*v++)<<8*q++; }
				else { w[c*13]^=s=1<<8*q++; }
				u[16+c]^=s; 
				if(len == 0){
				if(cost > 9999) {
					for(c=0;c<=cost;c++)rg32_f(u,w);
				} else {
					for(c=0;c<17;c++)rg32_f(u,w);
				}
					return;}len--;}}}}
uint32_t rg32_rand32(uint32_t*m,uint32_t*b,uint32_t*a){
	if(*a&2)rg32_f(m,b);
	return m[*a^=3];
}

/* Let's make a secure password hash from that */
/* Note: This outputs a string with a secure password.  It is up to
 * the caller to free() the string when it’s done with it. */
unsigned char *rg32crypt(unsigned char *password, unsigned char *salt) {
	unsigned char mysalt[12];
	unsigned char *usesalt = NULL;
	unsigned char *both = NULL;
	unsigned char *out = NULL;
	uint32_t cost = 32768; 
        uint32_t belt[40], mill[19], phase = 2;
	int a;

	if(password == NULL) { return NULL; } 
	if(salt == NULL) {
		FILE *rfile = NULL;
		rfile = fopen("/dev/urandom","rb");
		if(rfile == NULL) {
			puts("You do not have /dev/urandom");
			puts("I refuse to run under these conditions");
			exit(1);
		}
		mysalt[0] = '~';
		mysalt[1] = '~';
		for(a=2;a<10;a++) {
			unsigned char b;
			b = getc(rfile);
			// Simple to code: [0-9@A-Za-z\.]
			b &= 0x3f;
			b += '0'; 
			if(b > '9') { b += 6; }
			if(b > 'Z') { b += 6; }
			if(b > 'z') { b -= 77; }
			mysalt[a] = b;
		}	
		mysalt[10] = 0;
		usesalt = mysalt;
	} else {
		usesalt = salt;
	}
	if(usesalt[0] != '~' || usesalt[1] != '~') { return NULL; }
	if(strlen((char *)usesalt) < 10) { return NULL; }
	both = malloc(strlen((char *)password) + 14);
	if(both == NULL) {
		return NULL;
	}
	*both = 0;
	for(a = 0; a < 10; a++) {
		both[a] = usesalt[a];
	}
	both[10] = '+';
	both[11] = 0;
	strcat((char *)both, (char *)password);
	rg32_seed(mill, belt, (char *)both, strlen((char *)both), cost);
	free(both);
	out = malloc(34);
	if(out == NULL) {
		return NULL;
	}
	for(a = 0; a < 10; a++) {
		out[a] = usesalt[a];
	}
	for(a = 10; a < 32; a++) {
		uint32_t rand;
		unsigned char b;
		rand = rg32_rand32(mill, belt, &phase);
		b = (rand & 0x3f);
		b += '0';
		if(b > '9') { b += 6; }
		if(b > 'Z') { b += 6; }
		if(b > 'z') { b -= 77; }
		out[a] = b;
	}
	out[32] = 0;
	return out;
}	
	
/* Example of API usage, also public domain */
#include <stdio.h>
#include <stdlib.h>

int main(int argc,char **argv){
	unsigned char *pwHashed;
	
        if(argc < 2) {
                printf("Usage: rg32crypt '{password}' {salt}\n");
                exit(1);
        }

	if(argc < 3) {
		pwHashed = rg32crypt((unsigned char*)argv[1],0);
	} else {
		pwHashed = rg32crypt((unsigned char*)argv[1],
				(unsigned char*)argv[2]);
	}
	if(pwHashed != NULL) {
		puts((char *)pwHashed);
		free(pwHashed);
	} else {
		puts("Error generating hashed password");
	}
        return 0;
}
