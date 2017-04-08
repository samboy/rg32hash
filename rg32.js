/*
 * Copyright (c) 2010-2017 Sam Trenholme
 * 
 * TERMS
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * This software is provided 'as is' with no guarantees of correctness or
 * fitness for purpose.
 */

/* Example usage with Node:

var rg32 = require("./rg32.js");
var hash = new rg32.rg32("foo");
console.log(hash.hash256()); // 256-bit hash as string
console.log(hash.rng()); // 32-bit signed integer
console.log(hash.hexnumber()); // 32-bit hex number

Note that this uses charCodeAt to convert characters in to 8-bit (0 to 255)
bytes given to the hash, and converts any Unicode point above 255 (anything 
not in ISO 8859-1) in to its low eight bits.  If you want better Unicode 
compatibility, one solution is to use https://github.com/mathiasbynens/utf8.js 
to convert the string, e.g.

var utf8 = require('utf8');
var hash = new rg32.rg32(utf8.encode("¡Es una niña!"));
console.log(hash.hash256());

 */
function rg32(input) {
	var a = [];
	var b = [];
	
	function beltmill() {
		var ap, x, y, r, q, c, i;
		ap = [];
		q = [];

		for(c = 0; c < 3; c++) {
			q[c] = b[c * 13 + 12];
		}
		for(i = 12; i > 0; i--) {
			for(c = 0; c < 3; c++) {
				b[c * 13 + i] = b[c * 13 + i - 1];
			}
		}
		for(c = 0; c < 3; c++) {
			b[c * 13] = q[c];
		}
		for(c = 0; c < 12; c++) {
			i = c + 1 + ((c % 3) * 13);
			b[i] ^= a[c + 1];
		}
		for(c = 0; c < 19; c++) { // RG32 Mill
			y = (c * 7) % 19;
			r = (((c * c) + c) / 2) % 32;
			x = a[y] ^ (a[(y + 1) % 19] | 
					(a[(y + 2) % 19] ^ 0xffffffff));
			ap[c] = (x >>> r) | (x << (32 - r));
		}	
		for(c = 0; c < 19; c++) {
			a[c] = ap[c] ^ ap[(c + 1) % 19] ^ ap[(c + 4) % 19];
		}
		a[0] ^= 1;

		for(c = 0; c < 3; c++) {
			a[c+13]^=q[c];
		}

	}

	place = 1;
	function nextbyte() {
		var output = a[place];
		if(output < 0) {
			output += 0x100000000;
		}
		place++;
		if(place > 2) {
			place = 1;
			beltmill();
		}
		return output;
	}

	this.foo = input;

	// Return the next raw 32-bit number rg32 gives us
	this.rng = function() {
		var endian;
		endian = nextbyte();
		endian = ((endian & 0xff000000) >>> 24) |
			 ((endian & 0x00ff0000) >>>  8) |
			 ((endian & 0x0000ff00) <<  8) |
			 ((endian & 0x000000ff) << 24);
		return endian;
	};

	this.hexnumber = function() {
        	var z = this.rng();
        	if(z < 0) {
        	        z += 0x100000000;
        	}
        	return z.toString(16);
	}

	this.hash256 = function() {
		var z = "";
		for(var a = 0; a < 8; a++) {
			var y = this.hexnumber();
			// I find it amusing how many Node programs
			// broke because Javascript developers were too 
			// lazy to write the following one line of code
			// (As per http://archive.is/P0vJS )
			while(y.length < 8) { y = "0" + y; }
			z += y;
		}
		return z;
	}

	// Constructor: Input map
	var s, q, c, r, x, d, v, place;
	s = [];
	v = 0;
	d = 0;

	for(c = 0; c < 19; c++) {
		a[c] = 0;
	}
	for(c = 0; c < 39; c++) {
		b[c] = 0;
	}
	
	for(v = 0;v <= input.length;) {
		for(c = 0 ; c < 3 ; c++) { // Blank input words 
			s[c] = 0;
		}
		for(r = 0; r < 3; r++) {
			for(q = 0; q < 4; q++) {
				if(v < input.length) {
					x = input.charCodeAt(v);

					// While this stops high bit
					// characters from corrupting
					// the hash state, strings
					// with Unicode given to rg32
					// should be converted in to
					// UTF-8 first
					x &= 0xff;

					v++;
					s[r] |= x << (q * 8);
				} else {
					s[r] |= 1 << (q * 8);
					for(c = 0; c < 3; c++) {
						b[c * 13] ^= s[c];
						a[16 + c] ^= s[c];
					}	
					for(c = 0; c < 16; c++) {
						beltmill();
					}
					beltmill();
					beltmill();
					return;
				}
			}
		}
		for(c = 0; c < 3; c++) {
			b[c * 13] ^= s[c];
			a[16 + c] ^= s[c];
		}	
		beltmill();
	}

	// 16 blank rounds
	for(c = 0; c < 16; c++) {
		beltmill();
	}

}	

/* If you want to run this directly in a browser outside of node
 * Do something like var hash = new rg32("foo"); alert(hash.hexnumber());
 */

module.exports = {rg32: rg32};

