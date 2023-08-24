#!/bin/bash

make main > /dev/null 2>&1
./main $1

rm main *.o
