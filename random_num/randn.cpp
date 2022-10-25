//
// Created by 13938 on 2022/10/21.
//
#include <fstream>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include "string"

using namespace std;
int main(){
    srand(time(NULL));
    string file_name = "E:\\codes\\agorthm\\random_num\\output.txt";
    ofstream ofs(file_name);
    for(int i = 0; i < 2000; ++i)
        ofs << rand()%100 << " " << rand()%150 << endl;
    return 0;
}