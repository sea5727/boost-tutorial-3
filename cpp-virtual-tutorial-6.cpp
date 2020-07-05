#include <iostream>

class Base {

 public:
  Base() { std::cout << "기반 클래스" << std::endl; }

  virtual void what() { std::cout << "기반 클래스의 what()" << std::endl; }
};
class Derived : public Base {

 public:
  Derived() : Base() { std::cout << "파생 클래스" << std::endl; }

  void what() { std::cout << "파생 클래스의 what()" << std::endl; }
};
int main() {
  Base p; // 기반 클래스
  Derived c; // 파생 클래스

  Base* p_c = &c; // 기반 -> 파생을 가르킴
  Base* p_p = &p; // 기반 -> 기반을 가르킴

  std::cout << " == 실제 객체는 Base == " << std::endl;
  p_p->what(); // 기반 클래스의 What

  std::cout << " == 실제 객체는 Derived == " << std::endl;
  p_c->what(); // 파생 클래스의 What

  return 0;
}