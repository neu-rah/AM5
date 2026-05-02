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
    void clearLine() {Base::padWith(Base::freeX());Base::nl();}
    void clearFree() {do clearLine(); while(Base::freeY());}
    template<Fmt tag> void fmtStop(const Ctx& ctx) {
      if(tag&Fmt::View)clearFree();
      else if(tag&(Fmt::Title|Fmt::Item))clearLine();
    }
  };
};
