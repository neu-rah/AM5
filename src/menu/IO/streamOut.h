#pragma once

#include "menu/out.h"
#include <iostream>

template<typename Out,Out& out>
struct StreamOut {
  template<typename O>
  struct RawPart:O {
    static constexpr const Out& device{out};
    static constexpr void nl() {out<<endl;}
    static constexpr void flush() {out.flush();}
    template<typename T> static constexpr void put(const T& o) {out<<o;O::put(o);}
  };
  template<typename O> using Part=typename Raw::template Part<RawPart<O>>;
};

using ConsoleOut=StreamOut<decltype(std::cout), std::cout>;

#ifdef MENU_DEBUG
/// @brief this is a debug util, do not use otherwise!
using Debug_MinimalDrawConsole=Chain<ConsoleOut,PartialDraw>;
#endif