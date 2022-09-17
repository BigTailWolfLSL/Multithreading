#include <iostream>
#include <mutex>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <future>
#include <cassert>

using namespace std;

atomic_int ta=0,tb=0;
void th1(){
    tb.store(2,memory_order_relaxed);
    ta.store(1,memory_order_relaxed);
}
void th2(){
    cout<<"ta:"<<ta.load(memory_order_relaxed)<<endl;
    cout<<"tb:"<<tb.load(memory_order_relaxed)<<endl;
}

int main(){
    atomic_int a=1;
    int b=2;//b得是一个非原子变量
    cout<<a.load()<<endl;
    a.store(3);
    cout<<"result:"<<a.compare_exchange_weak(b,4)<<" a:"<<a<<" b:"<<b<<endl;
    cout<<"result:"<<a.compare_exchange_weak(b,4)<<" a:"<<a<<" b:"<<b<<endl;
    //C_E_W/C_E_S差不多，strong慢一点但稳点。两个参数，（expected，desired）
    //原子变量和expected不同，将expected修改为原子变量的值，所以expected不能是原子变量
    //与expected相同，将原子变量修改为desired
    a.store(3);
    b=2;
    cout<<"result:"<<a.compare_exchange_strong(b,4)<<" a:"<<a<<" b:"<<b<<endl;
    cout<<"result:"<<a.compare_exchange_strong(b,4)<<" a:"<<a<<" b:"<<b<<endl;

    cout<<atomic_load(&a)<<endl;//以函数的方式+原子变量指针调用，两种方式调用是等价的
    if (atomic_is_lock_free(&a)==a.is_lock_free())cout<<a.is_lock_free()<<endl;//该原子变量的操作是否都是无锁的

    thread t2(th2);thread t1(th1);//内存顺序与原子的读写操作相关联的，若是操作用错了语义，会报错的。但是Clion不知道。
    //有可能tb为0
    this_thread::sleep_for(chrono::seconds(1));
    t1.join();
    t2.join();

    a=1;
    atomic_thread_fence(memory_order_release);//写内存栅栏，分开”在下“写语句”在下“，覆盖原子操作，对非原子也有效
    b=2;
    //同理 acq和cons为读内存栅栏，保证”在上“读语句”在上“
    //rel+acq=rel+acq
    //seq_cst所有读写操作从此分开


    return 0;
}