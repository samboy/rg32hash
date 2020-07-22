-- Pure Lua implementation of bit32 for Lua 5.4+
bit32 = {}
bit32.band = function(a,...) 
  for _,v in pairs({...}) do a = a & v end
  return a & 0xffffffff
end
bit32.bor = function(a,...) 
  for _,v in pairs({...}) do a = a | v end
  return a & 0xffffffff
end
bit32.bxor = function(a,...) 
  for _,v in pairs({...}) do a = a ~ v end
  return a & 0xffffffff
end
bit32.bnot = function(a) return ~a & 0xffffffff end
bit32.lshift = function(a,r) return (a << r) & 0xffffffff end
bit32.rshift = function(a,r) return (a & 0xffffffff) >> r end
bit32.rrotate = function(a,r)
  r = r % 32
  a = a & 0xffffffff
  if r == 0 then return a end
  return ((a >> r) | (a << (32 - r))) & 0xffffffff
end
bit32.arshift = function(a,r)
  if (a & 0x80000000) == 0 then return (a & 0xffffffff) >> r end
  a = a & 0xffffffff
  for i=1,r do
     a = a >> 1
     a = a | 0x80000000
  end
  return a
end
