#include <iostream>
#include <mutex>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <future>
#include <cassert>

using namespace std;

int main(){
    promise<int> p;
    p.set_value(5);
    shared_future<int> f=p.get_future().share();
    cout<<f.get()<<endl;
    cout<<f.get()<<endl;
    shared_future<int> g=f;
    cout<<g.get()<<endl;

    promise<int> p2;
    future<int> f2=p2.get_future();
    if (f2.wait_for(chrono::milliseconds(35))==std::future_status::timeout)cout<<"not yet"<<endl;
    p2.set_value(1);
    if (f2.wait_for(chrono::milliseconds(35))==std::future_status::ready)cout<<"OK"<<endl;
    return 0;
}