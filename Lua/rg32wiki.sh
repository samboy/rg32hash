#!/bin/sh
# Wrapper so we can run rg32wiki.lua with SQA tests

# Note: rg32wiki is the code at 
# https://en.wikipedia.org/wiki/Module:RadioGatun32

LUA=lua

if which lua5.3 > /dev/null ; then
	LUA=lua5.3
elif which lua-5.3 > /dev/null ; then
	LUA=lua-5.3
elif which lunacy > /dev/null ; then
	LUA=lunacy
fi

cd ../Lua/
$LUA rg32wiki.lua "$1" | tail -1 | tr '[A-Z]' '[a-z]' | tr -d ' '
