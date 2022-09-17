#include <iostream>
#include <shared_mutex>
#include <mutex>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <future>

using namespace std;

int maxx(int a,int b){
    cout<<"thread ID:"<<this_thread::get_id()<<endl;
    this_thread::sleep_for(chrono::seconds(2));
    if (a>b)return a; else return b;
}

int main(){
    //packaged_task先定义好任务和关联的future,然后调用时执行任务并将值传给future
    packaged_task<int(int,int)> task(maxx);//没有多线程就是一个先安排任务再执行传值的功能
    future<int> fut=task.get_future();
    task(2,3);//执行任务
    cout<<fut.get()<<endl;

    packaged_task<int(int,int)> task2(maxx);//先打包好任务，关联好future
    future<int> fut2=task2.get_future();//另一个线程执行并传值回来
    thread t2(move(task2),1,2);//移动构造，原因1：task2属于此线程;原因2：packaged_task没有复制构造
    t2.join();//不同promise，join可以，因为future已被构造；而且主线程为接收方
    cout<<fut2.get()<<endl;//如果detach，这里就会阻塞咯
    return 0;
}