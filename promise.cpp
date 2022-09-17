#include <iostream>
#include <shared_mutex>
#include <mutex>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <future>

using namespace std;

void gett(future<int> n){
    cout<<"thread:"<<this_thread::get_id()<<endl;
    cout<<n.get()<<endl;
}

int main(){
    cout<<"main:"<<this_thread::get_id()<<endl;
    promise<int> p;
    p.set_value(5);
    this_thread::sleep_for(chrono::seconds(1));//不用多线程promise就是一个传递值的作用
    cout<<p.get_future().get()<<endl;

    promise<int> p2;
    thread t(gett,p2.get_future());
    this_thread::sleep_for(chrono::seconds(2));//用了多线程可在线程之间传递值
    t.detach();//join不行，因为join阻塞直到该线程结束，detach只是分离而已
    p2.set_value(5);//实现thread:2等待main的传值

    promise<void> pl;
    future<void> resultl=pl.get_future();
    pl.set_value();//future是可以为void的
    return 0;
}