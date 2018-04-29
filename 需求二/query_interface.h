#ifndef _QUERY_INTERFACE_H
#define _QUERY_INTERFACE_H

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
using std::queue;


void query_interface_start(std::istream& in, std::ostream& out, vector<struct student>& students, vector<struct floor>& floors);

/****************************查询界面的通用函数*****************************/
//用于从字符串得到数字
double get_double(string s, string::size_type i);

//输出帮助
void print_help();

//输出错误
void print_err(const string& err, std::ostream& out);

//输出结果的学生信息
void print_message(vector<struct student>& result_stu);
/****************************查询界面的通用函数*****************************/


/******************************weight********************************/
//安装weight排序
bool com_weight(const struct student& a, const struct student& b);

//中转函数
void get_weight(vector<struct student> students, const string& sort_judge, const string& sex_judge, const bool& is_rang, const double& rang_x, const double& rang_y);

//最大最下值
void get_the_weight(const vector<struct student>& temp_stu, const string& sex_judge, vector<struct student>& result_stu);

//范围查询
void get_weight_rang(const vector<struct student>& temp_stu, const string& sex_judge, vector<struct student>& result_stu, const double& rang_x, const double& rang_y);
/******************************weight********************************/


/******************************money*********************************/
bool com_money(const struct student& a, const struct student& b);

void get_money(vector<struct student> students, const string& sort_judge, const string& sex_judge, const bool& is_rang, const double& rang_x, const double& rang_y);

void get_the_money(const vector<struct student>& temp_stu, const string& sex_judge, vector<struct student>& result_stu);

void get_money_rang(const vector<struct student>& temp_stu, const string& sex_judge, vector<struct student>& result_stu, const double& rang_x, const double& rang_y);
/******************************money*********************************/


/******************************knowledge******************************/
bool com_knowledge(const struct student& a, const struct student& b);

void get_knowledge(vector<struct student> students, const string& sort_judge, const string& sex_judge, const bool& is_rang, const double& rang_x, const double& rang_y);

void get_the_knowledge(const vector<struct student>& temp_stu, const string& sex_judge, vector<struct student>& result_stu);

void get_knowledge_rang(const vector<struct student>& temp_stu, const string& sex_judge, vector<struct student>& result_stu, const double& rang_x, const double& rang_y);
/******************************knowledge******************************/


/******************************charm**********************************/
bool com_charm(const struct student& a, const struct student& b);

void get_charm(vector<struct student> students, const string& sort_judge, const string& sex_judge, const bool& is_rang, const double& rang_x, const double& rang_y);

void get_the_charm(const vector<struct student>& temp_stu, const string& sex_judge, vector<struct student>& result_stu);

void get_charm_rang(const vector<struct student>& temp_stu, const string& sex_judge, vector<struct student>& result_stu, const double& rang_x, const double& rang_y);
/******************************charm**********************************/

/******************************查询学生的房间****************************/
void get_stu_room(int& floor, int& room, const vector<struct student>& students);
/******************************查询学生的房间****************************/


/****************************查询恋爱历史******************************/
//查询恋爱过的人里恋人最多的人
void get_lover_maxlovers(vector<struct student> students);

//查询恋爱过的人里魅力值最大的人
void get_lover_maxcharm(vector<struct student> students);

//按名字排序
bool com_name(const struct student& a, const struct student& b);

//查询恋爱历史
void get_lover(vector<struct student> students, const string& com_str, const int& stu_id);
/****************************查询恋爱历史******************************/

#endif