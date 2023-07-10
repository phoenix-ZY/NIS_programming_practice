#include <iostream>
#include <cstring>
#include "sched_manage.h"
#include <mutex>
#include <thread>
#include <chrono>
#include <termios.h>
#include <unistd.h>
using namespace std;

mutex mtx;
GUEST guest;
bool stop = false;
void usage() //show help
{
    printf("帮助信息：\n");
    printf("命令行参数格式:\n");
    printf("  #cmd run     运行程序\n");
    printf("  #cmd -h      显示帮助\n");
    printf("1.addtask:添加任务\n");
    printf("2.showtask:显示任务\n");
    printf("3.deltask:删除任务\n");
    printf("4.changetask:修改任务\n");
    printf("5.quit:退出程序\n");
}

void inputthread()
{
    while(1){
        printf("请输入命令(：输入命令名称或者数字)\n");
        printf("1.addtask:添加任务  ");
        printf("2.showtask:显示任务  ");
        printf("3.deletetask:显示任务  ");
        printf("4.changetask:修改任务  ");
        printf("5.quit:退出程序\n");
        string command;
        cin >> command;
        if(command == "addtask" || command == "1"){
            printf("请输入任务名:");
            string name;
            cin >> name;
            cout << endl;
            printf("请输入任务优先级(1 高, 2 中, 3 低):");
            int prio;
            cin >> prio;
            cout << endl;
            printf("请输入任务类别(1 学习, 2 娱乐, 3 生活):");
            int type;
            cin >> type;
            cout << endl;
            printf("请输入任务开始时间(格式为 YYYY-MM-DD HH:MM:SS (MM SS 若不输入默认为0)) \n");
            string start;
            getchar();
            getline(cin, start);
            cout << endl; 
            printf("请输入任务提醒时间(格式为 YYYY-MM-DD HH:MM:SS (MM SS 若不输入默认为0)) \n");
            string remi;
            getline(cin, remi);
            cout << endl;
            mtx.lock();    
            guest.addTask(name, prio, type, start, remi);
            mtx.unlock();
            printf("任务创建成功\n");
        }
        else if(command == "showtask" || command == "2"){
            string command0;
            printf("你想按照什么顺序输出？\n");
            printf("1:按提醒时间 ");
            printf("2.按优先级  ");
            printf("3.按开始时间  ");
            printf("4.按id  ");
            printf("5.按分类\n");
            cin >> command0;
            cout << "id\tname\tpriority\ttype\tstarttime\t\tremindtime" << endl;
            if (command0 == "1" )
            {
            mtx.lock();    
            guest.printTask_by_reminder_time();
            mtx.unlock();
            }

            if (command0 == "2" )
            {
            mtx.lock();    
            guest.printTask_by_priority();
            mtx.unlock();
            }

            if (command0 == "3" )
            {
            mtx.lock();    
            guest.printTask_by_start_time();
            mtx.unlock();
            }

            if (command0 == "4" )
            {
            mtx.lock();    
            guest.printTask_by_id();
            mtx.unlock();
            }

            if (command0 == "5" )
            {
            mtx.lock();    
            guest.printTask_by_type();
            mtx.unlock();
            }
        }
        else if(command == "deltask" || command == "3"){
            printf("当前所有任务显示如下：\n");
            mtx.lock();
            bool flag = guest.printTask_by_reminder_time();
            mtx.unlock();
            if (flag == 0) continue;
            printf("请输入要删除的任务id:");
            int task;
            cin >> task;
            mtx.lock();
            guest.deleteTask(task);
            mtx.unlock();
            cout << endl;
    
        }
        else if(command == "changetask" || command == "4"){
            printf("当前所有任务显示如下：\n");
            mtx.lock();
            bool flag = guest.printTask_by_reminder_time();
            mtx.unlock();
            if (flag == 0) continue;
            printf("请输入要修改的任务id:");
            int task;
            bool flag_0 = 0;
            cin >> task;
            mtx.lock();
            flag_0 = guest.deleteTask(task);
            mtx.unlock();
            if (flag_0 == 1){
		    printf("请输入修改后的任务信息:\n");
		    printf("请输入任务名:");
		    string name;
		    cin >> name;
		    cout << endl;
		    printf("请输入任务优先级(1 高, 2 中, 3 低):");
		    int prio;
		    cin >> prio;
		    cout << endl;
		    printf("请输入任务类别(1 学习, 2 娱乐, 3 生活):");
		    int type;
		    cin >> type;
		    cout << endl;
		    printf("请输入任务开始时间(格式为 YYYY-MM-DD HH:MM:SS (MM SS 若不输入默认为0)) \n");
		    string start;
		    getchar();
		    getline(cin, start);
		    cout << endl; 
		    printf("请输入任务提醒时间(格式为 YYYY-MM-DD HH:MM:SS (MM SS 若不输入默认为0)) \n");
		    string remi;
		    getline(cin, remi);
		    cout << endl;
		    mtx.lock();    
		    guest.addTask(name, prio, type, start, remi);
		    mtx.unlock();
		    printf("任务修改成功\n");
            }
    
        }
        else if(command == "quit" || command == "5"){
            printf("程序退出成功\n");
            stop = true;
            break;
        }
        else{
            printf("错误指令,请输入正确指令。例如: \n");
            printf("1.addtask:添加任务  ");
            printf("2.showtask:显示任务  ");
            printf("3.deltask:删除任务  ");
            printf("4.changetask:修改任务  ");
            printf("5.quit:退出程序\n");
        }
    }
}

