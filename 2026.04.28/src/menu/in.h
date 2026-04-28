#pragma once

#include "menu/sys/base.h"

//input--
template<typename K>
struct InAPI:K {
  using Base=K;
  static constexpr bool available() {return false;}
  template<typename Nav> static constexpr bool parseKey(Nav& nav,Key k) {return false;}
};

template<typename... KK>
struct InDef:APIOf<InAPI<Nil>,KK...>{
  using Base=APIOf<InAPI<Nil>,KK...>;
};


