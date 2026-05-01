#include <menu.h>
#include <menu/IO/streamOut.h>
#include <menu/IO/ansiOut.h>
#include <menu/fmt/textFmt.h>
#include <menu/fmt/ansiFmt.h>
#include <menu/IO/pcKbdIn.h>
#include <menu/IO/linuxKeyIn.h>

#ifdef __AVR__
  #include <streamFlow.h>
  using namespace StreamFlow;
  #define cout Serial
  #include <avr_std.h>
#else
  #include <iostream>
  #include <type_traits>
#endif
using namespace std;

// this is why we do not derive Menu from its title, and made title a data member.

// and no way to transport the data construction to the outer class
  // // template<typename T>
  // // struct Dados {
  // //   using Type=T;
  // //   Type data;
  // //   template<typename O>
  // //   struct Part:O {
  // //     using Type=T;
  // //     using Base=O;
  // //     using Base::Base;
  // //     template<typename... OO> Part(OO&&... oo):Base{std::forward<OO>(oo)...}{}
  // //     // template<typename... OO> Part(OO&&... oo):Base{std::forward<OO>(oo)...}{}
  // //     const Type& get() const {return data;} 
  // //     void set(const Type& o) {data=o;}
  // //     operator Type&() {return data;}
  // //     operator const Type&() const {return data;}
  // //     template<typename Out> void print(Out& out,Ctx& ctx) {
  // //       out.template fmtStart<Fmt::Data>(ctx);
  // //       out.put(data);
  // //       out.template fmtStop<Fmt::Data>(ctx);
  // //       Base::print(out,ctx);
  // //     }
  // //   };
  // // };
// using Texto=Dados<const char*>;

OutDef<ConsoleOut> out;

template<typename T,typename B>
struct M26 {
  template<typename O>
  struct Part:T::template Part<O> {
    using Base=typename T::template Part<O>;
    using This=Part<O>;
    using Body=B;
    Body m_body;
    Part(Base t,Body b):Base{t},m_body{b}{}
  };
};

template<typename T,typename B>
auto m26(T t,B b) {return ItemDef<M26<Text,B>>(t,b);}

static constexpr const CText main_menu{"Main menu"};
static constexpr const CText op1{"option #1"};

//static menu
  // ItemDef<
  //   M26<
  //     StaticText<main_menu>,
  //     StaticBody<
  //       ItemDef<StaticText<op1>>
  //     >
  //   >
// > staticMenu;

auto menu=m26(
  "title",
  StaticBody<
    ItemDef<Text>
  >{"op1"}
);

int main(){
  // menu.printMenu(out,{});
  // cout<<menu.get()<<endl;
  cout<<"end."<<endl;
  return 0;
}