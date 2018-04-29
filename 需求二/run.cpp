#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<stack>
#include<cmath>
#include<algorithm>
#include"init_struct.h"
#include"read_config.h"
#include"read_dispatch.h"
#include"read_instructions.h"
#include"query_interface.h"
double get_double_num(string s, int& i);
//初始化宿舍的层数和房间数和房间的初始容量
void build_floors(struct InitData init_data, vector<struct floor>& floors);


int main(int argc, char* argv[])
{
    std::ifstream config;
    std::ifstream dispatch;
    std::ifstream instructions;
    
    //读入初始化数据
    struct InitData init_data;
    config.open(argv[1]);
    read_config(config, init_data);
    
    //初始化宿舍的层数和房间数和房间的初始容量
    vector<struct floor> floors;
    build_floors(init_data, floors);
    
    //读入学生信息
    
    vector<struct student> students;
    dispatch.open(argv[2]);
    read_dispatch(dispatch, students, floors, init_data);
    
    //读取并执行命令
    instructions.open(argv[3]);
    read_and_exec_instructions(instructions, init_data, students, floors);
    
    query_interface_start(cin, cout, students, floors);
    
    return 0;
}


//初始化宿舍的层数和房间数和房间的初始容量
void build_floors(struct InitData init_data, vector<struct floor>& floors)
{
    for(int i = 0; i < init_data.FLOOR; i++)
    {
        struct floor f;
        for(int j = 0; j < init_data.ROOM; j++)
        {
            f.room.push_back(std::make_pair<int,int>(init_data.CAPACITY, 0));
           // cout << "第 "<<j<<" 房  ";
        }
        floors.push_back(f);
        //cout << "第 "<<i<<" 层建造完成" << std::endl;
    }
    //cout << "初始房间容量: "<<floors[0].room[0].first<<std::endl;
}


/*************************read_instructions.h***********************************************/
//追加层数x
std::istream& add_floor(std::istream& in, int x, struct InitData init_data, vector<struct floor>& floors)
{
    for(int i = 0; i < x; i++)
    {
        struct floor f;
        for(int j = 0; j < init_data.ROOM; j++)
            f.room.push_back(std::make_pair<int,int>(init_data.CAPACITY, 0));
        floors.push_back(f);
    }
    //cout<<"添加楼层命令：添加"<<x<<"层； 现在共有 "<<floors.size()<<" 层"<<std::endl;
    //cout<<std::endl;
    return in;
}

//追加某层的房间数x
std::istream& add_room(std::istream& in, int x, struct InitData init_data, vector<struct floor>& floors)
{
    string s1, s2;
    int floor;
    in >> s1 >> floor >> s2;
    for(int j = 0; j < x; j++)
        floors[floor].room.push_back(std::make_pair<int,int>(init_data.CAPACITY, 0));
    //cout<<"添加房间命令："<<"在 "<<floor<<" 层添加 "<<x<<" 个房间；现在这一层共有 "<<floors[floor].room.size()<<" 个房间"<<std::endl;
    //cout<<std::endl;
    return in;
}

//追加某房间的容量x
std::istream& add_bed(std::istream& in, int x, struct InitData init_data, vector<struct floor>& floors)
{
    string s, floor_room;
    in >> s >> floor_room;
    
    int floor = (floor_room[0]-'0')*10 + (floor_room[1]-'0');
    int room = (floor_room[3]-'0')*10 + (floor_room[4]-'0');
    floors[floor].room[room].first += x;
    /*
    cout<<"添加房间容量命令：在房间 ";
    if(floor < 10) cout<<"0";
    cout<<floor<<"-";
    if(room < 10) cout<<"0";
    cout<<room<<" 追加容量 "<<x<<" ， 现在此房间的容量为 "<<floors[floor].room[room].first<<std::endl;
    cout<<std::endl;
    */
    return in;
}


int get_stu(string stu_name, vector<struct student>& students)
{
    vector<struct student>::size_type j;
    for(j = 0; j < students.size(); j++)
    {
        if(students[j].name != stu_name)
            continue;
        else
            return j;
    }
    return j;
}

//学生转移房间
void stu_move_from_to(string s, int i, string stu_name, vector<struct student>& students, vector<struct floor>& floors)
{
    int from_floor = (s[i+10]-'0')*10 + (s[i+11]-'0');
    int from_room = (s[i+13]-'0')*10 + (s[i+14]-'0');
    int to_floor = (s[i+19]-'0')*10 + (s[i+20]-'0');
    int to_room = (s[i+22]-'0')*10 + (s[i+23]-'0');
    
    //更改房间信息
    if(floors[to_floor].room[to_room].first == floors[to_floor].room[to_room].second)
    {
        cout << "Room full!" << std::endl;
        exit(0);
    }
    
    floors[from_floor].room[from_room].second--;
    floors[to_floor].room[to_room].second++;
    
    //更改学生信息
    vector<struct student>::size_type j = get_stu(stu_name, students);
    if(j == students.size())
    {
        cout << "Error!! No fine student!!" << std::endl;
        exit(0);
    }
    students[j].floor = to_floor;
    students[j].room = to_room;
    /*
    cout<<"把学生 "<<stu_name<<" 从房间 ";
    if(from_floor < 10) cout<<"0";
    cout<<from_floor<<"-";
    if(from_room < 10) cout<<"0";
    cout<<from_room<<" 转移到房间 ";
    if(to_floor < 10) cout<<"0";
    cout<<to_floor<<"-";
    if(to_room < 10) cout<<"0";
    cout<<to_room<<std::endl;
    */
}

//学生退学
void stu_quit(string stu_name, vector<struct student>& students, vector<struct floor>& floors)
{
    vector<struct student>::size_type j = get_stu(stu_name, students);
    if(j == students.size())
    {
        cout << "Error!! No fine student!!" << std::endl;
        exit(0);
    }
    /*
    if(students[j].quit)
    {
        cout << "Error, student already quit!" << std::endl;
        exit(0);
    }
    */
    floors[students[j].floor].room[students[j].room].second--;
    students[j].quit = true;
    
    //cout<<"学生 "<<stu_name<<" 退学"<<std::endl;
}

//学生入学
void stu_enrol(string stu_name, vector<struct student>& students, vector<struct floor>& floors)
{
    vector<struct student>::size_type j = get_stu(stu_name, students);
    if(j == students.size())
    {
        cout << "Error!! No fine student!!" << std::endl;
        exit(0);
    }
    /*
    if(!students[j].quit)
    {
        cout << "Error, student already enter!" << std::endl;
        exit(0);
    }
    */
    if(floors[students[j].floor].room[students[j].room].first == floors[students[j].floor].room[students[j].room].second)
    {
        cout << "Room full!" << std::endl;
        exit(0);
    }
    floors[students[j].floor].room[students[j].room].second++;
    students[j].quit = false;
    
    //cout<<"学生 "<<stu_name<<" 入学"<<std::endl;
}

