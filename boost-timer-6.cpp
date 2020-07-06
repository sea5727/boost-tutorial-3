//
// timer.cpp
// ~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <boost/function.hpp>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

void print(const boost::system::error_code& error_code, boost::asio::deadline_timer* t, int* count)
{
  if (*count < 5)
  {
    std::cout << *count << "\n";
    ++(*count);

    t->expires_at(t->expires_at() + boost::posix_time::seconds(1));
    t->async_wait(boost::bind(print, boost::asio::placeholders::error, t, count));
  }
}

void noargfunction()
{

}
void intargf(int a)
{

}
class test
{
public:
    test() 
        : count(0)
        , t(io, boost::posix_time::seconds(1))
        {

        }
    void testnoarg()
    {

    }
    void intarg(int a)
    {

    }
    void timetask(const boost::system::error_code& error_code)
    {
    }
    void testtime()
    {

        (io, boost::posix_time::seconds(1));
        boost::function<void (const boost::system::error_code&)> f2(boost::bind(&test::timetask, this, boost::asio::placeholders::error));
        t.async_wait(f2);
        // t.async_wait(boost::bind(&test::testtime, this, boost::asio::placeholders::error));
        // boost::function<void (int)> f2(boost::bind(&test::intarg, this, boost::placeholders::_1));

        t.async_wait(boost::bind(print, boost::asio::placeholders::error, &t, &count));
        
    }
public:
    int count;
    boost::asio::io_service io;
    boost::asio::deadline_timer t;
};
int main()
{
  boost::asio::io_service io;

  int count = 0;
  boost::asio::deadline_timer t(io, boost::posix_time::seconds(1));
  t.async_wait(boost::bind(print, boost::asio::placeholders::error, &t, &count));

  io.run();


  std::cout << "Final count is " << count << "\n";

  return 0;
}