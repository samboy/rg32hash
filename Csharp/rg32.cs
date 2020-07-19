// Placed in the public domain by Sam Trenholme on 2017-07-08
// This is a .NET (C#) implementation of RadioGatún[32]
// Note that the .NET library appears to convert UTF-8 in to CP-1252

using System;

namespace RG32
{
    public class RG32
    {
        private uint[] belt;
        private uint[] mill;
        private int phase;
        private int rtate = 0;
        private int place = 0;
        private uint iword = 0;

        private void beltmill()
        {
            int a;
            int rotate = 0;
            int index = 0;
            uint[] millcopy;
            millcopy = new uint[19];
            uint temp = 0;
            for(a = 0; a < 12; a++)
            {
                belt[a + ((a % 3) * 13)] ^= mill[a + 1];
            }
            for(a = 0; a < 19; a++)
            {
                rotate = (rotate + a) % 32;
                index = (a * 7) % 19;
                temp = mill[index];
                temp ^= mill[(index + 1) % 19] | ~mill[(index + 2) % 19];
                millcopy[a] = (temp >> rotate) | (temp << (32 - rotate));
            }
            for(a = 38; a >= 0; a--)
            {
                belt[a + 1] = belt[a];
            }
            for(a = 0; a < 19; a++)
            { 
                mill[a] = millcopy[a];
                mill[a] ^= millcopy[(a + 1) % 19];
                mill[a] ^= millcopy[(a + 4) % 19];
            }
            mill[0] ^= 1;
            for(a = 0; a < 3; a++) 
            {
                belt[a * 13] = belt[(a * 13) + 13];
                mill[a + 13] ^= belt[a * 13];
            }
        }

        private void inputMap(byte i)
        {
            iword |= ((uint)i << rtate);
            rtate += 8;
            if (rtate > 24)
            {
                rtate = 0;
                belt[place * 13] ^= iword;
                mill[place + 16] ^= iword;
                iword = 0;
                place++;
                if (place > 2)
                {
                    place = 0;
                    beltmill();
                }
            }
        }

        private void postInputMap()
        {
            int a;
            if (rtate > 0)
            {
                belt[place * 13] ^= iword;
                mill[place + 16] ^= iword;
            }
            if(place != 0 || rtate != 0)
            {
                beltmill();
            }
            for(a = 0; a < 16; a++)
            {
                beltmill();
            }
        }

        public RG32(string input)
        {
            int a;
            belt = new uint[40];
            mill = new uint[19];
            phase = 2;
            for(a = 0; a < 40; a++)
            {
                belt[a] = 0;
            }
            for(a = 0; a < 19; a++)
            {
                mill[a] = 0;
            }
            foreach(byte c in input)
            {
                inputMap(c);
            }
            inputMap(1);
            postInputMap();
        }

        private uint getWord()
        {
            phase ^= 3;
            if(phase == 1)
            {
                beltmill();
            }
            uint endianSwap = mill[phase];
            return (endianSwap << 24) | 
                   ((endianSwap & 0xff00) << 8) |
                   ((endianSwap & 0xff0000) >> 8) |
                   (endianSwap >> 24);
        }

        public string hash()
        {
            int a;
            string o = "";
            for(a = 0; a < 8; a++)
            {
                uint b = getWord();
                o += b.ToString("x8");
            }
            return o;
        }
    }
}
namespace RG32test
{
    class Program
    {
        static void Main(string[] args)
        {
            if(args.Length < 1)
            {
                Console.WriteLine("Usage: rg32test {hash input}");
                Console.ReadKey(); // DEBUG
                Environment.Exit(1);
            }
            RG32.RG32 r32 = new RG32.RG32(args[0]);
            Console.WriteLine(r32.hash());
        }
    }
}
