#!/usr/bin/env node
/*
 * Copyright (c) 2020 Sam Trenholme
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

var rg32 = require("./rg32.js");

function rg32crypt(password, salt) {
	var base64 = 
	    "0123456789@ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz.";
	var mypass = salt.substr(0, 10) + "+" + password;
	var r = new rg32.rg32(mypass);
	r.runbelt(32752);
	var a = 0;
	var me = 0;
	this.out = salt.substr(0, 10);
	for(a = 0; a < 22; a++) {
		me = (r.rng() >>> 24) & 0x3f;
		this.out = this.out + base64.substr(me, 1);
	}
	return this.out;
}

module.exports = {rg32crypt: rg32crypt};
		
