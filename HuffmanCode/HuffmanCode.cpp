#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<map>
#include<set>
#include<algorithm>
#include<stack>
#include<bitset>

using namespace std;

#define MaxLen 100000

ofstream fout;

class Node {
public:
    char data;
    long weight;
    Node *parent;
    Node *l_child;
    Node *r_child;

    Node();

    Node(char data, long weight);

    Node(Node &temp);

    Node(Node *l_child, Node *r_child);

    static bool cmp(Node *a, Node *b);
};

Node::Node(char data, long weight) {
    this->data = data;
    this->weight = weight;
    parent = nullptr;
    l_child = nullptr;
    r_child = nullptr;
}

Node::Node(Node &temp) {
    data = temp.data;
    weight = temp.weight;
    parent = temp.parent;
    l_child = temp.l_child;
    r_child = temp.r_child;
}

Node::Node(Node *l_child, Node *r_child) {
    data = '0';
    weight = l_child->weight + r_child->weight;
    this->l_child = l_child;
    this->r_child = r_child;
    parent = nullptr;
    l_child->parent = this;
    r_child->parent = this;
}

bool Node::cmp(Node *a, Node *b) {
    return a->weight > b->weight;
}

using NodePtr = Node *;

// 从输入文件读入需要编码的文本
string read_from_file(string in_file) {

    ifstream ifs;
    char *buffer = new char[MaxLen];
    ifs.open(in_file, ios::in | ios::binary);
    if (!ifs.is_open()) {
        cout << "file open error" << endl;
        fout << "file open error" << endl;
        return nullptr;
    }
//    ifs.read(buffer, 10000);
    int i;
    for (i = 0; i < MaxLen; ++i) {
        if (ifs.eof())
            break;
        buffer[i] = ifs.get();
    }
    buffer[i - 1] = '\0';

    string in_str(buffer);
    delete[] buffer;
    ifs.close();
    return in_str;
}

// 解析文件字符串
void parse_file(vector<Node *> &nodes, string &in_str) {
    set<char> keys;
    map<char, int> dict;
    for (string::iterator idx = in_str.begin(); idx != in_str.end(); ++idx) {
        if (!keys.count(*idx)) {
            keys.insert(*idx);
            dict.insert(pair<char, int>(*idx, 1));
        } else
            dict[*idx] += 1;
    }
    nodes.reserve(dict.size());
    for (auto idx = dict.begin(); idx != dict.end(); ++idx) {
        nodes.push_back(new Node(idx->first, idx->second));
    }
    sort(nodes.begin(), nodes.end(), Node::cmp);
}


class Huffman {
public:
    NodePtr tree;
    map<char, string> huff_dict;

    // 根据字符的权重构建 Huffman 树
    Huffman(vector<Node *> &nodes);

    // 根据 huffman 字典构建 Huffman 树
    Huffman(map<char, string> &dict);

    void creatHuff(vector<Node *> &nodes); //构造哈夫曼树
    void createHuffDict(vector<Node *> &nodes); //哈夫曼树生成哈夫曼编码字典
    void insert_node(vector<Node *> &nodes, NodePtr temp_node);

    static void encoding(string in_file, string out_file);
    static void decoding(string in_file, string out_file);

};

Huffman::Huffman(vector<Node *> &nodes) {
    creatHuff(nodes);
}

Huffman::Huffman(map<char, string> & huff_dict) {
    this->huff_dict = huff_dict;
    this->tree = new Node('0', 0);
    Node * temp;
    for(auto d = huff_dict.begin(); d != huff_dict.end(); ++d){
        char key = d->first;
        string code = d->second;
        temp = this->tree;
        for(auto c = code.begin(); c!=code.end(); ++c){
            if(*c == '0'){
                if(temp->l_child == nullptr)
                    temp->l_child = new Node(key, 0);
                temp = temp->l_child;
            }else if(*c == '1'){
                if(temp->r_child == nullptr)
                    temp->r_child = new Node(key, 0);
                temp = temp->r_child;
            }
        }
    }
}

