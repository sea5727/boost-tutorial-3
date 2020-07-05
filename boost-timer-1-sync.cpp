#include <iostream>
#include <boost/asio.hpp>

int main(int argc, char** argv) {
    std::cout << "hello boost-timer-1" << std::endl;

    boost::asio::io_service io;
    boost::asio::deadline_timer t(io, boost::posix_time::seconds(5));


    t.wait();
    std::cout << "bye.." << std::endl;
    return 0;

}
