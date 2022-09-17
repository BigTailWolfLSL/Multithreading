#include <iostream>
#include <shared_mutex>
#include <mutex>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <future>
#include <exception>

using namespace std;

int throwerror(){
    throw 2;
    return 1;
}

int throwlogic(){
    throw logic_error("foo");
    return 1;
}

int main(){
    future<int> f=async(throwerror);
    try{
        int ans=f.get();
        cout<<ans<<endl;//因为在另一个线程里面throw，且有返回值，在调用get的时候在此线程里throw
    }
    catch(int){
        cout<<"int type error from async"<<endl;
    }


    try{
        packaged_task<int()> f2(throwerror);//只是打包
        future<int> ans=f2.get_future();
        f2();//明确调用
        cout<<ans.get()<<endl;
    }
    catch(int){
        cout<<"int type error from pachaged_task"<<endl;
    }

    promise<int> p;
    try{
        p.set_value(throwerror());//因为未进promise就已经throw了
    }
    catch(int){
        cout<<"int type error from promise"<<endl;
    }

    promise<int> p2;
    try{
        p2.set_value(throwlogic());//这样的promise的future是没办法有exception
    }
    catch(exception){
        p2.set_exception(current_exception());//为promise的future设置exception
    }
    try{
        future<int> fut2=p2.get_future();
        cout<<fut2.get()<<endl;
    }
    catch(logic_error& o){
        cout<<o.what()<<endl;
    }
    return 0;
}