//学生进餐,打工，学习
double calculate(string fomula, double eat_time, double work_time, double study_time, double weight_inc, double knowledge_inc, struct InitData init_data)
{
    stack<double> Stack;
    //string fomula_weight_inc = init_data.WEIGHT_INC;
    
    for(int i = 0; i < fomula.size();)
    {
        if(fomula[i] >= '0' && fomula[i] <= '9')
            Stack.push(get_double_num(fomula, i));
        else if(fomula[i] == 'F')
        {
            i = i + 5;
            if(fomula[i] == 'T')
            {
                Stack.push(eat_time);
                i = i + 5;
            }
            if(fomula[i] == 'W')
            {
                Stack.push(init_data.FOOD_WEIGHT);
                i = i + 6;
            }
            if(fomula[i] == 'C')
            {
                Stack.push(init_data.FOOD_COST);
                i = i + 4;
            }
        }
        else if(fomula[i] == 'S')
        {
            i = i + 6;
            if(fomula[i] == 'T')
            {
                Stack.push(study_time);
                i = i + 4;
            }
            if(fomula[i] == 'W')
            {
                Stack.push(init_data.STUDY_WEIGHT);
                i = i + 6;
            }
            if(fomula[i] == 'K')
            {
                Stack.push(init_data.STUDY_KNOWLEDGE);
                i = i + 9;
            }
        }
        else if(fomula[i] == 'W')
        {
            i = i + 5;
            if(fomula[i] == 'T')
            {
                if(fomula[i+2] == 'I')
                {
                    Stack.push(weight_inc);
                    i = i + 5;
                }
                else
                {
                    Stack.push(work_time);
                    i = i + 4;
                }
            }
            if(fomula[i] == 'M')
            {
                Stack.push(init_data.WORK_MONEY);
                i = i + 5;
            }
            if(fomula[i] == 'W')
            {
                Stack.push(init_data.WORK_WEIGHT);
                i = i + 6;
            }
        }
        else if(fomula[i] == 'K')
        {
            Stack.push(knowledge_inc);
            i = i + 13;
        }
        
        else if(fomula[i] == '-')
        {
            double num1 = Stack.top();
            Stack.pop();
            double num2 = Stack.top();
            Stack.pop();
            
            Stack.push(num2 - num1);
            i++;
        }
        else if(fomula[i] == '*')
        {
            double num1 = Stack.top();
            Stack.pop();
            double num2 = Stack.top();
            Stack.pop();
            
            Stack.push(num1 * num2);
            i++;
        }
        else i++;
    }
    
    double ans = Stack.top();
    return ans;
}

//学生A和B间魅力差计算
double charm_sub_two(int stu_1, int stu_2, vector<struct student>& students)
{
    double x = students[stu_1].charm;
    double y = students[stu_2].charm;
    
    return x > y ? (x-y)/x : (y-x)/y;
}

//插入第三者的魅力差计算
double charm_sub_thrid(int stu_1, int stu_2, int stu_3, vector<struct student>& students)
{
    return (charm_sub_two(stu_1, stu_2, students) - charm_sub_two(stu_1, stu_3, students)) / charm_sub_two(stu_1, stu_3, students);
}

//A求爱B
void love_to(string stu_1, string stu_2, vector<struct student>& students, struct InitData init_data)
{
    int stu_l = get_stu(stu_1, students);
    int stu_ll = get_stu(stu_2, students);
    
    if(students[stu_l].is_loving)
    {
        //cout << students[stu_l].name << "在恋爱中，不可以成为第三者" << std::endl;
        return;
    }
    
    if(init_data.LOVE_THRESHOLD >= charm_sub_two(stu_l, stu_ll, students) )
    {
        if(students[stu_ll].is_loving)
        {
            int temp_stu = get_stu(students[stu_ll].love_queue.front().name, students);
            if(init_data.LOVER_CHANGE_THRESHOLD <= charm_sub_thrid(stu_l, stu_ll, temp_stu, students) )
            {
                students[temp_stu].is_loving = false;
                students[stu_l].is_loving = true;
                
                struct student l = students[stu_l], ll = students[stu_ll];
                students[stu_l].love_queue.push(ll);
                students[stu_ll].love_queue.push(l);
                //cout << students[stu_ll].name << "与原配 "<<students[temp_stu].name<<"分手 往事如烟" << std::endl;
            }
        }
        else
        {
            students[stu_l].is_loving = true;
            students[stu_ll].is_loving = true;
            
            struct student l = students[stu_l], ll = students[stu_ll];
            students[stu_l].love_queue.push(ll);
            students[stu_ll].love_queue.push(l);
            
            //cout<<"OK"<<std::endl;
        }
    }
    else
    {
        //cout<<"魅力不够啊"<<std::endl;
    }
    
    return;
}

//A和B分手
void stu_break_up(string stu_1, string stu_2, vector<struct student>& students)
{
    int stu_l = get_stu(stu_1, students);
    int stu_ll = get_stu(stu_2, students);
    
    students[stu_l].is_loving = false;
    students[stu_ll].is_loving = false;
}

