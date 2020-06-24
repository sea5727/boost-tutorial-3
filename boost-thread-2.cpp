#include <boost/thread/thread.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/noncopyable.hpp>
#include <iostream>

// task_wrapped 로 감싼 task(=func_test)를 100가지 등록,
// 특정숫자시 exception 발생

namespace detail { 


    template<class T>
    struct task_wrapped{
    private:
        T task_unwrapped_;
    public:
        explicit task_wrapped(const T& f) : task_unwrapped_(f) {

        }
        void operator()() const {
            try {
                boost::this_thread::interruption_point();
            }
            catch(const boost::thread_interrupted&){}

            try{
                task_unwrapped_();
            }
            catch(const std::exception &e){
                std::cerr << "Exception : " << e.what() << '\n';
            }
            catch(const boost::thread_interrupted & er){
                std::cerr << "Thread interrupted \n";
            }
            catch (...){
                std::cerr << "Unkknown exception \n";
            }
        }
    };
    template<class T>
    task_wrapped<T> make_task_wrapped(const T& task_unwrapped){
        return task_wrapped<T>(task_unwrapped);
    }
}

namespace tp_base {
    class tasks_processor : private boost::noncopyable {
    protected:
        static boost::asio::io_service& get_ios(){
            static boost::asio::io_service ios;
            static boost::asio::io_service::work work(ios);

            return ios;
        }
    public:
        template<class T>
        static void push_task(const T& task_unwrapped){
            get_ios().post(detail::make_task_wrapped(task_unwrapped));
        }

        static void start(){
            get_ios().run();
        }

        static void stop(){
            get_ios().stop();
        }
    };
}

int func_test(){
    static int counter = 0;
    ++ counter;

    boost::this_thread::interruption_point();

    switch(counter){
        case 3:
            throw std::logic_error("Just checking");
        case 10:
            throw boost::thread_interrupted();
        case 30:
            throw 1;
        case 90:
            tp_base::tasks_processor::stop();
    }
    return counter;

}
int main(){
    for(std::size_t i = 0 ; i < 100 ; i++){
        tp_base::tasks_processor::push_task(&func_test);
    }

    assert(func_test() == 1);

    int sum = 0 ; 

    tp_base::tasks_processor::push_task(
        [&sum]() { sum = 2 + 2;}
    );

    assert(sum == 0);

    tp_base::tasks_processor::start();
    assert(func_test() >= 91);

    std::cout << "program end.." << std::endl;
}