#include <iostream>
#include <condition_variable>
#include <queue>
#include <thread>


std::condition_variable cv;
std::queue<int> q;
std::mutex mtx;
constexpr int done = -1;

void print_ints()
{
    int i = 0;
    while(i != done)
    {
        {
            auto lock = std::unique_lock<std::mutex> { mtx };
            while ( q.empty() )
            {
                std::cout << "cv.wait start" << std::endl;
                cv.wait(lock);
                std::cout << "cv wait end" << std::endl;
            }
                
            
            i = q.front();
            q.pop();
        }
        if( i != done )
        {
            std::cout << "Got : " << i << "\n";
        }
    }
}

auto generate_ints()
{
    for(auto i : {1, 2, 3, done}){
        std::this_thread::sleep_for(std::chrono::seconds(1));
        {
            std::lock_guard<std::mutex> lock(mtx);
            q.push(i);
            q.push(i);
            q.push(i);
            q.push(i);
            q.push(i);
        }
        std::cout << "cv notify_one" << std::endl;
        cv.notify_one();
    }
}
int main(int argc, char** argv) {
    auto producer = std::thread{ generate_ints };
    auto consumer = std::thread{ print_ints };

    producer.join();
    consumer.join();

    return 0;

}
