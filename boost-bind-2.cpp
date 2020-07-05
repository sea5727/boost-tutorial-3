#include "stdio.h"  
#include "boost/bind/bind.hpp"  
#include <iostream>
class Man  
{  
public:  
    std::string name_;  
  
    Man(const char *name)  
    {  
        name_ = name;  
        printf("%s ctor\n", name_.c_str());  
    }  
  
    Man(const Man &r)  
    {  
        name_ = r.name_;  
        printf("%s cctor\n", name_.c_str());  
    }  
  
    ~Man()  
    {  
        printf("%s dtor\n", name_.c_str());  
    }  
  
    void Say(const char *msg)  
    {  
        printf("%s: %s\n", name_.c_str(), msg);  
    }  
};  
  
  
int main()  
{  
    Man bob("Bob");  
    std::cout << " ============== bind 1 start .. bind example.. copy oper ===============" << std::endl;
    boost::bind(&Man::Say, bob, "Hello")();  
    std::cout << " ============== bind 2 start .. bind example.. ref oper ===============" << std::endl;
    boost::bind(&Man::Say, boost::ref(bob), "Hello")();  
    std::cout << " ============== bind 3 start ===============" << std::endl;
    boost::bind(&Man::Say, boost::placeholders::_1, "Hello")(bob);  
    



    // bind는 객체가 복사된다. start1에서확인하였다.
    // 그렇다면... 5번 항목은 crash를 발생한다.
    std::cout << " ============== bind 4 start bind & shared_ptr ===============" << std::endl;
    std::shared_ptr<Man> bob2(new Man("Bob"));  
    std::function< void (const char *) > fn1 =boost::bind(&Man::Say, bob2, boost::placeholders::_1);   
    bob2.reset();  
    fn1("Hello");  

    std::cout << " ============== bind 5 start bind & shared_ptr & ref ===============" << std::endl;
    std::shared_ptr<Man> bob3(new Man("Bob"));  
    std::function< void (const char *) > fn2 =boost::bind(&Man::Say, boost::ref(bob3), boost::placeholders::_1);   
    bob3.reset();
    fn2("Hello");  


    return 0;  
}