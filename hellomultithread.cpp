#include <iostream>
#include <thread>//多线程的头文件

using namespace std;

void hello()//每个新线程都应该有一个初始函数，initial function
{
    cout<<"Hellomultithread\n";
}

int main()
{
    thread t(hello);//线程有可能自顾自地运行，晚于主程序结束
    t.join();//因此加入让主程序等待其结束
    thread t2([](){//利用lambda函数即兴定义thread
        cout<<"t2 ID:"<<this_thread::get_id()<<endl;
    });
    t2.join();
}