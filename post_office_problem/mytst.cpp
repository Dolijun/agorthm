//
// Created by Mr.D on 2022/9/27.
//
#include <iostream>

using namespace std;

bool cmp_x()

void QuickSort(int *array, int low, int high) {
    if (low >= high)
        return;
    int i = low;
    int j = high;
    int key = array[low];
    while (i < j) {
        while (array[j] >= key && i < j)
            --j;
        array[i] = array[j];
        while (array[i] <= key && i < j)
            ++i;
        array[j] = array[i];
    }
    array[i] = key;
    QuickSort(array, low, i - 1);
    QuickSort(array, i + 1, high);
}

// test
int main() {
    int array[] = {49, 38, 65, 97, 76, 13, 27, 49};
    int length = sizeof(array) / sizeof(*array);
    cout << "Origin:";
    for (int i = 0; i < length; i++) {
        cout << array[i] << " ";
    }
    cout << endl;
    QuickSort(array, 0, length - 1);
    cout << "QuickSort:";
    for (int i = 0; i < length; i++) {
        cout << array[i] << " ";
    }
    return 0;
}