//读取有关学生的命令
std::istream& student_data_change(std::istream& in, string stu_name, vector<struct student>& students, vector<struct floor>& floors, struct InitData init_data)
{
    string s;
    double eat_time = 0, study_time = 0, work_time = 0;
    getline(in, s);
    for(int i = 0; i < s.size(); i++)
    {
        if(s[s.size()-1] == 'P')
        {
            if(s[i] == ' ' || s[i] == ' ') continue;
            else
            {
                string stu_name2;
                for(int j = i; i < s.size(); i++)
                {
                    if(s[i] == ' ') break;
                    else
                    {
                        stu_name2.push_back(s[i]);
                    }
                }
                stu_break_up(stu_name, stu_name2, students);
                break;
            }
        }
        
        if(s[i] == ' ' || s[i] == ' ') continue;
        else if(s[i] == 'M')
        {
            stu_move_from_to(s, i, stu_name, students, floors);
            break;
        }
        else if(s[i] == 'Q')
        {
            stu_quit(stu_name, students, floors);
            break;
        }
        else if(s[i] == 'E')
        {
            if(s[i+1] == 'A')
            {
                i = i + 4;
                eat_time += get_double_num(s, i);
            }
            else if(s[i+1] == 'N')
            {
                stu_enrol(stu_name, students, floors);
                break;
            }
        }
        else if(s[i] == 'S')
        {
            i = i + 6;
            study_time += get_double_num(s, i);
        }
        else if(s[i] == 'W')
        {
            if(s[i+1] == 'O' && s[i+2] == 'O')
            {
                string stu_name2 = s.substr(i+4, s.size());
                //cout<<stu_name<<" LOVE "<<stu_name2<<std::endl;
                love_to(stu_name, stu_name2, students, init_data);
                break;
            }
            else
            {
                i = i + 5;
                work_time += get_double_num(s, i);
            }
        }
    }
    
    vector<struct student>::size_type j = get_stu(stu_name, students);
    if(j == students.size())
    {
        cout << "Error!! No fine student!!" << std::endl;
        exit(0);
    }
    /*
    cout<<"学生 "<<students[j].name<<" weight="<<students[j].weight<<" money="<<students[j].money<<" knowledge="<<students[j].knowledge<<" charm="<<students[j].charm;
    cout<<" eat_time="<<eat_time<<" work_time="<<work_time<<" study_time="<<study_time<<std::endl;
    */
    double knowledge_inc = calculate(init_data.KNOWLEDGE_INC, eat_time, work_time, study_time, 0, 0, init_data);
    double weight_inc = calculate(init_data.WEIGHT_INC, eat_time, work_time, study_time, 0, 0, init_data);
    double money_inc = calculate(init_data.MONEY_INC, eat_time, work_time, study_time, 0, 0, init_data);
    double charm_inc = calculate(init_data.CHARM_INC, eat_time, work_time, study_time, weight_inc, knowledge_inc, init_data);
    //cout << "weight_inc="<<weight_inc<<" money_inc="<<money_inc<<" knowledge_inc="<<knowledge_inc<<" charm_inc="<<charm_inc<<std::endl;
    
    if(students[j].knowledge + knowledge_inc >= 1 && students[j].knowledge + knowledge_inc <=100)
        students[j].knowledge += knowledge_inc;
    if(students[j].weight + weight_inc >= 0)
        students[j].weight += weight_inc;
    if(students[j].money + money_inc >= 0)
        students[j].money += money_inc;
    if(students[j].charm + charm_inc >= 1 && students[j].charm + charm_inc <= 100)
        students[j].charm += charm_inc;
    /*
    cout<<"改变后的学生信息：";
    cout<<" weight="<<students[j].weight;
    cout<<" money="<<students[j].money;
    cout<<" knowledge="<<students[j].knowledge;
    cout<<" charm="<<students[j].charm<<std::endl;
    cout<<std::endl;
    */
    return in;
}

//读取并执行命令
std::istream& read_and_exec_instructions(std::istream& in, struct InitData init_data, vector<struct student>& students, vector<struct floor>& floors)
{
    
    while(in)
    {
        string first_data;
        in >> first_data;
        
        if(!in.good()) break;
        
        //有关楼层数房间数和房间容量的命令
        if(first_data == "ADD")
        {
            int second_data;
            string third_data;
            in >> second_data >> third_data;
            if(third_data == "FLOOR(s)")
                add_floor(in, second_data, init_data, floors);
            else if(third_data == "ROOM(s)")
                add_room(in, second_data, init_data, floors);
            else if(third_data == "BED(s)")
                add_bed(in, second_data, init_data, floors);
        }
        //第一个数据是名字
        else
        {
            student_data_change(in, first_data, students, floors, init_data);
        }
    }
    in.clear();
    return in;
}
/*************************read_instructions.h***********************************************/

/*************************read_dispatch.h***************************************************/
//读入并初始化学生信息
std::istream& read_dispatch(std::istream& in, vector<struct student>& students, vector<struct floor>& floors, struct InitData init_data)
{
    while(in)
    {
        string floor_room;
        struct student stu;
        in >> stu.name >> stu.sex >> floor_room;
        
        if(!in.good()) break;
        
        stu.floor = (floor_room[0]-'0')*10 + (floor_room[1]-'0');
        stu.room = (floor_room[3]-'0')*10 + (floor_room[4]-'0');
        floors[stu.floor].room[stu.room].second++;
        
        stu.charm = init_data.CHARM;
        stu.knowledge = init_data.KNOWLEDGE;
        stu.money = init_data.MONEY;
        stu.weight = init_data.WEIGHT;
        stu.quit = false;
        stu.is_loving = false;
        
        students.push_back(stu);
        
    }
    
    //按学生姓名排序
    sort(students.begin(), students.end());
    /*
    cout << "按姓名排序后输出的学生房间信息: " << std::endl;
    for(vector<struct student>::size_type i = 0; i < students.size(); i++)
    {
        cout<<students[i].name<<" "<<"学生房间：";
        if(students[i].floor < 10) cout<<"0";
        cout<<students[i].floor<<"-";
        if(students[i].room < 10) cout<<"0";
        cout<<students[i].room<<"  "<<"房间人数："<<floors[students[i].floor].room[students[i].room].second<<std::endl;
    }
     cout<<std::endl;
    */
     /*
    cout << "按学生姓名排序后输出的学生信息如下：" << std::endl;
    for(vector<struct student>::size_type i = 0; i < students.size(); i++)
        cout<<students[i].name<<" "<<students[i].sex<<" "<<students[i].floor<<"-"<<students[i].room<<"  "<<"学生的房间人数："<<floors[students[i].floor].room[students[i].room].second<<std::endl;
    */
    in.clear();
    return in;
}
/*************************read_dispatch.h***************************************************/

/*************************read_config.h*****************************************************/
//判断字符串 Template 是否等于 Sample 的开头
bool is(string Template, string Sample)
{
    
    for(int i = 0; i < Template.size(); i++)
    {
        if(Template[i] == Sample[i])
            continue;
        else
            return false;
    }
    //cout << "正在读取："<<Sample<<std::endl;
    return true;
}


//从字符串得到数字
double get_double_num(string s, int& i)
{
    double temp1 = 0, temp2 = 0;
    int cnt = 1;
    bool j = false;
    for(; i < s.size(); i++)
    {
        if(s[i] <= '9' && s[i] >= '0')
        {
            if(j == false)
                temp1 = temp1 * 10 + (s[i] - '0');
            else
            {
                temp2 = temp2 + (s[i] - '0') * pow(0.1, cnt);
                cnt++;
            }
        }
        else if(s[i] == '.')
            j = true;
        else break;
    }
    //cout<<"得到的数字："<<temp1+temp2<<std::endl;
    return temp1 + temp2;
}

//把初始化数据读入结构体
void read_initData(string s, int& init_data)
{
    int temp = 0;
    for(int i = 0; i < s.size(); i++)
    {
        if(s[i] > '9' || s[i] < '0')
            continue;
        else
            temp = temp * 10 + (s[i] - '0');
    }
    
    init_data = temp;
    
    //cout << s << "    " << init_data << std::endl;
}
void read_initData(string s, double& init_data, int i)
{
    init_data = get_double_num(s, i);
    //cout << s << "    " << init_data << std::endl;
}

