#!/bin/bash

g++ -c src/*.cpp -std=c++14 -m64 -O0 -g -Wall -I include && g++ *.o -o bin/debug/main.sh -lSDL2main -lSDL2 -lSDL2_image && ./bin/debug/main.sh