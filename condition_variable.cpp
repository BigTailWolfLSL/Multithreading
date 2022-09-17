#include <iostream>
#include <shared_mutex>
#include <mutex>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <future>

using namespace std;

int n=0;
condition_variable cv;
mutex amutex;

void func1(){
    if (n==0){
        unique_lock<mutex> aguard(amutex);
        cv.wait(aguard);
    }
    cout<<n<<endl;
}

void change(){
//    unique_lock<mutex> aguard(amutex);//这里是否注释决定了运行顺序
    cv.notify_all();
    this_thread::sleep_for(chrono::seconds(1));
    n=1;
}

int main(){
    thread t1(func1);
    thread t2(change);
    t1.join();
    t2.join();
    return 0;
}