void Huffman::creatHuff(vector<Node *> &nodes) {
    vector<Node *> temp_nodes = nodes;
    int n = nodes.size();
    Node *temp_node1;
    Node *temp_node2;
    for (int i = 0; i < n - 1; ++i) {
        // 弹出节点
        temp_node1 = nodes.back();
        nodes.pop_back();
        temp_node2 = nodes.back();
        nodes.pop_back();
        // 生成新的节点
        NodePtr temp_node = new Node(temp_node1, temp_node2);
        //根据值插入
        insert_node(nodes, temp_node);
    }
    tree = nodes.front();
    createHuffDict(temp_nodes);
}

void Huffman::insert_node(vector<Node *> &nodes, NodePtr temp_node) {
    for (int i = 0; i < nodes.size(); ++i)
        if (nodes[i]->weight <= temp_node->weight) {
            nodes.insert(nodes.begin() + i, temp_node);
            return;
        }
    nodes.push_back(temp_node);
}

void Huffman::createHuffDict(vector<Node *> &nodes) {
    // stack
    NodePtr s;
    NodePtr parent;
    stack<char> st;

    // 遍历每个节点
    for (auto node = nodes.begin(); node != nodes.end(); ++node) {
        s = *node;
        string temp;
        //如果有父亲，自己 和 自己的父亲
        parent = s->parent;
        while (parent) {
            if (parent->l_child == s)
                st.push('1');
            else
                st.push('0');
            s = parent;
            parent = s->parent;
        }

        while (!st.empty()) {
            temp.push_back(st.top());
            st.pop();
        }
        huff_dict[(*node)->data] = temp;
    }
}

void Huffman::encoding(std::string in_file, std::string out_file) {
    // 从文件读入待编码字符串
    string file_str = read_from_file(in_file);

    // 根据字符串的频率生成编码字典 huff_dict
    vector<Node *> nodes;
    parse_file(nodes, file_str);
    Huffman huffman_ = Huffman(nodes);

    // 根据 huffm_dict 编码文本
    string huff_code;
    huff_code.reserve(sizeof(char) * file_str.size());
    for(auto c = file_str.begin(); c!= file_str.end(); ++c){
        huff_code += huffman_.huff_dict[*c];
    }
    string (huff_code).swap(huff_code);

    // 打开输出文件
    ofstream ofs;
    ofs.open(out_file, ios::out|ios::binary);

    // 将 huff_dict 编码成 01 bit stream 存入输出文件用于解码
    int len;
    len = huffman_.huff_dict.size();
    char keys[len];
    bitset<24> values[len];

    // 01 字符串 转为 01 比特溜
    string cmpl_str(8, '0');
    string temp_str = "111111111111111111111110";
    int i = 0;
    for(auto c = huffman_.huff_dict.begin(); c != huffman_.huff_dict.end(); ++c, ++i){
        keys[i] = c->first;
        values[i] = bitset<24> (temp_str.substr(c->second.size(), 24-c->second.size()) + c->second);
        if(c->second.size() >= 8)
            cmpl_str = c->second;
    }
    // 将字典写入文件
    len = sizeof(keys);
    ofs.write((const char *)&len, sizeof(long));
    ofs.write((const char *)&keys, len);

    len = sizeof(values);
    ofs.write((const char *)&len, sizeof(long));
    ofs.write((const char *)&values, len);

    // 将编码好的01串转化为 8位01 组成的数组
    int n = huff_code.size();
    bitset<8> to_save[n/8 + 1];
    string temp;
    temp.reserve(8);
    int cmpl;
    for(int i = 0, count = 0; i <n; i = i + 8, ++count){
        cmpl = i + 8 - n;
        if(cmpl > 0)
            // 结尾不足8位，用无效的前缀补充
            temp = huff_code.substr(i, 8) + cmpl_str.substr(0, cmpl);
        else
            temp = huff_code.substr(i, 8);
        to_save[count] = bitset<8>(temp);
    }

    // 将编码的01串写入输出文件
    len = sizeof(to_save);
    ofs.write((const char *)&len, sizeof(long));
    ofs.write((const char *)&to_save, len);
    ofs.close();
}

