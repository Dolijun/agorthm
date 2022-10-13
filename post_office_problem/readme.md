## 带权重的邮局选址问题

学号：S202274024
姓名：董立珺
联系电话：13938681254

***注：需要打开内联公式功能查看公式***

### 运行环境 

win10, MinGW w64 9.0

### 文件说明
code_assign01.cpp		# C++代码文件
input_assign01_0\*.txt 		# 输入文件
output_assign01.txt		# 输出文件
shell_assign01.sh	# shell 脚本

### 运行过程说明
#### 运行文件
1. 使用可执行文件
运行 exec_assign01.exe 文件，参数为需要输入文件，如：
``` shell
./exec_assign01 input_assign01.txt
```
2. 命令行或双击运行 shell_assign01.sh 文件，shell会自动输入所有输入文件 input_assign01_\*.txt
```shell
bash ./shell_assign01.sh
```
#### 输出结果及其结构
1. 标准输出
2. output_assign01.txt
输出依次显示：输入文件、输出文件、读入的居民点数、居民点坐标及权重、最佳邮局地址、居民到邮局的带权距离
```
*************************
Input filename: input_assign01_04.txt
Output filename: output_assign01.txt
There are 3 residents in total.
coordinate	weight
( 5, 1 )	2
( 2, 5 )	4
( 3, 3 )	1
The best post office positon:( 2, 5 )
The total weighted distance:17
*************************
```



### 问题描述

在一个按照东西和南北方向划分成规整街区的城市里，n个居民点散乱地分布在不同的街区中。用x坐标表示东西向，用y坐标表示南北向。各居民点的位置可以由坐标(x,y)表示。要求：为建邮局选址，使得n个居民点到邮局之距离的总和最小。
（每个居民点可以有若干人居住）

### 问题分析
#### 1. 最优解为N个权值得带权中位数
我们可以将邮局选址问题定义为如下数学问题：
假设共有n个居民点，第i个居民的坐标为$(x_i, y_i)$,权重为$w_i$,i为整数, $w_i$为整数,1≤i≤n，求邮局地址坐标$(x_p, y_p)$，使居民到邮局的距离乘以权重的和最小。

**因为邮局选址在x轴和y轴式相互独立的，所以我们首先分析一维带权邮局选址问题。**
对于一维邮局选址问题，假设x轴上有n个点，权重为$w_i$，i为整数。记第i个点到第j个点的距离为$dist(i, j)$，邮局位置为p，则所有居民到邮局p的距离和为：$\sum w_i*dist(i,p)$

记L为邮局p左边居民，左侧居民到邮局的距离和为：$\sum w_L*dist(L,p)$
记R为邮局p右边的居民，右侧居民到邮局的距离和为：$\sum w_R*dist(R,p)$

**证明：**

假设邮局一定在居民点，T为最优解，T+1为T右侧第一个居民点，R为T右侧的所有居民点，L为T左侧的所有居民点。
$\because T为最优点$
$\therefore \sum w_i*dist(i,T) \le \sum w_i*dist(i,T+1) $

$\therefore \sum w_L*dist(L,T) + \sum w_R*dist(R,T) \le \sum w_L*dist(L,T+1) + \sum w_R*dist(R,T+1) + w_T*dist(T,T+1) - w_{T+1}*dist(T+1,T+1)$

$\because dist(T+1,T+1)=0 $

移项，得：

$\sum w_R*(dist(R,T)-dist(R,T+1)) \le \sum w_L*(dist(L,T+1)-dist(L,T)) + w_T*dist(T,T+1) $

