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

template<typename Def>
struct OutAPI {
  template<typename O>
  struct Part:O {
    using Base=O;
    template<typename T> static constexpr void put(const T&) {}
    template<Edge edge,Fmt tag> static constexpr void fmt() {}
  };
};

template<typename... OO>
struct OutDef:APIOf<OutAPI<Nil>,OO...> {};

template<typename Dev,Dev& dev>
struct StreamOut {
  template<typename O>
  struct Part:O {
    using Base=O;
    template<typename T> static constexpr void put(const T& o) {dev<<o;}
  };
};

#ifndef __AVR__
  #include <iostream>
  using ConsoleOut=StreamOut<decltype(std::cout),std::cout>;
#endif