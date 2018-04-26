#ifndef _read_insturctions_H
#define _read_insturctions_H

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


//追加层数x
std::istream& add_floor(std::istream& in, int x, struct InitData init_data, vector<struct floor>& floors);

//追加某层的房间数x
std::istream& add_room(std::istream& in, int x, struct InitData init_data, vector<struct floor>& floors);

//追加某房间的容量x
std::istream& add_bed(std::istream& in, int x, struct InitData init_data, vector<struct floor>& floors);

int get_stu(string stu_name, vector<struct student>& students);

//学生转移房间
void stu_move_from_to(string s, int i, string stu_name, vector<struct student>& students, vector<struct floor>& floors);

//学生退学
void stu_quit(string stu_name, vector<struct student>& students, vector<struct floor>& floors);

//学生入学
void stu_enrol(string stu_name, vector<struct student>& students, vector<struct floor>& floors);

//学生进餐,打工，学习
double calculate(string fomula, double eat_time, double work_time, double study_time, double weight_inc, double knowledge_inc, struct InitData init_data);

//学生A和B间魅力差计算
double charm_sub_two(int stu_1, int stu_2, vector<struct student>& students);

//插入第三者的魅力差计算
double charm_sub_thrid(int stu_1, int stu_2, int stu_3, vector<struct student>& students);

//A求爱B
void love_to(string stu_1, string stu_2, vector<struct student>& students, struct InitData init_data);

//A和B分手
void stu_break_up(string stu_1, string stu_2, vector<struct student>& students);

//读取有关学生的命令
std::istream& student_data_change(std::istream& in, string stu_name, vector<struct student>& students, vector<struct floor>& floors, struct InitData init_data);

//读取并执行命令
std::istream& read_and_exec_instructions(std::istream& in, struct InitData init_data, vector<struct student>& students, vector<struct floor>& floors);

#endif