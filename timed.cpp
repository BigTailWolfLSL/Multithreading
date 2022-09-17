#include <iostream>
#include <mutex>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <future>
#include <cassert>

using namespace std;

int main(){
    chrono::system_clock::time_point timenow=chrono::system_clock::now();//现在时间
    chrono::milliseconds ms(54321);//54.321s
    chrono::milliseconds ms2=ms*2;
    if (5*chrono::seconds(1)==chrono::minutes(1)-chrono::seconds(55))cout<<"equal"<<endl;
    cout<<ms.count()<<endl;//获得对应int

    auto later_10s=chrono::system_clock::now()+chrono::seconds(10);
    //这就是timepoint类型的格式

    auto start=chrono::high_resolution_clock::now();
    this_thread::sleep_for(chrono::seconds(2));
    auto stop=chrono::high_resolution_clock::now();
    cout<<"it takes "<<(chrono::duration<double,ratio<1,1>>(stop-start)).count()<<" sec."<<endl;

    timed_mutex atmutex;
    atmutex.lock();
    unique_lock<timed_mutex> tguard(atmutex,chrono::seconds(1));//timed的锁除了有普通的功能外还有
    //try_lock_for&try_lock_until，以及工具上锁可以规定时间。过了时间未能上锁不会阻塞。
    cout<<tguard.owns_lock()<<endl;
    atmutex.unlock();

    chrono::steady_clock::now();//稳定的时钟，不被修改
}