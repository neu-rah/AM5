/**
 * @file out.h
 * @author Rui Azevedo (neu-rah) (ruihfazevedo@gmail.com)
 * @brief 
 * @version 5
 * @date 2026-04-17
 * 
 * @copyright Copyright (c) 2026
 * 
*/

#pragma once

template<typename Cfg=Nil>
struct OutAPI:Cfg {
  using Config=Cfg;
  template<typename T> static constexpr void put(const T&) {}
  template<Edge edge,Fmt tag> static constexpr void fmt(const Ctx&) {}
  static constexpr void fmtStart(Fmt tag,const Ctx& ctx) {}
  static constexpr void fmtStop(Fmt tag,const Ctx& ctx) {}
};

template<typename... OO>
struct OutDef:APIOf<OutAPI<>,OO...> {};

template<typename Dev,Dev& dev>
struct StreamOut {
  template<typename O>
  struct Part:O {
    using Base=O;
    static constexpr void nl() {endl(dev);}
    template<typename T> static constexpr void put(const T& o) {dev<<o;}
  };
};

#ifndef __AVR__
  #include <iostream>
  using ConsoleOut=StreamOut<decltype(std::cout),std::cout>;
#endif

struct ItemPrinter {
  template<typename O>
  struct Part:O {
    using Base=O;
    template<typename I>
    void printItem(I& item,Ctx& ctx) {
      Base::template fmt<Edge::start,Fmt::Item>(ctx);
      Base::printItem(item,ctx);
      Base::template fmt<Edge::stop,Fmt::Item>(ctx);
    }
  };
};

struct PrintItem {
  template<typename O>
  struct Part:O {
    using Base=O;
    template<typename I>
    void printItem(I& item,Ctx& ctx) {
      item.print(*this);
    }
  };
};