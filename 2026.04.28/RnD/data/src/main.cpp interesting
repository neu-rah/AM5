#include <iostream>
using namespace std;

struct Nil {};

template<typename Cfg=Nil>
struct API:Cfg {
  static constexpr void print() {}
};

template<typename O,typename T>
struct App:O::Part<T> {};

template<typename R>
struct Link {
  template<typename O>
  struct Part:R {
    static void print() {
      R::print();
      O::print();
    }
  };
};

struct A:Link<A> {
  static void print() {cout<<"A";}
};

struct B:Link<B> {
  static void print() {cout<<"B";}
};

int main() {
  A::Part<B::Part<API<Nil>>>::print();
  cout<<endl;
  return 0;
}