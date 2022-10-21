#include<fstream>
#include <iostream>
#include<string>
#include<vector>

using namespace std;

/* Output file */
ofstream fout;

class Object {
public:
    int weight;
    int value;

    Object();

    Object(int weight, int value);
};

Object::Object() {
    weight = 0;
    value = 0;
}

Object::Object(int weight, int value) {
    this->weight = weight;
    this->value = value;
}


class FullBag {
public:
    int N;
    int Capacity;
    vector<Object> objects;

    FullBag();

    FullBag(string file_path);

    void get_result();
};

FullBag::FullBag() {
    N = 0;
    Capacity = 0;
}

FullBag::FullBag(string file_path) {
    ifstream ifs(file_path);
    cout << "*************************" << endl;
    fout << "*************************" << endl;
    if (!ifs.is_open()) {
        cout << "open fail" << endl;
        fout << "open fail" << endl;
        return;
    } else {
        ifs >> Capacity >> N;
        if (Capacity <= 0) {
            cout << "Illegal Input: Capacity" << endl;
            fout << "Illegal Input: Capacity" << endl;
        } else if (N <= 0) {
            cout << "Illegal Input: N(umber of objects)" << endl;
            fout << "Illegal Input: N(umber of objects)" << endl;
        } else {
            this->objects.reserve(N);
            int w, v;
            for (int i = 0; i < N; ++i) {
                ifs >> w >> v;
                objects.push_back(Object(w, v));
                cout << "Object " << i + 1 << " weight:" << w << " value: " << v << endl;
                fout << "Object " << i + 1 << " weight:" << w << " value: " << v << endl;
            }
        }

    }
}

void FullBag::get_result() {
    vector<vector<int>> dp;
    vector<int> num;

    dp.reserve(N + 1);
    for (int i = 0; i <= N; ++i)
        dp.push_back(vector<int>(Capacity + 1, 0));

    num.reserve(N + 1);
    for (int i = 0; i <= N; ++i)
        num.push_back(0);


    for (int i = 1; i <= N; ++i)
        for (int j = 1; j <= Capacity; ++j) {
            int w = objects[i - 1].weight;
            int v = objects[i - 1].value;
            if (j < w)
                dp[i][j] = dp[i - 1][j];
            else
                dp[i][j] = max(dp[i - 1][j], dp[i][j - w] + v);
        }
    int j = Capacity;
    for (int i = N; i > 0; --i) {
        int w = objects[i - 1].weight;
        int v = objects[i - 1].value;
        while (j - w >= 0)
            if ((dp[i][j - w] + v) > dp[i - 1][j]) {
                j -= w;
                num[i] += 1;
            } else
                break;
    }
    int totle_value = dp[N][Capacity];
    int totle_weight = 0;
    for (int i = 1; i <= N; ++i)
        totle_weight += objects[i - 1].weight * num[i];
    cout << "*************************" << endl;

    for (int i = 1; i <= N; ++i)
        cout << "Object " << i << " num: " << num[i] << endl;
    cout << "Totle value: " << totle_value << endl;
    cout << "Totle weight: " << totle_weight << endl;

    fout << "*************************" << endl;
    for (int i = 1; i <= N; ++i)
        fout << "Object " << i << " num: " << num[i] << endl;
    fout << "Totle value: " << totle_value << endl;
    fout << "Totle weight: " << totle_weight << endl;

}

int main(int argc, char **argv) {
    string out_file = "./output.txt";
    fout = ofstream(out_file);

    for (int i = 1; i < argc; i++) {
        cout << "**********************" << endl;
        cout << "Input filename: " << argv[i] << endl;
        cout << "Output filename: ./output.txt" << endl;

        fout << "*************************" << endl;
        fout << "Input filename: " << argv[i] << endl;
        fout << "Output filename: ./output.txt" << endl;
        FullBag bag(argv[i]);
        bag.get_result();
    }

//    string curcwd = "E:\\codes\\agorthm\\full_bag";
//    FullBag bag("E:\\codes\\agorthm\\full_bag\\input.txt");
//    bag.get_result();

    cout << "*************************" << endl;
    fout << "*************************" << endl;

    return 0;
}



