#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/core/noncopyable.hpp>
#include <boost/thread/thread.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/read.hpp>
#include <memory>
#include <iostream>
struct connection_with_data : boost::noncopyable {
    boost::asio::ip::tcp::socket socket;
    std::string data;
    explicit connection_with_data(boost::asio::io_service &ios) : socket(ios){}

    void shutdown(){
        if(!socket.is_open()){
            return;
        }
        boost::system::error_code ignore;
        socket.shutdown(
            boost::asio::ip::tcp::socket::shutdown_both,
            ignore
        );
        socket.close(ignore);
    }
    ~connection_with_data(){
        shutdown();
    }
};

typedef std::unique_ptr<connection_with_data> connection_ptr;


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
        static connection_ptr create_connection(
            const char * addr,
            unsigned short port_num
        ){
            connection_ptr c(new connection_with_data(get_ios()));

            c->socket.connect(boost::asio::ip::tcp::endpoint(
                boost::asio::ip::address_v4::from_string(addr),
                port_num
            ));

            return c;
        }
    };
    
}


template<class T>
struct task_wrapped_with_connection;

template<class Functor>
void async_write_data(connection_ptr && c, const Functor & f){
    boost::asio::ip::tcp::socket & s = c->socket;
    std::string & d = c->data;

    boost::asio::async_write(
        s, 
        boost::asio::buffer(d),
        task_wrapped_with_connection<Functor>(std::move(c), f)
    );
}

template<class Functor>
void async_read_data( connection_ptr && c, const Functor & f, std::size_t at_least_bytes){
    c->data.resize(at_least_bytes);

    boost::asio::ip::tcp::socket & s = c->socket;
    std::string & d = c->data;
    char * p = (d.empty() ? 0 : &d[0]);

    boost::asio::async_read(
        s,
        boost::asio::buffer(p, d.size()),
        task_wrapped_with_connection<Functor>(std::move(c), f)
    );
}

template<class Functor>
void async_read_data_at_least( connection_ptr && c, const Functor & f, std::size_t at_least_bytes, std::size_t at_most){
    std::string & d = c->data;
    d.resize(at_most);
    char * p = (at_most == 0 ? 0 : &d[0] );

    boost::asio::ip::tcp::socket & s = c->socket;

    boost::asio::async_read(
        s, 
        boost::asio::buffer(p, at_most),
        boost::asio::transfer_at_least(at_least_bytes),
        task_wrapped_with_connection<Functor>(std::move(c) , f)
    );
}


template<class T>
struct task_wrapped_with_connection{
private:
    connection_ptr c_;
    T task_unwrapped_;
public:
    explicit task_wrapped_with_connection(connection_ptr && c, const T & f) : 
        c_(std::move(c)) , task_unwrapped_(f) {}
    
    void operator()( const boost::system::error_code & error, std::size_t bytes_count){
        const auto lambda = [this, &error, bytes_count]() {
            this->c_->data.resize(bytes_count);
            this->task_unwrapped_(std::move(this->c_), error);
        };
        const auto task = detail::make_task_wrapped(lambda);
        task();
    }
};


void process_server_response(connection_ptr && soc, const boost::system::error_code & err){
    if(err && err != boost::asio::error::eof){
        std::cerr << "Client error on receive: " << err.message() << '\n';
        assert(false);
    }

    if(soc->data.size() != 2){
        std::cerr << "Wrong bytes count \n";
        assert(false);
    }

    if(soc->data != "OK"){
        std::cerr << "Wrong response : " << soc->data << '\n';
        assert(false);
    }
    soc->shutdown();
    tp_base::tasks_processor::stop();
}

void receive_auth_response(connection_ptr &&soc, const boost::system::error_code & err){
    if(err){
        std::cerr << "error on sending data: " << err.message() << '\n';
        assert(false);
    }

    async_read_data(std::move(soc), &process_server_response, 2);
}

void send_auth(){
    connection_ptr soc = tp_base::tasks_processor::create_connection("127.0.0.1", 12345);
    soc->data = "auth_name";

    async_write_data(
        std::move(soc), &receive_auth_response
    );
}
int main(){

}