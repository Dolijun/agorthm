#include<fstream>
#include <iostream>
#include<string>
#include<vector>

using namespace std;

/* Output file */
ofstream fout;

class Object {
public:
    int weight; // 物品的重量
    int value; // 物品的价值

    Object(); // 物品的默认构造

    Object(int weight, int value); // 物品类根据重量和价值构造
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
    int Capacity; // 背包的容量
    int N;  // 背包中待选物品的种类
    vector<Object> objects; // 待选物品的列表，记录每种物品的重量和价值

    FullBag();  // 背包的默认构造

    FullBag(string file_path); // 从文件读入背包容量，物品数量和各个物品的重量和价值。

    void get_result(); // 根据背包的信息，进行动态规划，得到背包最优的容纳物品重量和质量和每种物品选择的数量
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

    // 构建状态转移矩阵
    dp.reserve(N + 1);
    for (int i = 0; i <= N; ++i)
        dp.push_back(vector<int>(Capacity + 1, 0));

    num.reserve(N + 1);
    for (int i = 0; i <= N; ++i)
        num.push_back(0);

    // 自顶向下的动态规划过程
    for (int i = 1; i <= N; ++i)
        for (int j = 1; j <= Capacity; ++j) {
            int w = objects[i - 1].weight;
            int v = objects[i - 1].value;
            if (j < w)
                dp[i][j] = dp[i - 1][j];
            else
                dp[i][j] = max(dp[i - 1][j], dp[i][j - w] + v);
        }

    // 自底向上，反推各类别物品的数量
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
    string log_file = "./output.txt";
    fout = ofstream(log_file);

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

    cout << "*************************" << endl;
    fout << "*************************" << endl;

    return 0;
}



