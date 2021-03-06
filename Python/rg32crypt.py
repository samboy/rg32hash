#!/usr/bin/env python3

# vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4

from rg32 import RadioGatun32
#test_rg32crypt = 1

class rg32crypt:
    def __init__(self,password,salt):
        self.base64 = (
            "0123456789@ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz.")
        m = salt[0:10] + "+" + password
        self.r = RadioGatun32(m)
        self.r.runbelt(32752)
        self.hash = salt
        for a in range(22):
            self.hash += self.b64()
    # Get a base64 char from RadioGatun
    def b64(self):
        a = self.r.rng32()
        a = a >> 24;
        a = (a & 0x3f);
        a = self.base64[a]
        return a

#if test_rg32crypt == 1:
#    import sys
#    x = rg32crypt(sys.argv[1],sys.argv[2])
#    print(x.hash)
