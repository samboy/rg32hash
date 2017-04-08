#!/usr/bin/env node
/*
 * Copyright (c) 2017 Sam Trenholme
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
var i = process.argv[2];

// The following bit of code requires the utf8 module at
// https://www.npmjs.com/package/utf8
// If you do not care about correct hashes for UTF-8 strings with
// non-ASCII characters, comment out the following two lines
var utf8 = require('./utf8.js');
i = utf8.encode(i);

var hash = new rg32.rg32(i); 
console.log(hash.hash256());