//将中缀公式转换成后缀公式并返回
string infix_to_postifx(string fomula_infx)
{
    stack<char> Stack;
    string ans;
    
    for(string::size_type i = 0; i < fomula_infx.size(); i++)
    {
        char c = fomula_infx[i];
        
        if(c == '(') Stack.push(c);
        else if(c == '-')
        {
            while(!Stack.empty() && Stack.top() != '(')
            {
                ans.push_back(Stack.top());
                Stack.pop();
            }
            Stack.push(c);
        }
        else if(c == '*')
        {
            while(!Stack.empty() && Stack.top() == '*')
            {
                ans.push_back(Stack.top());
                Stack.pop();
            }
            Stack.push(c);
        }
        else if(c == ')')
        {
            while(Stack.top() != '(')
            {
                ans.push_back(Stack.top());
                Stack.pop();
            }
            
            if(Stack.top() == '(') Stack.pop();
        }
        else
            ans.push_back(c);
    }
    
    while(!Stack.empty())
    {
        ans.push_back(Stack.top());
        Stack.pop();
    }
    
    return ans;
}

//把初始化数据里的公式转换成后缀公式然后读入结构体
void read_formula(string s, string& init_data)
{
    init_data = infix_to_postifx(s);
    //cout<<s <<std::endl;
    //cout<<init_data <<std::endl;
}

//读取文件 config 的初始化数据
std::istream& read_config(std::istream& in, struct InitData& init_data)
{
    string s;
    while(getline(in, s))
    {
        //
        if(s[0] == '/' && s[1] == '/')
            continue;
        else if(is("FLOOR:", s))
            read_initData(s, init_data.FLOOR);
        else if(is("ROOM:", s))
            read_initData(s, init_data.ROOM);
        else if(is("CAPACITY:", s))
            read_initData(s, init_data.CAPACITY);
        else if(is("WEIGHT:", s))
            read_initData(s, init_data.WEIGHT);
        else if(is("MONEY:", s))
            read_initData(s, init_data.MONEY);
        else if(is("KNOWLEDGE:", s))
            read_initData(s, init_data.KNOWLEDGE);
        else if(is("CHARM:", s))
            read_initData(s, init_data.CHARM);
        else if(is("FOOD_WEIGHT:", s))
            read_initData(s, init_data.FOOD_WEIGHT);
        else if(is("FOOD_COST:", s))
            read_initData(s, init_data.FOOD_COST);
        else if(is("STUDY_WEIGHT:", s))
            read_initData(s, init_data.STUDY_WEIGHT);
        else if(is("STUDY_KNOWLEDGE:", s))
            read_initData(s, init_data.STUDY_KNOWLEDGE);
        else if(is("WORK_MONEY:", s))
            read_initData(s, init_data.WORK_MONEY);
        else if(is("WORK_WEIGHT:", s))
            read_initData(s, init_data.WORK_WEIGHT);
        
        
        else if(is("WEIGHT_INC", s))
            read_formula(s, init_data.WEIGHT_INC);
        else if(is("MONEY_INC", s))
            read_formula(s, init_data.MONEY_INC);
        else if(is("KNOWLEDGE_INC", s))
            read_formula(s, init_data.KNOWLEDGE_INC);
        else if(is("CHARM_INC", s))
            read_formula(s, init_data.CHARM_INC);
        
        else if(is("LOVE_THRESHOLD:", s))
            read_initData(s, init_data.LOVE_THRESHOLD, 16);
        else if(is("LOVER_CHANGE_THRESHOLD:", s))
            read_initData(s, init_data.LOVER_CHANGE_THRESHOLD, 24);
        
        //else
          //  cout << s << std::endl;
    }
    in.clear();
    return in;
}
/*************************read_config.h*****************************************************/



