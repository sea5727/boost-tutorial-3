#include <iostream>
#include <deque>
#include <boost/function.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/thread.hpp>

void * test_thread(void * pvnull)
{
    std::string *arg = (std::string*)pvnull;
    std::cout << *arg << std::endl;
    std::cout << "this is thread thread" << std::endl;
    return (void *)NULL;
}
int main(int argc, char** argv) {

    boost::thread::attributes attr;
    auto stack_size = attr.get_stack_size();
    std::cout << "staci_size : " << stack_size << std::endl;

    attr.set_stack_size(80 * 1024);

    auto stack_size2 = attr.get_stack_size();
    std::cout << "staci_size2 : " << stack_size2 << std::endl;

    std::string * str = new std::string("this is arguments");

    boost::thread th(attr, boost::bind(test_thread, (void *)str));
    th.join();

    return 0;

}
