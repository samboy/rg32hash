#!/usr/bin/env lua
-- This is a hybrid C and Lua implementation of rg32crypt
-- The cryptograpic “heavy lifting” is done by C; the glue to make
-- a proper rg32crypt password is done in Lua
require("rg32")
function rg32crypt(password, salt)
  local mypass = string.sub(salt, 1, 10) .. "+" .. password
  rg32.randomseed(mypass)
  rg32.runmill(32752)
  local out = string.sub(salt, 1, 10)
  for a = 1, 22 do
    local base64 = 
            "0123456789@ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz."
    local me = rg32.rand32()
    me = math.floor(me / 16777216)
    me = me % 64;
    me = me + 1;
    out = out .. string.sub(base64, me, me) 
  end
  return out
end
--[[
Example usage:
require("rg32crypt")
print(rg32crypt("1","~~12345678"))

Here, "1" is the password, and "~~12345678" is the salt.
]]
