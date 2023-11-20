#!/usr/bin/env bash

cd ./SDL || exit
git pull
mkdir build
cmake -S . -B ./build
cmake --build ./build

