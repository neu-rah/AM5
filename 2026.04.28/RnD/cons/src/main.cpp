#include <menu.h>
#include <iostream>
using namespace std;

// template<typename T,typename B>
// struct A {
//   template<typename O>
//   struct Part:T::Part<O> {
//     using Base=typename T::Part<O>;
//     B body;
//     template<typename... OO>
//     Part(T&& t,OO&&... oo):Base{t},body{std::forward<OO>(oo)...}{}
//     template<typename... OO>
//     Part(OO&&... oo):body{std::forward<OO>(oo)...}{}
//   };
// };

struct T {template<typename O> using Part=O;};

// MenuDef<T,StaticBody<ItemDef<Data<int>>>> x{1};

template<typename T,typename... OO>
auto menuDef(OO&&... oo) {return MenuDef<T,StaticBody<OO...>>(std::forward<OO>(oo)...);}
template<typename T,typename... OO>
auto menuDef(T&& t,OO&&... oo) {return MenuDef<T,StaticBody<OO...>>(std::forward<T>(t),std::forward<OO>(oo)...);}

int main() {
  // cout<<x.body().m_item.get()<<endl;
  auto menu{menuDef<Text,ItemDef<Data<int>>>(ItemDef<Text>{"main"},{1967})};
  cout<<menu.size()<<endl;
}

M<T,B>::<I> -> 