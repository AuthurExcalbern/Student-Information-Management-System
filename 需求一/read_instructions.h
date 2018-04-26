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

#ifndef _read_insturctions_H

//追加层数x
std::istream& add_floor(std::istream& in, int x, struct InitData init_data, vector<struct floor>& floors);

//追加某层的房间数x
std::istream& add_room(std::istream& in, int x, struct InitData init_data, vector<struct floor>& floors);

//追加某房间的容量x
std::istream& add_bed(std::istream& in, int x, struct InitData init_data, vector<struct floor>& floors);

//从字符串得到数字
double get_double_num(string s, int& i);

int get_stu(string stu_name, vector<struct student>& students);

//学生转移房间
void stu_move_from_to(string s, int i, string stu_name, vector<struct student>& students, vector<struct floor>& floors);

//学生退学
void stu_quit(string stu_name, vector<struct student>& students, vector<struct floor>& floors);

//学生入学
void stu_enrol(string stu_name, vector<struct student>& students, vector<struct floor>& floors);

//学生进餐,打工，学习
double calculate(string fomula, double eat_time, double work_time, double study_time, double weight_inc, double knowledge_inc, struct InitData init_data);

//读取有关学生的命令
std::istream& student_data_change(std::istream& in, string stu_name, vector<struct student>& students, vector<struct floor>& floors, struct InitData init_data);

//读取并执行命令
std::istream& read_and_exec_instructions(std::istream& in, struct InitData init_data, vector<struct student>& students, vector<struct floor>& floors);

#endif