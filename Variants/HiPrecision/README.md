# RadioGatún[128]

RadioGatún has two fully official variants, RadioGatún[32] for 32-bit
words, and RadioGatún[64] for 64-bit words.  RadioGatún is not officially
defined for longer word lengths, but the question remains: what would
a RadioGatún[128] variant look like.

This code uses a crude arbitrary precision library using linked lists 
to store the numbers being used, allowing us to examine ultra-long-word
RadioGatún variants.

Because the words are longer, we need an extra blank round to make sure
every bit of the mill is affected by every bit in the belt or mill.  Once
the words are 368 bits or longer (I have only tested up to 512 bits), we
need two extra blank rounds.

# Some test vectors

```
RadioGatun[16] (might not be secure)
"" = "cfca74586c5dd67feaecf3ddb3373a0ced4d8f8895daa76993c95914b7c5f698"
"1234" = "1f21ff8201c72b9c3e4b2aa34cc6a7f9dc3bdb886f5e160254f408653477354f"
RadioGatun[32] (secure as of 2020)
"" = "f30028b54afab6b3e55355d277711109a19beda7091067e9a492fb5ed9f20117"
"1234" = "9ebdd24f469993796c4aac6a821735a65a3cdef8a359944ce71f34e7a08e1182"
RadioGatun[64] (secure as of 2020)
"" = "64a9a7fa139905b57bdab35d33aa216370d5eae13e77bfcdd85513408311a584"
"1234" = "733e2b49a53fb166b6f3bd341919578b8c931880f8b8bd7c0fbbee1a538e7307"
RadioGatun[128] (unofficial variant)
"" = "e4d17d0122b62de47e95ab9cc992d1ee85ece69c26e7efe7719e8db52ee051a5"
"1234" = "f2a9b17683570ec19c4b181799cc46cc22ce170475cd097da29cc9c33dd73fa1"
```

RadioGatún[16], RadioGatún[32], and RadioGatún[64] have 18 blank rounds.
RadioGatún[128], on the other hand, has 19 blank rounds.

