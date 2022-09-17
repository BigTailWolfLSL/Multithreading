#include <iostream>
#include <shared_mutex>
#include <mutex>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <future>

using namespace std;

int main(){
    mutex amutex;
    amutex.lock();
    cout<<"try_lock:"<<amutex.try_lock()<<endl;

    lock_guard<mutex> aguard1(amutex,adopt_lock);//第二参数可缺省，目前表示对应mutex以上锁。
    cout<<"lock_guard own lock?:no such function"<<endl;

    unique_lock<mutex> aguard2(amutex,defer_lock);//ado..已经上锁但是标记own，defer不own。
    cout<<"unique_lock own lock?:"<<aguard2.owns_lock()<<endl;//unique_lock比lockguard灵活，略大
    cout<<"unique_lock try_lock:"<<aguard2.try_lock()<<endl;//因为有owns_lock，为true时try_lock会报错

    shared_mutex smutex;
    shared_lock<shared_mutex> aguard3(smutex);//shared_lock和shared_mutex需要c++17
    cout<<"shared_lock own lock?:"<<aguard3.owns_lock()<<endl;
    aguard3.unlock();//已有内核，需将cmakelist中的standard改为17
    cout<<"shared_lock try_lock:"<<aguard3.try_lock()<<endl;//同unique_lock,try_lock会在own时报错
//shared_mutex读写的互斥量 里有两种lock，shared和不shared的。直接控制可以同时控制两种锁通过基础mutex的三种函数
//控制写锁（独占锁），函数名后面加_shared控制读锁（共享锁）
//shared_lock工具只控制shared_mutex的读锁(用法同unique_lock)，lockguard和unique_lock只控制它的写锁。
    return 0;
}