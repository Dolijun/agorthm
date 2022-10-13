#!/usr/bin/bash
g++  ./post_office.cpp -o post_office.exe
./post_office input.txt input2.txt input_assign01_05.dat

echo 按任意键继续...
read -n 1
echo 继续运行