#include<cstdlib>
#include<cstring>
#include<iostream>
#include<boost/asio.hpp>
#include<boost/bind.hpp>
 
using boost::asio::ip::tcp;
using namespace std;
 
enum { max_length = 1024 };
 
class client
{
public:
  client(boost::asio::io_service& io_context,
  //client(boost::asio::io_context& io_context,
      const string& host, const string& port) : socket_(io_context)
  {
    boost::asio::ip::tcp::resolver resolver(io_context);
    boost::asio::ip::tcp::resolver::query query(host, port);
    boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
 
    //비동기(Unblock) 상태로 서버와 접속한다.
    boost::asio::async_connect(socket_, endpoint_iterator,
        boost::bind(&client::handle_connect, this, boost::asio::placeholders::error));
  }
 
  void handle_connect(const boost::system::error_code& e){
    if(!e){
      cout << "Connected!" << endl;
      string msg = "Hello! Server!";
      //Server로부터 비동기 write를 시도한다.
      boost::asio::async_write(socket_, boost::asio::buffer(msg, msg.length()),
          boost::bind(&client::handle_write,this,boost::asio::placeholders::error)); 
    }
  }
 
  void handle_write(const boost::system::error_code& e){
    if(!e){
      cout << "Done!" << endl;
    }
  }
 
private:
  tcp::socket socket_;
  char data_[max_length];
};
 
int main(int argc, char* argv[])
{
  try
  {
    // Check command line arguments.
    if (argc != 3)
    {
      std::cerr << "Usage: client <host> <port>" << std::endl;
      return 1;
    }
 
    boost::asio::io_service io_context;
    //boost::asio:io_context io_context;
    client c(io_context, argv[1], argv[2]);
    io_context.run();
  }
  catch (std::exception& e)
  {
    cerr << e.what() << endl;
  }
 
  return 0;
}
