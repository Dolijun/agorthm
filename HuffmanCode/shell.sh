#!/usr/bin/bash
g++  ./HuffmanCode.cpp -o HuffmanCode.exe
# 编码测试
./HuffmanCode encoding input.txt encoding.txt

# 解码测试
./HuffmanCode decoding encoding.txt output.txt

echo 按任意键继续...
read -n 1
echo 继续运行