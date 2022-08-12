#!/bin/sh
# Wrapper so we can run rg32wiki.lua with SQA tests
LUA=lua

if which lunacy > /dev/null ; then
	LUA=lunacy
fi

cd ../Lua/
$LUA rg32wiki.lua "$1" | tail -1 | tr '[A-Z]' '[a-z]' | tr -d ' '