/************************query_interface*****************************************************/
void query_interface_start(std::istream& in, std::ostream& out, vector<struct student>& students, vector<struct floor>& floors)
{
    while(true)
    {
        out << "请输入你的查询命令：";
        
        string com_str;
        getline(in, com_str);
        
        //帮助
        if(com_str.size() == 0)
            out << "请输入 ? 查看帮助。" << std::endl;
        else if(com_str == "?" || com_str == "？" || com_str == "help")
            print_help();
        
        //学生信息查询
        else if(is("weight ", com_str))
        {
            com_str = com_str.substr(7, com_str.size());
            //out << com_str << std::endl;
            
            if(is("max", com_str))
            {
                com_str = com_str.substr(3, com_str.size());
                //out << com_str << std::endl;
                if(com_str == "")
                    get_weight(students, "max", "", false, 0, 0);
                else if(com_str == " boy")
                    get_weight(students, "max", "男", false, 0, 0);
                else if(com_str == " girl")
                    get_weight(students, "max", "女", false, 0, 0);
                else
                    print_err("weight", out);
            }
            else if(is("min", com_str))
            {
                com_str = com_str.substr(3, com_str.size());
                //out << com_str << std::endl;
                if(com_str == "")
                    get_weight(students, "min", "", false, 0, 0);
                else if(com_str == " boy")
                    get_weight(students, "min", "男", false, 0, 0);
                else if(com_str == " girl")
                    get_weight(students, "min", "女", false, 0, 0);
                else
                    print_err("weight", out);
            }
            else if(is("rang", com_str))
            {
                string::size_type cnt;
                
                cnt = com_str.find('(');
                if(cnt == com_str.size()) print_err("rang", out);
                double rang_x = get_double(com_str, cnt+1);
                
                cnt = com_str.find(',');
                if(cnt == com_str.size()) print_err("rang", out);
                double rang_y = get_double(com_str, cnt+1);
                
                //out << rang_x << " " << rang_y << std::endl;
                
                cnt = com_str.find(')');
                if(cnt == com_str.size()) print_err("rang", out);
                com_str = com_str.substr(cnt+1, com_str.size());
                //out << com_str << std::endl;
                
                if(com_str == "" || is(" D", com_str))
                    get_weight(students, "max", "", true, rang_x, rang_y);
                else if(is(" U", com_str))
                    get_weight(students, "min", "", true, rang_x, rang_y);
                else if(is(" boy", com_str))
                {
                    if(com_str[com_str.size()-1] == 'U')
                        get_weight(students, "min", "男", true, rang_x, rang_y);
                    else
                        get_weight(students, "max", "男", true, rang_x, rang_y);
                }
                else if(is(" girl", com_str))
                {
                    if(com_str[com_str.size()-1] == 'U')
                        get_weight(students, "min", "女", true, rang_x, rang_y);
                    else
                        get_weight(students, "max", "女", true, rang_x, rang_y);
                }
                else
                    print_err("weight", out);
            }
            
            else
                print_err("weight", out);
        }
        else if(is("money ", com_str))
        {
            com_str = com_str.substr(6, com_str.size());
            //out << com_str << std::endl;
            
            if(is("max", com_str))
            {
                com_str = com_str.substr(3, com_str.size());
                //out << com_str << std::endl;
                if(com_str == "")
                    get_money(students, "max", "", false, 0, 0);
                else if(com_str == " boy")
                    get_money(students, "max", "男", false, 0, 0);
                else if(com_str == " girl")
                    get_money(students, "max", "女", false, 0, 0);
                else
                    print_err("money", out);
            }
            else if(is("min", com_str))
            {
                com_str = com_str.substr(3, com_str.size());
                //out << com_str << std::endl;
                if(com_str == "")
                    get_money(students, "min", "", false, 0, 0);
                else if(com_str == " boy")
                    get_money(students, "min", "男", false, 0, 0);
                else if(com_str == " girl")
                    get_money(students, "min", "女", false, 0, 0);
                else
                    print_err("money", out);
            }
            else if(is("rang", com_str))
            {
                string::size_type cnt;
                
                cnt = com_str.find('(');
                if(cnt == com_str.size()) print_err("rang", out);
                double rang_x = get_double(com_str, cnt+1);
                
                cnt = com_str.find(',');
                if(cnt == com_str.size()) print_err("rang", out);
                double rang_y = get_double(com_str, cnt+1);
                
                //out << rang_x << " " << rang_y << std::endl;
                
                cnt = com_str.find(')');
                if(cnt == com_str.size()) print_err("rang", out);
                com_str = com_str.substr(cnt+1, com_str.size());
                //out << com_str << std::endl;
                
                if(com_str == "" || is(" D", com_str))
                    get_money(students, "max", "", true, rang_x, rang_y);
                else if(is(" U", com_str))
                    get_money(students, "min", "", true, rang_x, rang_y);
                else if(is(" boy", com_str))
                {
                    if(com_str[com_str.size()-1] == 'U')
                        get_money(students, "min", "男", true, rang_x, rang_y);
                    else
                        get_money(students, "max", "男", true, rang_x, rang_y);
                }
                else if(is(" girl", com_str))
                {
                    if(com_str[com_str.size()-1] == 'U')
                        get_money(students, "min", "女", true, rang_x, rang_y);
                    else
                        get_money(students, "max", "女", true, rang_x, rang_y);
                }
                else
                    print_err("money", out);
            }
            
            else
                print_err("money", out);
        }
        else if(is("knowledge ", com_str))
        {
            com_str = com_str.substr(10, com_str.size());
            //out << com_str << std::endl;
            
            if(is("max", com_str))
            {
                com_str = com_str.substr(3, com_str.size());
                //out << com_str << std::endl;
                if(com_str == "")
                    get_knowledge(students, "max", "", false, 0, 0);
                else if(com_str == " boy")
                    get_knowledge(students, "max", "男", false, 0, 0);
                else if(com_str == " girl")
                    get_knowledge(students, "max", "女", false, 0, 0);
                else
                    print_err("knowledge", out);
            }
            else if(is("min", com_str))
            {
                com_str = com_str.substr(3, com_str.size());
                //out << com_str << std::endl;
                if(com_str == "")
                    get_knowledge(students, "min", "", false, 0, 0);
                else if(com_str == " boy")
                    get_knowledge(students, "min", "男", false, 0, 0);
                else if(com_str == " girl")
                    get_knowledge(students, "min", "女", false, 0, 0);
                else
                    print_err("knowledge", out);
            }
            else if(is("rang", com_str))
            {
                string::size_type cnt;
                
                cnt = com_str.find('(');
                if(cnt == com_str.size()) print_err("rang", out);
                double rang_x = get_double(com_str, cnt+1);
                
                cnt = com_str.find(',');
                if(cnt == com_str.size()) print_err("rang", out);
                double rang_y = get_double(com_str, cnt+1);
                
                //out << rang_x << " " << rang_y << std::endl;
                
                cnt = com_str.find(')');
                if(cnt == com_str.size()) print_err("rang", out);
                com_str = com_str.substr(cnt+1, com_str.size());
                //out << com_str << std::endl;
                
                if(com_str == "" || is(" D", com_str))
                    get_knowledge(students, "max", "", true, rang_x, rang_y);
                else if(is(" U", com_str))
                    get_knowledge(students, "min", "", true, rang_x, rang_y);
                else if(is(" boy", com_str))
                {
                    if(com_str[com_str.size()-1] == 'U')
                        get_knowledge(students, "min", "男", true, rang_x, rang_y);
                    else
                        get_knowledge(students, "max", "男", true, rang_x, rang_y);
                }
                else if(is(" girl", com_str))
                {
                    if(com_str[com_str.size()-1] == 'U')
                        get_knowledge(students, "min", "女", true, rang_x, rang_y);
                    else
                        get_knowledge(students, "max", "女", true, rang_x, rang_y);
                }
                else
                    print_err("knowledge", out);
            }
            
            else
                print_err("knowledge", out);
        }
        else if(is("charm ", com_str))
        {
            com_str = com_str.substr(6, com_str.size());
            //out << com_str << std::endl;
            
            if(is("max", com_str))
            {
                com_str = com_str.substr(3, com_str.size());
                //out << com_str << std::endl;
                if(com_str == "")
                    get_charm(students, "max", "", false, 0, 0);
                else if(com_str == " boy")
                    get_charm(students, "max", "男", false, 0, 0);
                else if(com_str == " girl")
                    get_charm(students, "max", "女", false, 0, 0);
                else
                    print_err("charm", out);
            }
            else if(is("min", com_str))
            {
                com_str = com_str.substr(3, com_str.size());
                //out << com_str << std::endl;
                if(com_str == "")
                    get_charm(students, "min", "", false, 0, 0);
                else if(com_str == " boy")
                    get_charm(students, "min", "男", false, 0, 0);
                else if(com_str == " girl")
                    get_charm(students, "min", "女", false, 0, 0);
                else
                    print_err("charm", out);
            }
            else if(is("rang", com_str))
            {
                string::size_type cnt;
                
                cnt = com_str.find('(');
                if(cnt == com_str.size()) print_err("rang", out);
                double rang_x = get_double(com_str, cnt+1);
                
                cnt = com_str.find(',');
                if(cnt == com_str.size()) print_err("rang", out);
                double rang_y = get_double(com_str, cnt+1);
                
                //out << rang_x << " " << rang_y << std::endl;
                
                cnt = com_str.find(')');
                if(cnt == com_str.size()) print_err("rang", out);
                com_str = com_str.substr(cnt+1, com_str.size());
                //out << com_str << std::endl;
                
                if(com_str == "" || is(" D", com_str))
                    get_charm(students, "max", "", true, rang_x, rang_y);
                else if(is(" U", com_str))
                    get_charm(students, "min", "", true, rang_x, rang_y);
                else if(is(" boy", com_str))
                {
                    if(com_str[com_str.size()-1] == 'U')
                        get_charm(students, "min", "男", true, rang_x, rang_y);
                    else
                        get_charm(students, "max", "男", true, rang_x, rang_y);
                }
                else if(is(" girl", com_str))
                {
                    if(com_str[com_str.size()-1] == 'U')
                        get_charm(students, "min", "女", true, rang_x, rang_y);
                    else
                        get_charm(students, "max", "女", true, rang_x, rang_y);
                }
                else
                    print_err("charm", out);
            }
            
            else
                print_err("charm", out);
        }
        
        //查询某学生的房间号
        else if(is("stu ", com_str))
        {
            com_str = com_str.substr(4, com_str.size());
            
            int stu_id = get_stu(com_str, students);
            
            if(stu_id == students.size())
            {
                cout << std::endl << "学生不存在。" << std::endl;
                continue;
            }
            
            if(students[stu_id].quit)
                cout << std::endl << "房间号：" << std::endl;
            else
            {
                cout << std::endl << "房间号：";
                if(students[stu_id].floor < 10) cout << "0";
                cout << students[stu_id].floor << "-";
                if(students[stu_id].room < 10) cout << "0";
                cout << students[stu_id].room << std::endl;
            }
            cout << std::endl;
        }
        
        //房间学生查询
        else if(is("room ", com_str))
        {
            com_str = com_str.substr(5, com_str.size());
            //out << com_str << std::endl;
            
            int floor = (com_str[0]-'0')*10 + (com_str[1]-'0');
            int room = (com_str[3]-'0')*10 + (com_str[4]-'0');
            
            if(floor > floors.size() || room > floors[floor].room.size())
            {
                cout << std::endl << "房间不存在。" << std::endl;
                continue;
            }
            
            get_stu_room(floor, room, students);
            cout << std::endl;
        }
        
        //学生恋爱历史查询
        else if(is("love ", com_str))
        {
            com_str = com_str.substr(5, com_str.size());
            //out << com_str << std::endl;
            
            if(is("max_lover", com_str))
            {
                get_lover_maxlovers(students);
            }
            else if(is("max_charm", com_str))
            {
                get_lover_maxcharm(students);
            }
            else if(is("list ", com_str))
            {
                com_str = com_str.substr(5, com_str.size());
                
                string::size_type cnt;
                cnt = com_str.find(' ');
                
                string stu_name = com_str.substr(0, cnt);
                //cout << stu_name << std::endl;
                int stu_id = get_stu(stu_name, students);
                
                com_str = com_str.substr(cnt+1, com_str.size());
                
                get_lover(students, com_str, stu_id);
            }
            else
                print_err("love", cout);
        }
        
        else if(is("quit", com_str))
        {
            break;
        }
        
        else
            out << "未知命令形式" << std::endl;
    }
}


