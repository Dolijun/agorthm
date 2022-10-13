/*******************************
Author: Donglijun S202274024
Data: 2022-09-28
Description: Post Office Problem
*******************************/
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

/* Output file */
ofstream fout;

/* Residents */
class Point {
public:
    int x, y;   /* Coordinate of point */
    int w;      /* Weight of point */

    Point();    /* Default initialization */

    Point(int x, int y, int w);     /* Initialize with coordinates and weights */

    Point(Point &p);        /* Copy initialization */
};

using ptr_Point = Point *;

/* Village, Store information for all residents */
class Village {
private:
    ptr_Point people;   /* Point to all residents */
    int N;              /* Number of residents */

public:
    Village();      /* Default initialization */

    Village(string filename);       /* Initialization from file */

    ~Village();     /* Destructor of Village */

    static bool cmp_x(ptr_Point x, ptr_Point y);        /* Compare x-axis coordinate values */

    static bool cmp_y(ptr_Point x, ptr_Point y);        /* Compare y-axis coordinate values */

    void
    QuickSort(ptr_Point *array, int low, int high, bool(*func)(ptr_Point p1, ptr_Point p2)); /* Sort using quick sort */

    void get_post();        /* Get Post Office Coordinates according to residential coordinates */
};

Point::Point() {
/*
Description: Default constructor. The initialization coordinate value and weight are (0, 0) and -1 respectively
*/
    x = 0;
    y = 0;
    w = -1;
}

Point::Point(int x, int y, int w) {
/*
Description: Initialize coordinate value and weight with input.
Input:  int x, int y, int w: Point coordinate and weight
*/
    this->x = x;
    this->y = y;
    this->w = w;
}

Point::Point(Point &p) {
/*
Description: Copy constructor. Copy coordinate and weight from Point p
Input:  p: Reference of Point class
*/
    x = p.x;
    y = p.y;
    w = p.w;
}


Village::Village(string filename) {
/*
Description: Initialization from file
Input: filename: input file path
        The first line represents the number of points N
        Remaining N rows represents the coordinate values and weights of the N points
*/
    ifstream ifs(filename);
    if (!ifs.is_open()) {
        cout << "open fail" << endl;
        fout << "open fail" << endl;
        return;
    } else {
        ifs >> N;
        if (N == 0) {
            cout << "Illegal input: N==0 " << endl;
            fout << "Illegal input: N==0 " << endl;

            return;
        }
        cout << "There are " << N << " residents in total." << endl;
        cout << "coordinate\tweight" << endl;
        fout << "There are " << N << " residents in total." << endl;
        fout << "coordinate\tweight" << endl;
        people = new Point[N];
        int x, y, w;
        for (int i = 0; i < N; ++i) {
            ifs >> x >> y >> w;
            people[i] = Point(x, y, w);
            cout << "( " << x << ", " << y << " )\t" << w << endl;
            fout << "( " << x << ", " << y << " )\t" << w << endl;
        }
    }
}

Village::~Village() {
/*
Description: Destructor of class Village, delete heap data
*/
    if (people != NULL) {
        delete people;
        people = NULL;
    }
}

bool Village::cmp_x(ptr_Point &p1, ptr_Point &p2) {
/*
Description: Compare x-axis coordinate values
Input: ptr_Point p1, p2: points to be compared
*/
    return p1->x <= p2->x;
}

bool Village::cmp_y(ptr_Point &p1, ptr_Point &p2) {
/*
Description: Compare y-axis coordinate values
Input: ptr_Point p1, p2: points to be compared
*/
    return p1->y <= p2->y;
}

void Village::QuickSort(ptr_Point *array, int low, int high, bool(*cmp)(ptr_Point p1, ptr_Point p2)) {
/*
Description: Sort array according to function cmp
Input: 
    array: Array to be sorted
    low: Start position of the array to be sorted
    high: End position of the array to be sorted
    cmp: Comparison function
*/

    if (low >= high)
        return;
    int i = low;
    int j = high;
    ptr_Point key = array[low];

    while (i < j) {
        while (cmp(key, array[j]) && i < j)
            --j;
        array[i] = array[j];
        while (cmp(array[i], key) && i < j)
            ++i;
        array[j] = array[i];
    }
    array[i] = key;
    QuickSort(array, low, i - 1, cmp);
    QuickSort(array, i + 1, high, cmp);
}

void Village::get_post() {
/*
Description: Display optimal post office coordinates on standard output
*/

    int sum_weight = 0;     /* the sum of all point weights */
    int temp_sum = 0;       /* the sum weights of temporary points */
    int min_weight = 0;
    ptr_Point post = new Point();
    ptr_Point *ptrs = new ptr_Point[N];   /* Pointer array of all residents */
    for (int i = 0; i < N; ++i) {
        ptrs[i] = &people[i];
        sum_weight += ptrs[i]->w;
    }

    QuickSort(ptrs, 0, N - 1, cmp_x);     /* Sort Pointer array according to X coordinate value */
    int i;
    for (i = 0; i < N; ++i) {
        temp_sum += ptrs[i]->w;
        if (temp_sum * 2 >= sum_weight)
            break;
    }
    post->x = ptrs[i]->x;       /* optimal X coordinate value */

    temp_sum = 0;
    QuickSort(ptrs, 0, N - 1, cmp_y);     /* Sort Pointer array according to Y coordinate value */
    for (i = 0; i < N; ++i) {
        temp_sum += ptrs[i]->w;
        if (temp_sum * 2 >= sum_weight)
            break;
    }
    post->y = ptrs[i]->y;       /* optimal Y coordinate value */

    for (i = 0; i < N; ++i)      /* The total weighted distance from all residents to the optimal post office  */
        min_weight += ptrs[i]->w * (abs(ptrs[i]->x - post->x) + abs(ptrs[i]->y - post->y));


    cout << "The best post office positon:" << "( " << post->x << ", " << post->y << " )" << endl;
    cout << "The total weighted distance:" << min_weight << endl;
    fout << "The best post office positon:" << "( " << post->x << ", " << post->y << " )" << endl;
    fout << "The total weighted distance:" << min_weight << endl;
}

int main(int argc, char **argv) {
    fout = ofstream("./output.txt");
    for (int i = 1; i < argc; i++) {
        cout << "*************************" << endl;
        cout << "Input filename: " << argv[i] << endl;

        fout << "*************************" << endl;
        fout << "Input filename: " << argv[i] << endl;
        fout << "Output filename: output.txt" << endl;
        Village v(argv[i]);
        v.get_post();
    }
    cout << "*************************" << endl;
    fout << "*************************" << endl;

    return 0;
}