$\because \left\{ \begin {array}{**C**} dist(R,T)-dist(R,T+1)=dist(T,T+1)\\ dist(L,T+1)-dist(L,T)=dist(T,T+1) \end{array} \right. $

$\therefore \sum w_R*dist(T,T+1) \le \sum w_L*dist(T,T+1) + w_T*dist(T,T+1) $

$\therefore \sum w_R \le \sum w_L + w_T $

同理可得：

$\sum w_L \le \sum w_R + w_T $

故 T 为N个权值得带权中位数

**即邮局最优地址得x，y坐标分别为所有居民点得x坐标带权中位数和y坐标带权中位数**

#### 2. 带权的邮局选址问题和不带权邮局选址问题相同
不带权的邮局选址问题中每个居民点只有一个居民居住，带权的邮局选址问题中每个居民点可以有多个居民居住。**这两个问题属于同一个问题**，可以认为在前者的居民点可以重合的条件下，问题与后者相同。所以用不带权的邮局选址问题的方法可以解决带权邮局选址问题。

#### 3. 当邮局可以选择任意点时，居民点上一定有最优解
我们将带权邮局选址问题看作不带权的邮局选址问题，权重w表示w个居民点位置重叠。
假设有奇数个居民，通过*1*证明可知，居民点一定在奇数个居民点的坐标中位数上，此时最优解一定在居民点上。
假设有偶数个居民点，若中间的两个中间居民点重合，则最优解一定在居民点上；若中间的两个居民点不重合，则这两个居民点之间的所有点都是最优解（包括两端的居民点）。（证明思路对于一维邮局选址问题，**最优解在中间两居民点之间移动时，左右两侧的距离变化相同**）

综上所述，居民点上一定有邮局地址的最优解。

### 类设计
#### 1. 居民点Point类
居民点用一个类 Point 表示, 属性包括坐标 (x, y) 以及权重 w 

| 函数原型说明                      | 功能                               |输入 | 返回值 | 说明 |
| --------------------------------- | ----------------------------------- | ------ | ---- |---|
| Point::Point()                    | 默认构造                            |        |      | |
| Point::Point(int x, int y, int w) | 根据输入的x, y, w初始化对象         | x, y, w分别为点的横纵坐标和权重 |      | |
| Point::Point(Point &p)            | 拷贝构造，拷贝Point p的属性进行构造 | 待拷贝对象的引用 |      | |

```c++
class Point {
public:
    int x, y;   /* Coordinate of point */
    int w;      /* Weight of point */
    
    Point();    /* Default initialization */
    
    Point(int x, int y, int w);     /* Initialize with coordinates and weights */
    
    Point(Point &p);        /* Copy initialization */
};
```

#### 2. Village类

社区用.类 Village 表示，属性包括指向 Point 对象的指针数组存储所有居民点

| 函数原型说明                                                 | 功能                                                | 输入                                                         | 返回值         | 说明                     |
| ------------------------------------------------------------ | --------------------------------------------------- | ------------------------------------------------------------ | -------------- | ------------------------ |
| Village::Village()                                           | 默认构造                                            |                                                              |                |                          |
| Village::Village(string filename)                            | 从文件中读入数据进行初始化                          |                                                              |                | filename为输入文件的路径 |
| Village::~Village()                                          | 析构函数                                            |                                                              |                | 释放对象堆区内存         |
| static bool cmp_x(ptr_Point &x, ptr_Point &y)                | 比较输入点的x坐标值                                 | x, y表示待对比的两个Point对象引用                            | 比较结果(bool) | 根据该函数对坐标点排序   |
| static bool cmp_y(ptr_Point &x, ptr_Point &y)                | 比较输入点的y坐标值                                 | x, y表示待对比的两个Point对象引用                            | 比较结果(bool) | 根据该函数对坐标点排序   |
| void QuickSort(ptr_Point *array, int low, int high, bool(\*func)(ptr_Point p1, ptr_Point p2)) | 根据func (比较函数)，对坐标点数组array进行排序      | array为待排序数组，low，high为待排序数组的前后位置，func为比较函数 | void           |                          |
| void get_post()                                              | 分别对x,y坐标值排序，得到邮局坐标的最优解并打印输出 |                                                              | void           |                          |

```c++
/* Village, Store information for all residents */
class Village {
private:
    ptr_Point people;   /* Point to all residents */
    int N;              /* Number of residents */

public:
    Village();      /* Default initialization */

    Village(string filename);       /* Initialization from file */
	
    ~Village();     /* Destructor of Village */

    static bool cmp_x(ptr_Point &x, ptr_Point &y);        /* Compare x-axis coordinate values */

    static bool cmp_y(ptr_Point &x, ptr_Point &y);        /* Compare y-axis coordinate values */

    void QuickSort(ptr_Point *array, int low, int high, bool(*func)(ptr_Point p1, ptr_Point p2)); /* Sort using quick sort */

    void get_post();        /* Get Post Office Coordinates according to residential coordinates */
};
```

### 算法伪码

```C++
people = get_input(filename);  // 从文件中得到所有居民点坐标数组
sort_x(people);		//根据x坐标值进行排序
post_x = get_weighted_mid_x(people);  //获取x坐标的带权中位数作为邮局x坐标
sort_y(people);		//根据y坐标值进行排序
post_y = get_weighted_mid_y(people);  //获取y坐标的带权中位数作为邮局y坐标
totle_weighted_distance = get_totle_weighted_distance(people, post_x, post_y);	//根据居民点坐标和邮局坐标得到总共的带权距离
output_to_file(post_x, post_y);		// 输出邮局坐标
```
#### 排序算法伪码
程序采用快速排序算法，伪码如下：
```C++
int quick_sort_parition(array, low, high){
	// low 位置的元素作为排序的哨兵
	p_mid = array[low];
	while(high > low){
		while(array[high] >= p_mid and low < high)
			--high;
		array[low] = array[high];
		while(array[low] <= p_mid and low < high)
			++low;
		array[low] = array[high];
	}
	array[high] = p_mid;
	return high;
}
void quick_sort(array, low, high){
	// 递归调用 quick_sort 函数， 二分法进行算法排序
	if(low < high){
		mid = quick_sort_parition(array, low, high);
		quick_sort(array, low, mid-1);
		quick_sort(array, mid+1, high);
	}
}
```
#### 中位数查找伪码
```C++
position get_weighted_mid_y(array){
	int weight = 0
	int temp_weight = 0
	// 获取总权重
	for(int i = 0; i < array.lenght; ++i)
		weight += array[i].weight;
	// 找到第i个元素为带权中位数，返回第i个元素的位置
	for(int i = 0; i < array.lenght; ++i){
		temp_weight += array[i].weight;
		if(2 \* temp_weight >= weight)
			return array[i].position;
	}
	return NULL;
}
```

### 算法时间复杂度说明
1. 快速排序的时间复杂度为$O(nlogn)$
2. 带权中位数查找时间复杂度为$O(n)$
