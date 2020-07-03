#include "boost/bind/bind.hpp"  

int fn(int a, int b, int c, int d, int e)  
{  
    printf("a = %d, b = %d, c = %d, d = %d, e = %d\n", a, b, c, d , e);  
    return a + b + c + d + e;  
}  
  
int main()  
{  
    printf("%d\n", boost::bind(fn, 1, 2, 3, 4, 5)());  

    
    printf("%d\n", boost::bind(fn, 1, 2, boost::placeholders::_1, 4, 5)(10));  

    printf("%d\n", boost::bind(fn, 1, boost::placeholders::_2, boost::placeholders::_1, 4, 5)(10, 11));  

    return 0;  
}  
