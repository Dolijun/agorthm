#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

ofstream fout;

class Container {
public:
    int N;
    int sum=10000000,tem=0;
    vector<vector<int>> job_salary;
    vector<bool> job_assignment;
    Container();

    Container(int N);

    void assign_job(int t);

};

Container::Container() {
    N = 0;
}

Container::Container(int N) {
    this->N = N;
    this->job_salary.reserve(N + 1);
    this->job_assignment.reserve(N + 1);
    this->job_assignment.assign(N, 0);
}

void Container::assign_job(int t) {
    if(t == N and tem < sum) {
        sum = tem;
        return;
    }
    for(int i = 0; i < N; ++i){
        if(job_assignment[i] == 0){
            tem += job_salary[t][i];
            job_assignment[i] = 1;
            if(tem < sum) assign_job(t+1);
            tem = tem - job_salary[t][i];
            job_assignment[i] = 0;
        }
    }
}

Container *load_from_file(string in_file) {
    int N;
    ifstream ifs;
    ifs.open(in_file, ios::in);
    if (!ifs.is_open()) {
        cout << "file open error" << endl;
        fout << "file open error" << endl;
    }
    ifs >> N;
    Container *container = new Container(N);
    int temp_int;
    for (int i = 0; i < N; ++i) {
        vector<int> temp;
        temp.reserve(N + 1);
        for (int j = 0; j < N; ++j) {
            ifs >> temp_int;
            temp.push_back(temp_int);
        }
        container->job_salary.push_back(temp);
    }
    return container;
}

int main() {
    string log_file = "./output.txt";
    fout.open(log_file, ios::out);
    Container *container;
    string in_file = "E:\\codes\\agorthm\\job_assignment_problem\\input.txt";
    container = load_from_file(in_file);
    container->assign_job(0);
    cout << container->sum << endl;
}