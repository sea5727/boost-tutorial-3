#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
 
using namespace std;
class PRINTER
{
public:
    PRINTER(boost::asio::io_service& io)
        : timer_(io, boost::posix_time::seconds(1))
        , count_(0)
    {
        timer_.async_wait(boost::bind(&PRINTER::print, this));
    }
    ~PRINTER()
    {
        cout << "Final count is " << count_ << endl;
    }
    void print()
    {
        if (count_ < 5)
        {
            cout << count_ << endl;
            ++count_;
            timer_.expires_at(timer_.expires_at() + boost::posix_time::seconds(1));
            timer_.async_wait(boost::bind(&PRINTER::print, this));
        }
    }
 
private:
    boost::asio::deadline_timer timer_;
    int count_;
};
 
int main()
{
    boost::asio::io_service io;
    PRINTER p(io);
    io.run();
 
    return 0;
}
