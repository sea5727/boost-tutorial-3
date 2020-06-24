#include <iostream>
#include <deque>
#include <boost/function.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/thread.hpp>

class work_queue{
public:
    typedef boost::function<void()> task_type;
private:
    std::deque<task_type>   tasks_;
    boost::mutex            tasks_mutex_;
    boost::condition_variable cond_;
public:
    void push_task(const task_type & task){
        boost::unique_lock<boost::mutex> lock(tasks_mutex_);
        tasks_.push_back(task);
        lock.unlock();

        cond_.notify_one();
    }

    task_type try_pop_task(){
        task_type ret;
        boost::lock_guard<boost::mutex> lock(tasks_mutex_);
        if(!tasks_.empty()){
            ret = tasks_.front();
            tasks_.pop_front();
        }
        return ret;
    }

    task_type pop_task(){
        boost::unique_lock<boost::mutex> lock(tasks_mutex_);
        while(tasks_.empty()){
            cond_.wait(lock);
        }
        task_type ret = tasks_.front();
        tasks_.pop_front();

        return ret;
    }

    void flush(){
        boost::lock_guard<boost::mutex> lock(tasks_mutex_);
        cond_.notify_all();
    }
};

work_queue g_queue;

void some_task(){
    std::cout << "some task" << std::endl;
}
const std::size_t tests_tasks_count = 10 /*0000*/;

void pusher(int id){
    for(std::size_t i = 0 ; i < tests_tasks_count ; ++ i){
        std::cout << "push_task id:" << id << std::endl;
        g_queue.push_task(&some_task);
    }
}
void popper_sync(int id){
    for(std::size_t i = 0 ; i < tests_tasks_count; ++i){
        work_queue::task_type t = g_queue.pop_task();
        std::cout << "pop_task id:" << id << std::endl;
        t();
    }
}

int main(int argc, char** argv) {
    boost::thread pop_sync1(boost::bind(&popper_sync, 1));
    boost::thread pop_sync2(boost::bind(&popper_sync, 2));
    boost::thread pop_sync3(boost::bind(&popper_sync, 3));

    boost::thread push1(boost::bind(&pusher, 1));
    boost::thread push2(boost::bind(&pusher, 2));
    boost::thread push3(boost::bind(&pusher, 3));

    push1.join();
    push2.join();
    push3.join();
    g_queue.flush();

    pop_sync1.join();
    pop_sync2.join();
    pop_sync3.join();

    assert(!g_queue.try_pop_task());

    g_queue.push_task(&some_task);

    assert(g_queue.try_pop_task());


    return 0;

}