void checkthread()
{
    while(!stop) {
       this_thread::sleep_for(chrono::seconds(2));
       mtx.lock();
       bool flag = guest.checktask();
       mtx.unlock();
       if (flag == 1)
       {
           printf("你可以继续上面的操作：\n");
       }
       if (stop) break;
    }
}



int main(int argc, char* argv[]){
    if (argc < 2) {
        cout << "输入参数错误" << endl;
        cout << "命令行参数格式:" << endl;
        printf("  #cmd run     运行程序\n");
        printf("  #cmd -h      显示帮助\n");
        return 1;
    }

    if (strcmp(argv[1], "-h") == 0) {
        usage();
        return 0;
    }

    if (strcmp(argv[1], "run") != 0) {
        cout << "输入参数错误" << endl;
        cout << "命令行参数格式:" << endl;
        printf("  #cmd run     运行程序\n");
        printf("  #cmd -h      显示帮助\n");
        return 1;
    }

    //登录
    string user_name;
    string psw;
    string psw_1;
    USERS user;


    
    while(1){
        cout << "请输入用户名" << endl;
        cin >> user_name;
        if(user_name.length() == 0) {cout << "用户名不能为空，请重试"<<endl; continue;}
    
        if(user.find_user(user_name)){
            struct termios old, new_;
            tcgetattr(STDIN_FILENO, &old);   
            new_ = old;
            new_.c_lflag &= ~ECHO;            
            tcsetattr(STDIN_FILENO, TCSANOW, &new_);
            psw = getpass("请输入密码: ");
            tcsetattr(STDIN_FILENO, TCSANOW, &old);
            int check = user.check_psw(user_name, psw);
            if(check == -1){
                cout << "密码错误，请重试" <<endl;
                continue;
            }
            else if(check != -2 && check != -3){ //登录成功
                guest = user.read_task(check);
                cout << "登录成功" << endl;
                break;
            }
        }
        else{
            struct termios old, new_;
            cout << "用户不存在，将为您创建一个新用户，输入1继续，如果您输错了用户名，请输入r返回:" <<endl;
            string temp;
            cin >> temp;
            if(temp == "r") continue;
            tcgetattr(STDIN_FILENO, &old);   
            new_ = old;
            new_.c_lflag &= ~ECHO;            
            tcsetattr(STDIN_FILENO, TCSANOW, &new_);
            psw = getpass("请设置密码: ");
            if(psw.length() == 0) {cout << "密码不能为空，请重试"<<endl; continue;}
            tcsetattr(STDIN_FILENO, TCSANOW, &old);
            
            tcgetattr(STDIN_FILENO, &old);   
            new_ = old;
            new_.c_lflag &= ~ECHO;            
            tcsetattr(STDIN_FILENO, TCSANOW, &new_);
            psw_1 = getpass("请再次输入密码: ");
            tcsetattr(STDIN_FILENO, TCSANOW, &old);
            if (psw == psw_1)
            {
            	user.create_user(user_name, psw);
            	GUEST guest(user_name);
            	break;
            }
            else cout << "两次密码不相同,请重试" <<endl;
            
        }

    }
    thread t1(inputthread);
    thread t2(checkthread);
    
    t1.join();
    t2.join();
    


    return 0;
}
