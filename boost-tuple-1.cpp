#include <iostream>
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_comparison.hpp>
#include <set>
#include <cassert>
#include <string>
#include <tuple>

boost::tuple<int, std::string> almost_a_pair(10, "hello");
boost::tuple<int, float, double, int> quad(10, 1.0f, 10.0, 1);

void sample1(){
    const int i = boost::get<0>(almost_a_pair);
    const std::string & str = boost::get<1>(almost_a_pair);

    const double d = boost::get<2>(quad);
}

void sample2(){
    std::set<boost::tuple<int, double, int>> s;

    s.insert(boost::make_tuple(1, 1.0, 2));
    s.insert(boost::make_tuple(2, 10.0, 2));
    s.insert(boost::make_tuple(3, 100.0, 2));

    const auto t = boost::make_tuple(0, -1.0, 2);
    assert( 2 == boost::get<2>(t));
    std::cout << boost::get<2>(t) << std::endl;

}
void sample3(){
    boost::tuple<int, float, double, int> quad(10, 11.0f, 10.0, 1);
    int i;
    float f;
    double d;
    int i2;

    boost::tie(i, f, d, i2) = quad;

    assert (i == 10);
    assert (i2 == 1);
}

template<int I>
struct printer { 
    printer() {std::cout << I ;}
};


int main(int argc, char** argv) {
    std::cout << "hello world!!" << std::endl;
    // std::tuple<printer<0>, printer<1>, printer<2> t;
    // (void) t;
    return 0;

}