/****************************查询界面的通用函数*****************************/
double get_double(string s, string::size_type i)
{
    double temp1 = 0, temp2 = 0;
    int cnt = 1;
    bool j = false;
    for(; i < s.size(); i++)
    {
        if(s[i] <= '9' && s[i] >= '0')
        {
            if(j == false)
                temp1 = temp1 * 10 + (s[i] - '0');
            else
            {
                temp2 = temp2 + (s[i] - '0') * pow(0.1, cnt);
                cnt++;
            }
        }
        else if(s[i] == '.')
            j = true;
        else break;
    }
    //cout<<"得到的数字："<<temp1+temp2<<std::endl;
    return temp1 + temp2;
}

void print_help()
{
    cout << "以下是可选的查询命令：" << std::endl;
    cout << std::endl;
    cout << "weight max/min (boy/girl)         -- 体重最大值最小值查询，可以指定性别(boy/girl)。" << std::endl;
    cout << "weight rang(x,y) (boy/girl) (U/D) -- 体重范围查询：" << std::endl;
    cout << "                                        + 查询体重范围在[x,y]内的学生;" << std::endl;
    cout << "                                        + 可以指定性别(boy/girl);" << std::endl;
    cout << "                                        + 可以指定升序(U)或降序(D);" << std::endl;
    cout << "                                        + 当同时指定性别和排序时，应使性别在前，排序在后。" << std::endl;
    cout << std::endl;
    cout << "                                  -- 可以将weight替换为money/knowledge/charm进行查询。" << std::endl;
    cout << std::endl;
    cout << "stu name                          -- 查询学生的房间号。" << std::endl;
    cout << std::endl;
    cout << "room floor-room                   -- 查询某房间入住的所有学生。" << std::endl;
    cout << std::endl;
    cout << "love max_lover/max_charm          -- 查询所有学生中恋人最多(谈过恋爱的魅力值最高)的人。" << std::endl;
    cout << "love list name (time/name/charm)  -- 查询学生(name)的恋爱历史，可用time/name/charm排序。" << std::endl;
    cout << std::endl;
    cout << "quit                              -- 退出。" << std::endl;
}

void print_err(const string& err, std::ostream& out)
{
    out << "在执行 " << err << "命令时出现错误。" << std::endl;
}

void print_message(vector<struct student>& result_stu)
{
    vector<struct student>::size_type cnt = 0;
    
    if(result_stu.size() == 0)
        cout << "无复合要求学生。" <<std::endl;
    
    for(vector<struct student>::const_iterator i = result_stu.begin(); i != result_stu.end(); i++)
    {
        cout << std::endl;
        cnt++;
        if(cnt % 10 == 0)
        {
            cout << "[输入任何符号将继续输出]";
            
            string temp;
            getline(cin, temp);
        }
        
        cout << i->name << " " << i->sex << std::endl;
        cout << "weight=" << i->weight << " " << "money=" << i->money <<" "<< "knowledge=" << i->knowledge << " " << "charm=" << i->charm << std::endl;
        cout << "floor-room:";
        if(i->floor < 10) cout << "0";
            cout << i->floor << "-";
        if(i->room < 10) cout << "0";
            cout << i->room << std::endl;
        cout << std::endl;
    }
}
/****************************查询界面的通用函数*****************************/


