//
// timer.cpp
// ~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <vector>
#include <iostream>
#include <boost/function.hpp>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>


void add(int *param)
{
  (*param) +=  1;
}

int main(int argc, char ** argv)
{
  if(argc < 2)
  {
    std::cout << "input <times> <seconds>" << std::endl;
    return 0 ;
  }
  
  int times = atoi(argv[1]);
  int seconds = atoi(argv[2]);

  std::cout << "times : " << times << std::endl;
  std::cout << "seconds : " << seconds << std::endl;


  boost::asio::io_service io;

  
  std::vector<boost::shared_ptr<boost::asio::deadline_timer>> timers;
  int count = 0;
  for(int i = 0 ; i < times ; i++)
  {
    boost::shared_ptr<boost::asio::deadline_timer> t(new boost::asio::deadline_timer(io, boost::posix_time::seconds(seconds)));
    t->async_wait(boost::bind(add, &count));
    timers.push_back(std::move(t));
  }


  io.run();


  std::cout << "Final count is " << count << "\n";


  return 0;
}