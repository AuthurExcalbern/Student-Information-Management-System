#ifndef _read_dispatch_h
#define _read_dispatch_h

#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<stack>
#include<cmath>
#include<algorithm>
using std::cin;
using std::cout;
using std::string;
using std::vector;
using std::stack;
using std::pair;


//读入并初始化学生信息
std::istream& read_dispatch(std::istream& in, vector<struct student>& students, vector<struct floor>& floors, struct InitData init_data);

#endif