/******************************weight********************************/
bool com_weight(const struct student& a, const struct student& b)
{
    return a.weight > b.weight || (a.weight == b.weight && a.name > b.name);
}

void get_weight(vector<struct student> students, const string& sort_judge, const string& sex_judge, const bool& is_rang, const double& rang_x, const double& rang_y)
{
    vector<struct student> temp_stu, result_stu;
    
    std::sort(students.begin(), students.end(), com_weight);
    
    if(sort_judge == "max")
        std::copy(students.begin(), students.end(), back_inserter(temp_stu));
    else if(sort_judge == "min")
        std::copy(students.rbegin(), students.rend(), back_inserter(temp_stu));
    
    if(is_rang)
        get_weight_rang(temp_stu, sex_judge, result_stu, rang_x, rang_y);
    else
        get_the_weight(temp_stu, sex_judge, result_stu);
    
    print_message(result_stu);
}

void get_the_weight(const vector<struct student>& temp_stu, const string& sex_judge, vector<struct student>& result_stu)
{
    bool have_sex = true;
    if(sex_judge == "")
        have_sex = false;
    
    double the_weight;
    bool first = true;
    for(vector<struct student>::const_iterator i = temp_stu.begin(); i != temp_stu.end(); i++)
    {
        if(first)
        {
            if(!have_sex || i->sex == sex_judge)
            {
                first = false;
                the_weight = i->weight;
                result_stu.push_back(*i);
            }
            else
                continue;
        }
        else
        {
            if(i->weight == the_weight && (!have_sex || i->sex == sex_judge))
                result_stu.push_back(*i);
            else if(i->weight < the_weight)
                break;
        }
    }
}

void get_weight_rang(const vector<struct student>& temp_stu, const string& sex_judge, vector<struct student>& result_stu, const double& rang_x, const double& rang_y)
{
    bool have_sex = true;
    if(sex_judge == "")
        have_sex = false;
    
    for(vector<struct student>::const_iterator i = temp_stu.begin(); i != temp_stu.end(); i++)
    {
        if(!have_sex || i->sex == sex_judge)
        {
            if(i->weight >= rang_x && i->weight <= rang_y)
                result_stu.push_back(*i);
        }
    }
}
/******************************weight********************************/

/******************************money*********************************/
bool com_money(const struct student& a, const struct student& b)
{
    return a.money > b.money || (a.money == b.money && a.name > b.name);
}

void get_money(vector<struct student> students, const string& sort_judge, const string& sex_judge, const bool& is_rang, const double& rang_x, const double& rang_y)
{
    vector<struct student> temp_stu, result_stu;
    
    std::sort(students.begin(), students.end(), com_money);
    
    if(sort_judge == "max")
        std::copy(students.begin(), students.end(), back_inserter(temp_stu));
    else if(sort_judge == "min")
        std::copy(students.rbegin(), students.rend(), back_inserter(temp_stu));
    
    if(is_rang)
        get_money_rang(temp_stu, sex_judge, result_stu, rang_x, rang_y);
    else
        get_the_money(temp_stu, sex_judge, result_stu);
    
    print_message(result_stu);
}

void get_the_money(const vector<struct student>& temp_stu, const string& sex_judge, vector<struct student>& result_stu)
{
    bool have_sex = true;
    if(sex_judge == "")
        have_sex = false;
    
    double the_money;
    bool first = true;
    for(vector<struct student>::const_iterator i = temp_stu.begin(); i != temp_stu.end(); i++)
    {
        if(first)
        {
            if(!have_sex || i->sex == sex_judge)
            {
                first = false;
                the_money = i->money;
                result_stu.push_back(*i);
            }
            else
                continue;
        }
        else
        {
            if(i->money == the_money && (!have_sex || i->sex == sex_judge))
                result_stu.push_back(*i);
            else if(i->money < the_money)
                break;
        }
    }
}

void get_money_rang(const vector<struct student>& temp_stu, const string& sex_judge, vector<struct student>& result_stu, const double& rang_x, const double& rang_y)
{
    bool have_sex = true;
    if(sex_judge == "")
        have_sex = false;
    
    for(vector<struct student>::const_iterator i = temp_stu.begin(); i != temp_stu.end(); i++)
    {
        if(!have_sex || i->sex == sex_judge)
        {
            if(i->money >= rang_x && i->money <= rang_y)
                result_stu.push_back(*i);
        }
    }
}
/******************************money*********************************/

/******************************knowledge******************************/
bool com_knowledge(const struct student& a, const struct student& b)
{
    return a.knowledge > b.knowledge || (a.knowledge == b.knowledge && a.name > b.name);
}

void get_knowledge(vector<struct student> students, const string& sort_judge, const string& sex_judge, const bool& is_rang, const double& rang_x, const double& rang_y)
{
    vector<struct student> temp_stu, result_stu;
    
    std::sort(students.begin(), students.end(), com_knowledge);
    
    if(sort_judge == "max")
        std::copy(students.begin(), students.end(), back_inserter(temp_stu));
    else if(sort_judge == "min")
        std::copy(students.rbegin(), students.rend(), back_inserter(temp_stu));
    
    if(is_rang)
        get_knowledge_rang(temp_stu, sex_judge, result_stu, rang_x, rang_y);
    else
        get_the_knowledge(temp_stu, sex_judge, result_stu);
    
    print_message(result_stu);
}

void get_the_knowledge(const vector<struct student>& temp_stu, const string& sex_judge, vector<struct student>& result_stu)
{
    bool have_sex = true;
    if(sex_judge == "")
        have_sex = false;
    
    double the_knowledge;
    bool first = true;
    for(vector<struct student>::const_iterator i = temp_stu.begin(); i != temp_stu.end(); i++)
    {
        if(first)
        {
            if(!have_sex || i->sex == sex_judge)
            {
                first = false;
                the_knowledge = i->knowledge;
                result_stu.push_back(*i);
            }
            else
                continue;
        }
        else
        {
            if(i->knowledge == the_knowledge && (!have_sex || i->sex == sex_judge))
                result_stu.push_back(*i);
            else if(i->knowledge < the_knowledge)
                break;
        }
    }
}

void get_knowledge_rang(const vector<struct student>& temp_stu, const string& sex_judge, vector<struct student>& result_stu, const double& rang_x, const double& rang_y)
{
    bool have_sex = true;
    if(sex_judge == "")
        have_sex = false;
    
    for(vector<struct student>::const_iterator i = temp_stu.begin(); i != temp_stu.end(); i++)
    {
        if(!have_sex || i->sex == sex_judge)
        {
            if(i->knowledge >= rang_x && i->knowledge <= rang_y)
                result_stu.push_back(*i);
        }
    }
}
/******************************knowledge******************************/

