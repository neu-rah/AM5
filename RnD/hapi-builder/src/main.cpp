#include <iostream>
using namespace std;

//just FA with HAPI variant (one that can be empty)

// HAPY --------------------------------------------------------------------------
template<typename... OO> struct Chain;

template<> struct Chain<> {
  template<typename P> using Part=P;
  template<typename... XX> using App=Chain<XX...>;
  template<typename... XX> using Ins=Chain<XX...>;
  template<template<typename> class> using Map=Chain<>;
};

template<typename O,typename... OO>
struct Chain<O,OO...> {
  template<typename P> using Part=typename O::template Part<typename Chain<OO...>::template Part<P>>;
  template<typename... XX> using App=Chain<O,OO...,XX...>;
  template<typename... XX> using Ins=Chain<XX...,O,OO...>;
  template<template<typename> class F> using Map=Chain<F<O>,F<OO>...>;
};

template<typename API,typename... OO> using APIOf=typename Chain<OO...>::template Part<API>;

//--------------------------------------------------------------------------

struct Nil{};

template<typename Def=Nil>
struct API {
  static constexpr void path() {}
};

template<typename... OO> using Def=APIOf<API<Nil>,OO...>;

//--------------------------------------------------------------------------
struct A {
  template<typename O>struct Part:O {
    using Base=O;
    static void path() {
      cout<<"A/";
      return Base::path();
    }
  };
};

struct B {
  template<typename O>struct Part:O {
    using Base=O;
    static void path() {
      cout<<"B/";
      return Base::path();
    }
  };
};

struct C {
  template<typename O>struct Part:O {
    using Base=O;
    static void path() {
      cout<<"C/";
      return Base::path();
    }
  };
};

// using T=typename Chain<A,B,C>::template Part<API<>>;
using T=Def<A,B,C>;
constexpr T o{};

int main(){
  o.path();
  cout<<endl;
  return 0;
}