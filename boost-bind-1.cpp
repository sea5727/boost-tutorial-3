#include <boost/bind.hpp>

#include <boost/array.hpp>
#include <algorithm>
#include <functional>
#include <string>
#include <cassert>

void sample1(){ //5보다 작은거 찾기
    const boost::array<int, 12> v = {{
        1,2,3,4,5,6,7,100, 99,98,97,96
    }};

    const std::size_t count0 = std::count_if(v.begin(), v.end(), [](int x){
        return 5 < x;
    });

    const std::size_t count1 = std::count_if(v.begin(), v.end(), 
        boost::bind(std::less<int>(), 5, _1));
    
    assert(count0 == count1);
}

void sample2(){//빈문자열 찾기
    const boost::array<std::string, 3> v = {{
        "We" , "are", "the campions!" 
    }};

    const std::size_t count0 = std::count_if(v.begin(), v.end(), [] (const std::string & s) { return s.empty(); });

    const std::size_t count1 = std::count_if(v.begin(), v.end(), boost::bind(&std::string::empty, _1));

    assert(count0 == count1);
}
void sample3(){ // 5보다 작은 문자열 찾기
    const boost::array<std::string, 3> v = {{
        "We " , "are" , " the campions!"
    }};

    const std::size_t count0 = std::count_if(v.begin(), v.end(), 
        [](const std::string &s) { return s.size() < 5;
    });

    const std::size_t count1 = std::count_if(v.begin(), v.end(),
    boost::bind(
        std::less<std::size_t>(),
        boost::bind(&std::string::size, _1),
        5
    ));

    assert(count0 == count1);
}

void sample4(){
    const boost::array<std::string, 3> v = {{
        "We " , "are", "the campions!"
    }};

    std::string s(
        "Expensive copy constructor is called when binding"
    );

    const std::size_t count0 = std::count_if(v.begin(), v.end(), 
        [&s](const std::string &x) {return x < s;}
    );

    const std::size_t count1 = std::count_if(v.begin(), v.end(),
        boost::bind(std::less<std::string>(), _1, s)
    );

    assert(count0 == count1);

}

void sample5(){
    const boost::array<std::string, 3> v = {{
        "We ", "are", "the campions!"
    }};

    std::string s(
        "Expensive copy constructor is NOT called when binding"
    );

    const std::size_t count1 = std::count_if(v.begin(), v.end(),
        boost::bind(std::less<std::string>(), _1, boost::cref(s))
    );
}

void sample6(){
    const auto twice = boost::bind(std::plus<int>(), 1, _1);
    assert(twice(2) == 4);

    const auto minus_from_second = boost::bind(std::minus<int>(), _2, _1);

    assert(minus_from_second(2, 4) == 2);

    const auto sum_second_and_third = boost::bind(
        std::plus<int>(), _2, _3
    );

    assert(sum_second_and_third(10, 20, 30) == 50);
}
int main(int argc, char** argv) {

    return 0;

}
