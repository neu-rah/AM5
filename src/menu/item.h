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
  template<typename Out> static constexpr void print(Out&) {}
  template<typename Nav> static constexpr void nav(Nav& n,CKE cke,Path) {}
};

template<typename... OO>
struct ItemDef:APIOf<ItemAPI<Nil>,OO...> {
  using Base=APIOf<ItemAPI<Nil>,OO...>;
  using Base::Base;
};

template<typename Def>
struct Data {
  template<typename O>
  struct Part:Def,O {
    using Base=O;
    template<typename... OO>
    Part(const Def& def,OO&&... oo):Def{def},Base{oo...}{}
    template<typename Out>
    void print(Out& out) {
      Out::template fmt<Edge::start,Fmt::Data>();
      out.put(Def::get());
      Out::template fmt<Edge::stop,Fmt::Data>();
    }
    template<typename Nav> 
    static constexpr void nav(Nav& n,CKE cke,Path path) {
      switch(cke.cmd) {
        case Cmd::Up: Def::up();break;
        case Cmd::Up: Def::down();break;
      }
      Base::template nav(n,cke,path);
    }
  };
};

