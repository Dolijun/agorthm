## Huffman encoding and Decoding

***注：需要打开内联公式功能查看公式***

### 运行环境 

win10, MinGW w64 9.0

### 文件说明
code_assign03.cpp		# C++代码文件
input_assign03_0\*.txt 		# 输入文件
output_assign03.txt		# 输出文件
shell_assign03.sh	# shell 脚本

### 运行过程说明
#### 运行文件
程序有两种模式：
* encoding可以对输入文件的文本进行编码
* decoding可以对编码文件进行解码

程序运行需要三个参数
* 参数1为运行模式：encoding 或 decoding
* 参数2为输入文件：input_file_path
* 参数3为输出文件：output_file_path

***注：encoding模式输入文件为文本文件，输出为编码文件；decoding模式输入文件为编码文件，输出文件为文本文件***

1. 使用可执行文件(以编码模式为例)
运行 exec_assign03.exe 文件，参数分别为模式、输入文件、输出文件，如：
``` shell
./exec_assign03 encoding input_assign03_0.txt encoding_assign03_0.txt
```
2. 命令行或双击运行 shell_assign03.sh 文件

shell会自动输入文件 input_assign03_0.txt，输出 encoding_assign03_0.txt，然后对 encoding_assign03_0.txt 进行解码输出解码文件 decoding_assign03_0.txt 
```shell
bash ./shell_assign03.sh
```

#### 输出结果及其结构
1. 标准输出
2. log_assign03.txt
输出依次显示：编码模式，输入文件，输出文件
```
./exec_assign03 encoding input.txt encoding.txt

Use mode:encoding
Use input file:input.txt
Use output file:encoding.txt
Encoding finished
```
编码的结果保存在encoding.txt中

### 问题描述

(简答题) 利用huffman编码对文本文件进行压缩与解压。
输入：一个文本文件

输出：压缩后的文件

算法过程：

（1）统计文本文件中每个字符的使用频度

（2）构造huffman编码

（3）以二进制流形式压缩文件

程序对要求进行了扩展，实现了对文本文件的压缩和解压两个过程。

### 数据结构设计
#### 1. 树节点
Huffman树的节点类的结构
```c++
class Node {
public:
    char data; // 节点字符
    long weight; // 节点权重
    Node *parent; //节点的父节点
    Node *l_child; // 节点的左儿子
    Node *r_child; // 节点的右儿子

    Node(); 

    Node(char data, long weight); // 通过数据和权重构造节点

    Node(Node &temp); // 拷贝构造

    Node(Node *l_child, Node *r_child); // 构造新节点，左右儿子指向给定的节点

    static bool cmp(Node *a, Node *b); // 比较不同节点的大小，用于节点排序
};
```

#### 2. Huffman类
Huffman类，包括Huffman树和Huffman编码字典。可以通过各个节点的频率或Huffman编码字典进行构造
```c++
class Huffman {
public:
    NodePtr tree; // huffman树
    map<char, string> huff_dict; // huffman编码字典

    // 根据字符的权重构建 Huffman 树
    Huffman(vector<Node *> &nodes);

    // 根据 huffman 字典构建 Huffman 树
    Huffman(map<char, string> &dict);

    void creatHuff(vector<Node *> &nodes); //构造哈夫曼树
    void createHuffDict(vector<Node *> &nodes); //哈夫曼树生成哈夫曼编码字典
    void insert_node(vector<Node *> &nodes, NodePtr temp_node); // 在构造Huffman树的过程中，将两个节点融合产生新的节点，将新节点重新插入原本有序的节点序列中。

    static void encoding(string in_file, string out_file); // 根据输入文件，构造Huffman类，然后对输入文件进行编码，保存到输出文件中。
    static void decoding(string in_file, string out_file); // 根据输入的编码文件，读入Huffman编码字典，构造Huffman类，对编码进行解码，解码的文本保存在输出文件中。

};
```
### 算法设计
#### 1. 统计文本文件中每个字符的使用频率
1. 遍历输入文件的文本，使用字典保存文本中每个字符出现的频率。
2. 根据字符出现的频率对字符进行排序
```C++
    set<char> keys;
    map<char, int> dict;
    // 遍历输入文本，统计每个字符
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
    // 根据字符频率，对字符进行排序
    sort(nodes.begin(), nodes.end(), Node::cmp);
```
#### 2. 构造Huffman编码
1. 构造Huffman树。将频率最小的两个节点合并为新的节点，新节点的权重，等于两个子节点的频率和。根据新节点的权重重新插入到有序的节点序列中。
```C++
    // nodes 为根据权重（频率）排序的节点数组
    vector<Node *> temp_nodes = nodes;
    int n = nodes.size();
    Node *temp_node1;
    Node *temp_node2;
    // 依次选取权重最小的节点，融合为新的节点，并重新插入到有序节点序列中。
    for (int i = 0; i < n - 1; ++i) {
        // 弹出权重最小的两个节点
        temp_node1 = nodes.back();
        nodes.pop_back();
        temp_node2 = nodes.back();
        nodes.pop_back();
        // 合并生成新的节点
        NodePtr temp_node = new Node(temp_node1, temp_node2);
        // 根据权重重新插入有序节点序列
        insert_node(nodes, temp_node);
    }
    tree = nodes.front();
```
2. 根据构造的Huffman树，生成Huffman编码字典。编码的过程根据huffman字典对输入文本中的字符进行替换。
```C++
 NodePtr s;
    NodePtr parent;
    stack<char> st;

    // 遍历每个叶节点，自底向上生成每个字符的编码
    for (auto node = nodes.begin(); node != nodes.end(); ++node) {
        s = *node;
        string temp;
        parent = s->parent;
        while (parent) {
            if (parent->l_child == s)
                st.push('1');
            else
                st.push('0');
            s = parent;
            parent = s->parent;
        }
        // 对编码序列逆序
        while (!st.empty()) {
            temp.push_back(st.top());
            st.pop();
        }
        // 在字典中添加字符对应的编码
        huff_dict[(*node)->data] = temp;
    }
```

#### 3. Encoding and Decoding
编码文件的过程会将huffman编码字典和编码的比特流保存保存到编码文件
为了节省编码文件的空间，在写入文件前，程序会将需要写入文件的01串压缩为 01 bit流
```C++
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
```

### 算法伪码
#### 编码过程
```C++
string file_str = read_from_file(in_file); // 从输入文件读入字符串
map<char, int> freq_dict = parse_file(file_str); // 统计字符串中，每个字符出现的频率
Huffman huffman = Huffman(freq_dict); // 根据字符频率创建哈夫曼树和哈夫曼编码字典
ofs.write_to_file(huffman.huff_dict); // 将huffman 编码字典写入编码文件
string huff_code = huffman.decode(file_str); // 编码输入的字符串
ofs.write_to_file(huff_code); // 将编码的01串写入文件
```
#### 解码过程
```C++
huff_dict = load_from_file(in_file); // 从输入文件中读入Huffman编码字典
huff_code = load_from_file(in_file); //从输入文件中读入编码01串

Huffman huffman = Huffman(huff_dict); // 根据Huffman编码字典构建Huffman树

decode_str = huffman.decode(huff_code); // 利用huffman树解码编码的01串

ofs.write_to_file(decoder_str); // 将解码的字符串写入文件
```





## 算法分析
实验使用了字典，堆栈，树等数据结构，算法有字典的生成，快速排序，插入排序