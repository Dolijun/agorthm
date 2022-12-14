# 广义背包问题

***注：需要打开内联公式功能查看公式***

## 运行环境

win10, MinGW w64 9.0

## 运行过程说明

### 运行文件

### 输出结果及其结构
* *在输出文件中会显示程序的输入输出文件
* 读入的各个类别物品的重量和价值以及背包的容量
* 最优情况各个物品的数量，以及最优结果的物品总价值和总重量
```
*************************
Input filename: 
Output filename:
*************************
Object * weitht: * value: *
...
Bag capacity: 
*************************
Object * num: 
Totle value: 
Totle weight: 
*************************
```

## 问题描述

利用动态规划算法实现广义背包问题
广义背包问题的描述如下：给定载重量为M的背包和n种物品，每种物品有一定的重量和价值，现在需要设计算法，在不超过背包载重量的前提下，巧妙选择物品，使得装入背包的物品的总价值最大化。规则是，每种物品均可装入背包多次或不装入（但不能仅装入物品的一部分）。

需要给出最优值和最优解，即装入物品的最大总价值，以及具体每种物品装入几个。

## 问题分析

这个问题非常类似于01背包问题，所不同的是每种物品有无限件。
也就是从每种物品的角度考虑，与它相关的策略已并非取或不取两种而是有取0件、取1件、取2件……等很多种。如果仍然按照解01背包时的思路，令$f[i][v]
$表示前i种物品恰放入一个容量为v的背包的最大价值。

### 完全背包问题的递推公式：

可以按照每种物品不同的策略写出状态转移方程：
$$
dp[i][j]=max(dp[i-1][j], dp[i][j-w_i] + v_i)
$$
当$k=0$时，表示当前物品没有被选择，即dp[i-1][j];

当$k\ne0$时，对当前物品进行了选择，至少选择了i种物品的一个物品。

$dp[i][j-w_i]$表示在0~i种物品中选择，可用背包空间为$j-w_i$时，背包所能取得的最大价值。而$dp[i][j-w_i]
$本身又需要通过$dp[i][j-k*w_i], 1\le k \le [j/w_i]$求得。求解$dp[i][j-w_i]$的过程包含了当前第i种物品除了不选择之外的所有选择组合。

$max(dp[i-1][j], dp[i][j-k*w_i] + k*v_i), 1 \le k \le [j/w_i]$表达的含义是在还有0~(i-1) 种物品中可选择，剩余背包空间为$j

- k * w_i$时，所能取得的最大价值，$1 \le k \le [j/w_i]$ 也是当前物品i必须选择下的所有选择组合。

所以$max(dp[i][j-w_i] + v_i)$ 和 $max(dp[i][j-k*w_i] + k * v_i)$所表达的含义相同。

### 获取完全背包选择物品的种类和数量

动态规划的过程可以获得状态转移矩阵dp
$dp[i][j]$依赖于$dp[i-1][j]$和$dp[i][j-w_i]$，即状态转移矩阵的第i行和第i-1行。

动态规划是自上而下的，反向求物品的种类和数量需要自下而上，通过判断$dp[i-1][j]$和$dp[i][j-w_i] +
v_i$的大小，判断当剩余空间为j的状态时，是否选择物品i。

## 数据结构设计

### 物品类

用于存储每个物品的价值和重量

```C++
class Object {
public:
    int weight; // 物品的重量
    int value; // 物品的价值

    Object(); // 物品的默认构造

    Object(int weight, int value); // 物品类根据重量和价值构造
};
```

### 背包类

存储物品的容量，待选物品种类和每种物品信息，能够从文件读入背包和物品信息，get_result方法会根据背包和物品信息得到背包容纳物品的最优结果。

```c++
class FullBag {
public:
    int Capacity; // 背包的容量
    int N;  // 背包中待选物品的种类
    vector<Object> objects; // 待选物品的列表，记录每种物品的重量和价值

    FullBag();  // 背包的默认构造

    FullBag(string file_path); // 从文件读入背包容量，物品数量和各个物品的重量和价值。

    void get_result(); // 根据背包的信息，进行动态规划，得到背包最优的容纳物品重量和质量和每种物品选择的数量
};
```

## 算法设计

本次实验采用的思路是动态规划

首先会构建一个**状态转移矩阵**，矩阵有N行C列，N表示物品的种类数量，C表示背包的容量。记状态转移矩阵为dp，$dp[i][j]
$表示可选前i种物品，背包容量为j时，背包容纳物品的最优价值。

```c++
 // 构建状态转移矩阵
vector<vector<int>> dp;
dp.reserve(N + 1);
for (int i = 0; i <= N; ++i)
dp.push_back(vector<int>(Capacity + 1, 0));

num.reserve(N + 1);
for (int i = 0; i <= N; ++i)
num.push_back(0);
```

动态规划过程就是依次判断前i种物品在背包容量为c时，背包最多容纳的物品价值，0<i<N, 0<c<C。
在可选物品为前i种，背包容量为c时，只需要判断$dp[i-1][j]$和$dp[i][j-w_i]+v_i$，判断相对当容量为j时，相对于背包容量为$j-w_i$时，是否要选择第i件物品。

```c++
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
```

自底向上，反推各类别物品的数量。
通过判断$dp[i][j-w] + b$ 和 $dp[i-1][j]$的大小，判断当容量为j时，是否选择了物品i，进而统计每种类别物品数量。

```c++
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
```

## 算法伪码

```c++
// 从输入文件中，读入背包和物品的信息
Capacity, N, object[N] = get_info_from_file(file_path);
// 构建动态规划的状态转移矩阵
int dp[N][Capacity];
// 自顶向下，动态规划
for (int i = 1; i <= N; ++i)
    for (int j = 1; j <= Capacity; ++j) {
        if (j < w)
            dp[i][j] = dp[i - 1][j];
        else
            dp[i][j] = max(dp[i - 1][j], dp[i][j - w] + v);
}
// 自底向上，反推物品数量
for (int i = N; i > 0; --i) {
while (j - w >= 0)
if ((dp[i][j - w] + v) > dp[i - 1][j]) {
j -= w;
num[i] += 1;
} else
break;
}
// 输出物品数量
```
## 算法分析
> 空间复杂度：构建dp矩阵，开销是$O(N*C)$，N表示物品数量,C为背包容量
> 时间复杂度：动态规划过程，最坏情况对dp矩阵每个值都更新，时间开销是$O(N * C)$; 反推物品数量的过程只需要对背包容量遍历，时间复杂度是$O(C)$
> 因此时间复杂度是$O(N*C)$



