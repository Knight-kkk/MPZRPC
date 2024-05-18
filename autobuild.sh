#!/bin/bash
rm `pwd`/log/*
rm `pwd`/build/*
cd `pwd`/build/ &&
cmake .. &&
make &&
cp -r ../src/include ../lib