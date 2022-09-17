#include <iostream>
#include <shared_mutex>
#include <mutex>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <future>

using namespace std;

int maxx(int a,int b){
    if (a>b)return a; else return b;
}

int main(){
    future<int> answer=async(launch::async|launch::deferred,maxx,1,2);//可能是clion版本比较旧,编译器没问题
    cout<<answer.get()<<endl;//上面两种方式都可以的意思
    return 0;
}