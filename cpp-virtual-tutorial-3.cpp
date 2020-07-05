#include <iostream>
#include <string>

class Base {
  std::string s;

 public:
  Base() : s("기반") { std::cout << "기반 클래스" << std::endl; }

  void what() { std::cout << s << std::endl; }
};
class Derived : public Base {
  std::string s;

 public:
  Derived() : s("파생"), Base() { std::cout << "파생 클래스" << std::endl; }

  void what() { std::cout << s << std::endl; }
};
int main() {
  Base p; // 기반
  Derived c; // 기반, 파생

  std::cout << "=== 포인터 버전 ===" << std::endl;
  Derived* p_p = &p; // 자식 pointer 는 부모를 가리키지 못함. 컴파일 오류발생 invalid conversion from ‘Base*’ to ‘Derived*’ 
  p_p->what();

  return 0;
}