/******************************charm**********************************/
bool com_charm(const struct student& a, const struct student& b)
{
    return a.charm > b.charm || (a.charm == b.charm && a.name > b.name);
}

void get_charm(vector<struct student> students, const string& sort_judge, const string& sex_judge, const bool& is_rang, const double& rang_x, const double& rang_y)
{
    vector<struct student> temp_stu, result_stu;
    
    std::sort(students.begin(), students.end(), com_charm);
    
    if(sort_judge == "max")
        std::copy(students.begin(), students.end(), back_inserter(temp_stu));
    else if(sort_judge == "min")
        std::copy(students.rbegin(), students.rend(), back_inserter(temp_stu));
    
    if(is_rang)
        get_charm_rang(temp_stu, sex_judge, result_stu, rang_x, rang_y);
    else
        get_the_charm(temp_stu, sex_judge, result_stu);
    
    print_message(result_stu);
}

void get_the_charm(const vector<struct student>& temp_stu, const string& sex_judge, vector<struct student>& result_stu)
{
    bool have_sex = true;
    if(sex_judge == "")
        have_sex = false;
    
    double the_charm;
    bool first = true;
    for(vector<struct student>::const_iterator i = temp_stu.begin(); i != temp_stu.end(); i++)
    {
        if(first)
        {
            if(!have_sex || i->sex == sex_judge)
            {
                first = false;
                the_charm = i->charm;
                result_stu.push_back(*i);
            }
            else
                continue;
        }
        else
        {
            if(i->charm == the_charm && (!have_sex || i->sex == sex_judge))
                result_stu.push_back(*i);
            else if(i->charm < the_charm)
                break;
        }
    }
}

void get_charm_rang(const vector<struct student>& temp_stu, const string& sex_judge, vector<struct student>& result_stu, const double& rang_x, const double& rang_y)
{
    bool have_sex = true;
    if(sex_judge == "")
        have_sex = false;
    
    for(vector<struct student>::const_iterator i = temp_stu.begin(); i != temp_stu.end(); i++)
    {
        if(!have_sex || i->sex == sex_judge)
        {
            if(i->charm >= rang_x && i->charm <= rang_y)
                result_stu.push_back(*i);
        }
    }
    
    
}
/******************************charm**********************************/

/******************************查询学生的房间****************************/
void get_stu_room(int& floor, int& room, const vector<struct student>& students)
{
    vector<struct student> result_stu;
    
    for(vector<struct student>::const_iterator i = students.begin(); i != students.end(); i++)
    {
        if(i->floor == floor && i->room == room)
            result_stu.push_back(*i);
    }
    
    print_message(result_stu);
}
/******************************查询学生的房间****************************/


/****************************查询恋爱历史******************************/
void get_lover_maxlovers(vector<struct student> students)
{
    struct student stu;
    queue<struct student>::size_type max_num = 0;
    vector<string> lovers;
    
    
    for(vector<struct student>::iterator i = students.begin(); i != students.end(); i++)
    {
        if(i->love_queue.size() > max_num)
        {
            int cnt = 0;
            while(!(i->love_queue.empty()))
            {
                if(!(std::find(lovers.begin(), lovers.end(), i->love_queue.front().name) != lovers.end()))
                {
                    cnt++;
                    lovers.push_back(i->love_queue.front().name);
                }
                
                i->love_queue.pop();
            }
            if(cnt > max_num)
            {
                max_num = cnt;
                stu = *i;
            }
        }
    }
    
    if(max_num == 0)
        cout << std::endl << "没有人谈过恋爱。" << std::endl;
    else
        cout << std::endl << stu.name << " 共和 " << max_num << " 个不同的人谈过恋爱。" << std::endl << std::endl;
    
}

void get_lover_maxcharm(vector<struct student> students)
{
    struct student stu;
    double max_charm = 0;
    
    for(vector<struct student>::const_iterator i = students.begin(); i != students.end(); i++)
    {
        if(!(i->love_queue.empty()) && i->charm > max_charm)
        {
            stu = *i;
            max_charm = i->charm;
        }
    }
    
    if(max_charm == 0)
        cout << std::endl << "没有人谈过恋爱。" << std::endl << std::endl;
    else
        cout << std::endl << stu.name << " 的魅力值最大，为 " << max_charm << std::endl << std::endl;
}

bool com_name(const struct student& a, const struct student& b)
{
    return a.name > b.name;
}

void get_lover(vector<struct student> students, const string& com_str, const int& stu_id)
{
    if(is("charm", com_str))
    {
        cout << std::endl << "以魅力排序输出 " << students[stu_id].name << " 的恋爱历史：" << std::endl;
        
        vector<struct student> lovers;
        while(!(students[stu_id].love_queue.empty()))
        {
            lovers.push_back(students[stu_id].love_queue.front());
            students[stu_id].love_queue.pop();
        }
        
        sort(lovers.begin(), lovers.end(), com_charm);
        int cnt = 0;
        for(vector<struct student>::const_iterator i = lovers.begin(); i != lovers.end(); i++)
        {
            cnt++;
            if(cnt % 10 == 0)
            {
                cout << "[输入任何符号将继续输出]";
                string temp;
                getline(cin, temp);
            }
            
            cout << std::endl << i->name << std::endl;
        }
        cout << std::endl;
    }
    else if(is("name", com_str))
    {
        cout << std::endl << "以姓名排序输出 " << students[stu_id].name << " 的恋爱历史：" << std::endl;
        
        vector<struct student> lovers;
        while(!(students[stu_id].love_queue.empty()))
        {
            lovers.push_back(students[stu_id].love_queue.front());
            students[stu_id].love_queue.pop();
        }
        
        sort(lovers.begin(), lovers.end(), com_name);
        
        int cnt = 0;
        for(vector<struct student>::const_iterator i = lovers.begin(); i != lovers.end(); i++)
        {
            cnt++;
            if(cnt % 10 == 0)
            {
                cout << "[输入任何符号将继续输出]";
                string temp;
                getline(cin, temp);
            }
            
            cout << i->name << std::endl;
        }
        cout << std::endl;
    }
    else //if(is("time", com_str))
    {
        cout << std::endl << "以时间先后输出 " << students[stu_id].name << " 的恋爱历史：" << std::endl;
        
        int cnt;
        while(!(students[stu_id].love_queue.empty()))
        {
            cnt++;
            if(cnt % 10 == 0)
            {
                cout << "[输入任何符号将继续输出]";
                string temp;
                getline(cin, temp);
            }
            
            cout << students[stu_id].love_queue.front().name << std::endl;
            students[stu_id].love_queue.pop();
        }
        cout << std::endl;
    }
}
/****************************查询恋爱历史******************************/
/*******************************query_interface*******************************/