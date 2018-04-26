#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<stack>
#include<cmath>
#include<algorithm>
#include<queue>
using std::cin;
using std::cout;
using std::string;
using std::vector;
using std::stack;
using std::pair;
using std::queue;

//学生信息结构体
struct student
{
    string name;
    
    string sex;
    
    //1~100
    double charm;
    double knowledge;
    
    //应该 >= 0
    double money;
    double weight;
    
    //1~99
    unsigned int floor, room;
    
    //是否已经退学
    bool quit;
    
    bool is_loving;
    queue<struct student> love_queue;
    
    bool operator<(const struct student & rhs)const
    {
        return name < rhs.name;
    }
};

//记录宿舍的楼层数 + 每层的房间数 + 每房的容量 + 已经住了多少人
struct floor
{
    vector< pair<int, int> > room;
};

//记录从 config 读入的初始数据
struct InitData
{
    int FLOOR, ROOM, CAPACITY;
    int WEIGHT, MONEY, KNOWLEDGE, CHARM;
    int FOOD_WEIGHT, FOOD_COST;
    int STUDY_WEIGHT, STUDY_KNOWLEDGE;
    int WORK_MONEY, WORK_WEIGHT;
    
    double LOVE_THRESHOLD, LOVER_CHANGE_THRESHOLD;
    
    //储存从中缀转换的：后缀表达式
    string WEIGHT_INC;
    string MONEY_INC;
    string KNOWLEDGE_INC;
    string CHARM_INC;
};