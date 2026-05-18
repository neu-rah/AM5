#pragma once

#include "menu/sys/base.h"

template<typename Cfg=Nil>
struct DataPrintAPI:DataAPI<Cfg> {
  template<typename Out>
  static constexpr void print(Out& out,Ctx& ctx) {}
};

template <typename... OO>
struct DataDef : APIOf<DataPrintAPI<>, OO...> {
  using Base = APIOf<DataPrintAPI<>, OO...>;
  using Base::Base;
  using Base::get;
  template<typename Out> void print(Out& out,Ctx&& ctx={}) {print(out,ctx);}
  template<typename Out> void print(Out& out,Ctx& ctx) {
    out.template fmtStart<Fmt::Data>(ctx);
    out.put(get());
    out.template fmtStop<Fmt::Data>(ctx);
    Base::print(out,ctx);
  }
};
