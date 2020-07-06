#include <iostream>
#include <boost/function.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
 
using namespace std;
 
class PRINTER
{
public:
    PRINTER(boost::asio::io_service& io)
        : strand_(io)
        , timer1_(io, boost::posix_time::seconds(1))
        , timer2_(io, boost::posix_time::seconds(1))
        , count_(0)
    {
        timer1_.async_wait(strand_.wrap(boost::bind(&PRINTER::print1, this, boost::asio::placeholders::error)));
        timer2_.async_wait(strand_.wrap(boost::bind(&PRINTER::print2, this)));
    }
    ~PRINTER()
    {
        cout << "Final count is " << count_ << endl;
    }


    void print1(const boost::system::error_code & error_code)
    {
        if(error_code)
        {
            switch(error_code.value())
            {
                case boost::system::errc::operation_canceled : 
                {
                    cout << "!! find value : " << error_code.value() << "this error :  " << error_code.message() <<  endl;//Operation canceled
                    break;
                }
                default : 
                {
                    cout << "value : " << error_code.value() << "this error :  " << error_code.message() <<  endl;//Operation canceled
                    break;
                }
            }
            return;
            
        }
        cout << "Timer 1: " << count_ << endl;
        ++count_;
        
        timer1_.expires_at(timer1_.expires_at() + boost::posix_time::seconds(1));
        timer1_.async_wait(strand_.wrap(boost::bind(&PRINTER::print1, this, boost::asio::placeholders::error)));
        
    }
    void print2()
    {
        cout << "Timer 2: " << count_ << endl;
        ++count_;

        timer2_.expires_at(timer2_.expires_at() + boost::posix_time::seconds(1));
        
        timer2_.async_wait(strand_.wrap(boost::bind(&PRINTER::print2, this)));
        
        timer1_.cancel();
        // timer2_.cancel();
    }
private:
    boost::asio::io_service::strand strand_;
    boost::asio::deadline_timer timer1_;
    boost::asio::deadline_timer timer2_;
    int count_;
};
 
int main()
{
    boost::asio::io_service io;
    PRINTER p(io);
    boost::thread t(boost::bind(&boost::asio::io_service::run, &io));
    io.run();
    t.join();
 
    return 0;
}
