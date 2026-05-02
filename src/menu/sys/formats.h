#pragma once

#include "menu/sys/base.h"

struct Formats {
  template<typename O>
  struct Part:UseEditCursorFmt::template Part<O> {
    using IsFormat=std::true_type;
    static_assert(O::template Excludes<IsPrinter>::value,"Printers must preseed Formats");//TODO:: then chop OutDef into Printers|Fmt|Dev
  };
};

struct ClearFreeFmt {
  template<typename O>
  struct Part:O {
    static_assert(O::template Requires<IsCursor>::value,"ClearFree needs a valid Cursor (IsCursor class) not the API default fallback");
    using Base=O;
    using Base::fmtStart;
    using Base::fmtStop;
    using Base::clear;
    using Base::clearLine;
    using Base::clearFree;
    using Base::setColors;
    template<Fmt tag> 
    std::enable_if_t<tag&(Fmt::Title|Fmt::Item)>
    fmtStop(const Ctx& ctx) {
      clearLine();
      Base::template fmtStop<tag>(ctx);
    }
  };
};
