#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <future>
#include <queue>
#include <functional>
#include <vector>
#include <memory>
using namespace std;

typedef function<void()> task;

class task_queue{//安全队列
private:
    queue<task> m_queue;
    mutex m_mutex;
public:
    bool empty(){
        unique_lock<mutex> lock(m_mutex);
        return m_queue.empty();
    }

    int size(){
        unique_lock<mutex> lock(m_mutex);
        return m_queue.size();
    }

    void enqueue(task t){//添加任务到队列
        unique_lock<mutex> lock(m_mutex);
        m_queue.push(t);//IDE bug in latest clion 无视
    }

    bool dequeue(task &t){
        unique_lock<mutex> lock(m_mutex);

        if (m_queue.empty()){ return false;}
        t=m_queue.front();
        m_queue.pop();
        return true;
    }

};

class threadpool{
private:
    bool m_shutdown=true;
    mutex m_condition_mutex;
    condition_variable m_condition_lock;
    task_queue m_queue;
    vector<thread> m_threads;

    class threadworker{
    private:
        int m_id;
        threadpool* m_pool;
    public:
        threadworker(threadpool* pool,const int id):m_pool(pool),m_id(id){}
        void operator()(){
            task m_task;
            bool dequeued;
            while (!(m_pool->m_shutdown)){
                {
                    unique_lock<mutex> lock(m_pool->m_condition_mutex);
                    if (m_pool->m_queue.empty()){
                        m_pool->m_condition_lock.wait(lock);
                    }
                    dequeued=m_pool->m_queue.dequeue(m_task);
                }
                if (dequeued){m_task();}
            }
        }
    };
public:
    threadpool(const int n_threads=thread::hardware_concurrency()):m_threads(vector<thread>(n_threads)),m_shutdown(false){}//无视bug
    threadpool(const threadpool &&)= delete;
    threadpool(threadpool &&)= delete;
    threadpool &operator=(const threadpool &)= delete;
    threadpool &operator=(threadpool &&)=delete;

    void init(){
        for (int i=0; i<m_threads.size();++i) m_threads[i]=thread(threadworker(this,i));
    }
    void shotdown(){
        m_shutdown=true;
        m_condition_lock.notify_all();

        for (int i=0;i<m_threads.size();++i)if (m_threads[i].joinable())m_threads[i].join();
    }

    template <class F,class... ARGS>//forward<>+模板+右值引用=完美转发；这里...省略各种包
    auto submit(F&& f,ARGS&& ...args)->future<decltype(f(args...))>{//decltype返回函数返回值的类型
        function<decltype(f(args...))()> func=bind(forward<F>(f),forward<ARGS>(args)...);//bind捆绑函数和形参
        auto task_ptr=make_shared<packaged_task<decltype(f(args...))()>>(func);
        task task_func=[task_ptr](){(*task_ptr)();};
        m_queue.enqueue(task_func);
        m_condition_lock.notify_one();
        return task_ptr->get_future();
    }
};

int fibonacci(int n){
    if (n==0|n==1)return 1;
    int a=1,b=1;
    for (int i=2;i<=n;++i){
        a=a+b;
        int t=a;
        a=b;
        b=t;
    }
    return b;
}

void Fibo_runbypool(){
    threadpool pool(2);
    pool.init();

    future<int> arr[15];

    for (int i=0;i<10;++i){
        arr[i]=pool.submit(fibonacci,i);
    }

    this_thread::sleep_for(chrono::seconds(1));

    for (int i=0;i<10;++i){
        cout<<i<<":"<<arr[i].get()<<endl;
    }

    pool.shotdown();
}

int main(){
    Fibo_runbypool();
    return 0;
}