#include <boost/thread/thread.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/noncopyable.hpp>
#include <boost/asio/deadline_timer.hpp>

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

    template <class Functor>
    struct timer_task{
    private:
        std::unique_ptr<boost::asio::deadline_timer> timer_;
        task_wrapped<Functor> task_;
    public:
        explicit timer_task(
            std::unique_ptr<boost::asio::deadline_timer> timer,
            const Functor& task_unwrapped)
            : timer_(std::move(timer))
            , task_(task_unwrapped)
        {}

        void operator()(const boost::system::error_code & error) const {
            if(!error){
                task_();
            }
            else {
                std::cerr << error << '\n';
            }
        }
    };
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
        template<class Time, class Func>
        static void run_delayed(Time duration_or_time, const Func& f){
            std::unique_ptr<boost::asio::deadline_timer> timer(
                new boost::asio::deadline_timer(
                    get_ios(), duration_or_time
                )
            );
            boost::asio::deadline_timer & timer_ref = *timer;

            timer_ref.async_wait(
                detail::timer_task<Func>(
                    std::move(timer), f
                )
            );

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

struct test_functor{
    int& i_ ;
    explicit test_functor(int &i) : i_(i){}

    void operator()() const {
        std::cout << "test_functor start.." << std::endl;
        i_ = 1;
        tp_base::tasks_processor::stop();
    }
};



// void test_func1();
void test_func1(){
    std::cout << "test_func1 start .." << std::endl;
}

int main(){
    const int seconds_to_wait = 3;
    int i = 0;
    tp_base::tasks_processor::run_delayed(
        boost::posix_time::seconds(seconds_to_wait),
        test_functor(i)
    );

    tp_base::tasks_processor::run_delayed(
        boost::posix_time::from_time_t(time(NULL) + 1),
        &test_func1
    );

    int t1 = static_cast<int>(time(NULL));
    assert(i == 0);

    tp_base::tasks_processor::start();
}