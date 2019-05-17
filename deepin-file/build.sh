#!/bin/sh

PLATFORM=$(uname -s)
CXXFLAGS=-std=c++11

if [ $PLATFORM == "Linux" ]; then
    CXX=g++
elif [ $PLATFORM == "Darwin" ]; then
    CXX=clang++
else
    echo "unknown platform"
fi

echo "$CXX $CXXFLAGS *.cc -lpthread"

$CXX -std=c++11 *.cc -lpthread
