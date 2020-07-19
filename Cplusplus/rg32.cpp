// Radio Gatun 32 hash/stream cipher/PRNG 
// Written in 2009 and donated to the public domain 2017 by 
// Sam Trenholme

#include <iostream>
#include <stdint.h>
#include <stdio.h>

// RadioGatun 32 class

class RG32
{
public:
	RG32 (const char *data);
	uint32_t Num ();
private: 
	void init();
	void mill();
	void belt();
	void input_char(char a);
	uint32_t a[20]; // Mill
	uint32_t b[40]; // Belt	
	uint32_t iword; // Input word
	uint32_t p[3]; // Input words (we add three at a time)
	int place; // Place of byte in input word
	int mplace; // Word from mill to use in output
	int pword; // Word (we input three at a time) to add input to
};

RG32::RG32(const char *in) 
{
	int c = 0;
	RG32::init();
	while(*in != 0) 
	{
		RG32::input_char(*in);
		in++;
	}
	RG32::input_char(1); // 1 at end
	if(pword == 0 && iword == 0) 
	{
		for(c = 0; c < 16; c++) 
		{
			RG32::belt();
		}
		return;
	}
	p[pword % 3] = iword;
	for(c = 0; c < 3; c++) // Map input to belt and mill
	{
		b[c * 13] ^= p[c];
		a[16 + c] ^= p[c];
	}
	for(c = 0; c < 17; c++) 
	{
		RG32::belt();
	}

}

uint32_t RG32::Num() 
{
	uint32_t out;
	if(mplace < 1 || mplace > 2) {
		RG32::belt();
		mplace = 1;
	}
	out = a[mplace];
	mplace++;
	// Endian swap so the test vectors match up
	out = (out << 24) | ((out & 0xff00) << 8) | ((out & 0xff0000) >> 8) |
		(out >> 24);
	return out;
}

void RG32::init ()
{
	int z;
	for(z = 0; z < 20; z++) 
	{
		a[z] = 0;
	}
	for(z = 0; z < 40; z++)
	{
		b[z] = 0;
	}	
	place = 0;
	iword = 0;
	pword = 0;
	mplace = 0;
}

void RG32::mill () 
{
	uint32_t A[19];
	uint32_t x;
	int i = 0, y = 0, r = 0, z = 0, q = 0;
	for(i = 0; i < 19; i++) 
	{
		y = (i * 7) % 19;
		r = ((i * (i + 1)) / 2) % 32;
		x = a[y] ^ (a[ ((y + 1) % 19) ] | (~a [ ((y + 2) % 19) ]));
		A[i] = (x >> r) | (x << (32 - r));
	}
	for(i = 0; i < 19; i++) 
	{
		y = i;
		z = (i + 1) % 19;
		q = (i + 4) % 19;
		a[i] = A[y] ^ A[z] ^ A[q];
	}
	a[0] ^= 1;
}

void RG32::belt () 
{
	uint32_t q[3];
	int s = 0, i = 0, v = 0;
	for(s = 0; s < 3; s++) 
	{
		q[s] = b[((s * 13) + 12)];
	}
	for(i = 12; i > 0; i--) 
	{
		for(s = 0; s < 3; s++) 
		{
			v = i - 1;
			if(v < 0) 
			{
				v = 12;
			}
			b[(s * 13) + i] = b[(s * 13) + v];
		}
	}
	for(s = 0; s < 3; s++) 
	{
		b[(s * 13)] = q[s];
	}
	for(i = 0; i < 12; i++) 
	{
		s = (i + 1) + ((i % 3) * 13);
		b[s] ^= a[(i + 1)];
	}
	RG32::mill();
	for(i = 0; i < 3; i++) 
	{
		a[(i + 13)] ^= q[i];
	}
}

void RG32::input_char(char input) 
{
	int q = 0, c = 0, r = 0, done = 0;

	q = input;
	q &= 0xff;
	place++;

	if(place == 1) 
	{
		iword |= q;	
		return;
	}
	else if(place == 2) 
	{
		iword |= q << 8;
		return;
	}
	else if(place == 3) 
	{
		iword |= q << 16;
		return;
	}
	else if(place > 5) // Shouldn't ever happen
	{ 
		place = 0;
		iword = 0;
		return;
	}

	iword |= q << 24; 

	p[pword] = iword;
	iword = 0;
	place = 0;
	pword++;
	if(pword < 3) 
	{
		return;
	}
	else if(pword > 3) // Shouldn't ever happen
	{
		pword = 0;
		return;
	}

	// iword and pword done; run the belt and continue
	for(c = 0; c < 3; c++) // Map input to belt and mill
	{
		b[c * 13] ^= p[c];
		a[16 + c] ^= p[c];
		p[c] = 0;
	}
	RG32::belt();
	pword = 0;
	place = 0;
	return;
	
}

// The following just makes sure we generate correct numbers
int show_num(const char *num) {
	RG32 *state;

	state = new RG32(num);
	printf("%s %x\n",num,state->Num());
	delete state;
}

int main ()
{
	show_num("");
	show_num("1");
	show_num("12");
	show_num("123");
	show_num("1234");
	show_num("12345");
	show_num("123456");
	show_num("1234567");
	show_num("12345678");
	show_num("123456789");
	show_num("1234567890");
	show_num("12345678901");
	show_num("123456789012");
	show_num("1234567890123");
	show_num("12345678901234");
	show_num("123456789012345");
	show_num("1234567890123456");
	show_num("12345678901234567");
	show_num("123456789012345678");
	show_num("1234567890123456789");
	show_num("12345678901234567890");
	show_num("123456789012345678901");
	show_num("1234567890123456789012");
	show_num("12345678901234567890123");
	show_num("123456789012345678901234");
	show_num("SECOND CRYPTOGRAPHIC HASH WORKSHOP");
	show_num("August 24-25, 2006");
	show_num("Santa Barbara, California");

	RG32 *test;
	test = new RG32("Santa Barbara, California");
	printf("%x\n",test->Num());
	printf("%x\n",test->Num());
	printf("%x\n",test->Num());
	printf("%x\n",test->Num());
	printf("%x\n",test->Num());
	printf("%x\n",test->Num());
	printf("%x\n",test->Num());
	printf("%x\n",test->Num());
		
}
