/**
 * @file item.h
 * @author Rui Azevedo (ruihfazevedo@gmail.com)
 * @brief 
 * @version 5
 * @date 2026-04-17
 * 
 * @copyright Copyright (c) 2026
 * 
*/

#pragma once

#include "menu/sys/base.h"

template<typename Def>
struct ItemAPI:Def {
  static constexpr Sz depth() {return 1;}
  static constexpr bool enabled() {return true;}
  static constexpr void enable(bool=true) {}
  static constexpr bool changed() {return false;}
  static constexpr void sync() {}
  template<typename Out> static constexpr void printMenu(Out&,Ctx&) {}
  template<typename Out> static constexpr void printBody(Out&,Ctx&) {}
  template<typename Out> static constexpr void printItem(Out&,Ctx&) {}
  template<typename Out> static constexpr void print(Out&) {}
  static constexpr void nav(CKE cke,Path) {}
  
  template <typename Out>
  static constexpr bool printMenu(Out& out,Ctx& ctx) {return false;}
  
  // template<typename Out>
  // static constexpr bool printBody(Out& out,Ctx&) {return false;}

  template<typename Out> 
  static constexpr void print(Out& out,Ctx& ctx) {}

  template<bool kbd,typename Nav> 
  static constexpr bool nav(Nav& n,const CKE& cke,const Path p) 
    {return false;}

};

template<typename... OO>
struct ItemDef:APIOf<ItemAPI<Nil>,OO...> {
  using Base=APIOf<ItemAPI<Nil>,OO...>;
  using Base::Base;
  using Base::printMenu;
  using Base::nav;
  template<typename Out> void printMenu(Out& out,Ctx&& ctx) {Base::printMenu(out,ctx);}
  void enter(Path path) {nav({Cmd::Enter},path);}
};

template<typename T>
struct Data {
  template<typename O>
  struct Part:T,O {
    using Base=O;
    using Base::Base;
    using Def=typename T::Def;
    using Type=typename T::Type;
    auto get() {return T::get();}
    template<typename... OO>
    Part(const Def& def,OO&&... oo):T{def},Base{std::forward<OO>(oo)...}{}
    template<typename Out>
    void print(Out& out) {
      Out::fmtStart(Fmt::Data,{});
      out.put(get());
      Out::fmtStop(Fmt::Data,{});
      Base::print(out);
    }
    static constexpr void nav(CKE cke,Path path) {
      switch(cke.cmd) {
        case Cmd::Up: T::up();break;
        case Cmd::Down: T::down();break;
      }
      Base::nav(cke,path);
    }
  };
};

using Text=Data<typename TypeDef<const char*>::Value>;
using Int=Data<typename TypeDef<int>::Value>;
template<int& o> using IntRef=Data<typename TypeDef<int>::template Ref<o>>;
// using Float=Data<TypeDef<double>::Value>;

using ActionFunc=void(*)(int);

template<ActionFunc action>
struct Action {
  template<typename O>
  struct Part:O {
    using Base=O;
    static constexpr void nav(CKE cke,Path path) {
      if(cke.cmd==Cmd::Enter) action(path.sel());
      Base::nav(cke,path);
    }
  };
};