void Huffman::decoding(std::string in_file, std::string out_file) {
    // 读文件
    ifstream ifs;
    ifs.open(in_file, ios::in| ios::binary);
    if(!ifs.is_open()){
        cout << "read file open error" << endl;
        fout << "read file open error" << endl;
        return ;
    }
    long len;
    int temp_size;
    int dict_len;

    // 根据写入格式读取编码文件
    ifs.read((char *)&len, sizeof (long));
    temp_size = len / sizeof(char);
    dict_len = temp_size;
    char keys[temp_size + 1];
    ifs.read((char *)&keys, len);

    ifs.read((char *)&len, sizeof (long));
    temp_size = len / sizeof(bitset<24>);
    bitset<24> values[temp_size + 1];
    ifs.read((char *)&values, len);

    ifs.read((char *)&len, sizeof (long));
    temp_size = len / sizeof(bitset<8>);
    bitset<8> temp_code[temp_size + 1];
    ifs.read((char *)&temp_code, len);

    // 重建 huffman 编码字典
    map<char, string> huff_dict;
    for(int i = 0; i < dict_len; ++i){
        int posi = values[i].to_string().find('0') + 1;
        huff_dict[keys[i]] = values[i].to_string().substr(posi, 24 - posi);
    }

    // 读入代解码的01串
    string huff_code;
    huff_code.reserve(temp_size * 8 + 8);
    for(int i = 0; i < temp_size; ++i){
        huff_code += temp_code[i].to_string();
    }

    // 根据字典构建 huffman 编码树
    Huffman huffman_ = Huffman(huff_dict);

    // 根据编码树，将 01 编码转化为原始文本
    string result;
    result.reserve(temp_size);
    NodePtr temp;
    temp = huffman_.tree;
    for(auto c=huff_code.begin(); c!=huff_code.end(); ++c){
        if(*c == '0')
            temp = temp->l_child;
        else
            temp = temp->r_child;
        if(temp->l_child == nullptr){
            result += temp->data;
            temp = huffman_.tree;
        }
    }
    ofstream ofs;
    ofs.open(out_file, ios::out);
    ofs << result << endl;
}


int main(int argc, char **argv) {
    // 设置日志文件
    string log_file = "E:\\codes\\agorthm\\HuffmanCode\\log.txt";
    fout.open(log_file, ios::out);

    //遍历每个输入文件
    string mode;
    string in_file;
    string out_file;
    if(argc <= 1){
        mode = "encoding";
        cout << "Use default mode: "<< mode << endl;
        fout << "Use default mode: "<< mode << endl;
    }else{
        mode = argv[1];
        cout << "Use mode:" << mode << endl;
        fout << "Use mode:" << mode << endl;
    }
    if(argc <= 2){
        in_file = "E:\\codes\\agorthm\\HuffmanCode\\input.txt";
        cout << "Use default input file: "<< in_file << endl;
        fout << "Use default input file: "<< in_file << endl;
    }else{
        in_file = argv[2];
        cout << "Use input file:" << in_file << endl;
        fout << "Use input file:" << in_file << endl;
    }

    if(argc <= 3){
        out_file = "E:\\codes\\agorthm\\HuffmanCode\\output.txt";
        cout << "Use default output file: " << out_file << endl;
        fout << "Use default output file: " << out_file << endl;

    }else{
        out_file = argv[3];
        cout << "Use output file:" << out_file << endl;
        fout << "Use output file:" << out_file << endl;

    }

    if(mode == "encoding"){
        Huffman::encoding(in_file, out_file);
        cout << "Encoding finished" << endl;
        fout << "Encoding finished" << endl;
    }else if(mode == "decoding"){
        Huffman::decoding(in_file, out_file);
        cout << "Decoding finished" << endl;
        fout << "Decoding finished" << endl;
    }else{
        cout << "Unknown mode" << endl;        fout << "Unknown mode" << endl;
        return -1;
    }
    fout.close();
    return 0;

}
