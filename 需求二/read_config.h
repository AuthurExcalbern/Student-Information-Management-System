#ifndef _read_config_h
#define _read_config_h

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


//判断字符串 Template 是否等于 Sample 的开头
bool is(string Template, string Sample);

//从字符串得到数字
double get_double_num(string s, int& i);

//把初始化数据读入结构体
void read_initData(string s, int& init_data);
void read_initData(string s, double& init_data);

//将中缀公式转换成后缀公式并返回
string infix_to_postifx(string fomula_infx);

//把初始化数据里的公式转换成后缀公式然后读入结构体
void read_formula(string s, string& init_data);

//读取文件 config 的初始化数据
std::istream& read_config(std::istream& in, struct InitData& init_data);

#endif