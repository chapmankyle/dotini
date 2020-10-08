#!/bin/bash

g++ test/main.cpp src/ini.cpp -o main
./main
rm main
