#!/bin/bash

make clean
make

echo "=== 5.1"
./hw3 < sample1.input

echo "=== 5.2"
./hw3 < sample2.input

echo "=== 5.3"
./hw3 < sample3.input

echo "=== 5.4"
./hw3 < sample4.input

echo "=== textbook"
./hw3 < textbook.input

