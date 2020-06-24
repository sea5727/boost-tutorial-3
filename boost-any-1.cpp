#include <boost/any.hpp>
#include <iostream>
#include <exception>
#include <string>
#include <vector>


int main(int argc, char** argv) {
    std::vector<boost::any> some_values;
    some_values.push_back(10);
    const char *c_str = "hello there!";
    some_values.push_back(c_str);
    some_values.push_back(std::string("Wow"));
    std::string &s = boost::any_cast<std::string&>(some_values.back());
    s += "That is great ! \n";

    std::cout << s << std::endl;

    return 0;

}
