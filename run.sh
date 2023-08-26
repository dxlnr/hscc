#!/bin/bash

make all > /dev/null 2>&1
./hscc $*

make clean > /dev/null 2>&1
rm hscc
