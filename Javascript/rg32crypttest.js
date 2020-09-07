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

/* Usage: ./rg32crypttest 1 ~~12345678 */

var rg32crypt = require("./rg32crypt.js");
var password = process.argv[2];
var salt = process.argv[3];

r = rg32crypt.rg32crypt(password, salt);
console.log(r);
