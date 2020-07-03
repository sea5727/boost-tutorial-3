#include <iostream>
#include <boost/asio.hpp>

void print(const boost::system::error_code & error_code)
{
    std::cout << "print!!" << std::endl;
}

int main(int argc, char** argv) {
    std::cout << "hello boost-timer-2-async" << std::endl;

    boost::asio::io_service io;
    boost::asio::deadline_timer t(io, boost::posix_time::seconds(5));


    t.async_wait(&print);
    
    io.run();

    std::cout << "bye.." << std::endl;
    return 0;

}
