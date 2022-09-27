//
// Created by Mr.D on 2022/9/22.
//
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class Point {
public:
    int x;
    int y;
    int w;

    Point() {
        x = 0;
        y = 0;
        w = -1;
    }
    Point(string line){
//        cout << line.length() << endl;
//        cout << line << endl;
        int temp = 0;
        int counter = 0;
        int temp_len = 0;
        for(int i=0; i<line.length(); ++i){
            if(line[i] != ' '){
                temp = temp*10 + (line[i] - '0');
                ++temp_len;
            }else{
                if(temp_len > 0){
                    if(counter == 0)
                        x = temp;
                    else if(counter == 1)
                        y = temp;
                    else if(counter == 2)
                        w = temp;
                }
                temp = 0;
                temp_len = 0;
                ++counter;
            }
        }
        if(temp_len > 0){
            if(counter == 0)
                x = temp;
            else if(counter == 1)
                y = temp;
            else if(counter == 2)
                w = temp;
        }
        ++counter;
        if(counter != 3){
            cout << "more than 3 int" << endl;
            return ;
        }

    }
    Point(Point &p) {
        x = p.x;
        y = p.y;
        w = p.w;
    }
};
using Point_ptr = Point *;

class Village {
private:
    Point_ptr people;
    int N;

public:
    Village();

    Village(string filename);

    static bool cmp_x(Point_ptr x, Point_ptr y);

    static bool cmp_y(Point_ptr x, Point_ptr y);

    void QuickSort(Point_ptr *array, int low, int high, bool(*func)(Point_ptr p1, Point_ptr p2));

    void get_post(){
        Point_ptr post = new Point();
        int sum_weight = 0;
        int temp_sum = 0;
        int min_weight = 0;
        Point_ptr * ptrs = new Point_ptr [N];
        for(int i = 0; i < N; ++i){
            ptrs[i] = &people[i];
            sum_weight += ptrs[i]->w;
        }

        QuickSort(ptrs, 0, N-1, cmp_x);
        int i;
        for( i = 0; i < N; ++i){
            temp_sum += ptrs[i]->w;
            if(temp_sum * 2 >= sum_weight)
                break;
        }
        post->x = ptrs[i]->x;

        temp_sum = 0;
        QuickSort(ptrs, 0, N-1, cmp_y);
        for( i = 0; i < N; ++i){
            temp_sum += ptrs[i]->w;
            if(temp_sum * 2 >= sum_weight)
                break;
        }
        post->y = ptrs[i]->y;

        for(i = 0; i < N; ++i)
            min_weight += ptrs[i]->w * (abs(ptrs[i]->x - post->x) + abs(ptrs[i]->y - post->y));


        cout << "best positon:" << "x=" << post->x << " y=" << post->y << endl;
        cout << "min_weight:" << min_weight << endl;
    }
};

Village::Village(string filename) {
    ifstream in_file(filename);
    if(!in_file.is_open()){
        cout << "open fail" <<endl;
        return;
    }else{
        cout << "open success" << endl;
        string buf;
        getline(in_file, buf);
        N = int(buf[0] - '0');
        // build array
        people = new Point[N];
        for(int i=0; i<N; ++i){
            getline(in_file, buf);
            people[i] = Point(buf);
        }
    }
}

bool Village::cmp_x(Point_ptr p1, Point_ptr p2) {
    return p1->x <= p2->x;
}

bool Village::cmp_y(Point_ptr p1, Point_ptr p2) {
    return p1->y <= p2->y;
}

void Village::QuickSort(Point_ptr *array, int low, int high, bool(*cmp)(Point_ptr p1, Point_ptr p2)) {
    if (low >= high)
        return;
    int i = low;
    int j = high;
    Point_ptr key = array[low];

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

int main(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        cout << "*************************" << endl;
        cout << argv[i] << endl;
//        string file_name = "F:\\codes\\agorthm\\post_office_problem\\input.txt";
        Village  v(argv[i]);
        v.get_post();
    }
    return 0;
}
