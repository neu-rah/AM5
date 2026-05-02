/**
 * @file textFmt.h
 * @author Rui Azevedo (neu-rah) (ruihfazevedo@gmail.com)
 * @brief text format
 * @version 5
 * @date 2026-04-28
 * 
 * @copyright Copyright (c) 2026
 * 
*/

#include "menu/sys/base.h"
#include "menu/sys/formats.h"

struct TextFmt {
  template<typename O>
  struct Part:Formats::template Part<O> {
    using Base=typename Formats::template Part<O>;
    using Base::nl;
    using Base::fmtStart;
    using Base::fmtStop;
    using Base::put;

    template<Fmt tag>
    std::enable_if_t<tag&Fmt::NavCursor>
    fmtStart(const Ctx& ctx) {
      put(ctx?ctx.enabled?'>':'-':' ');
    }

    template<Fmt tag>
    std::enable_if_t<tag&(Fmt::View|Fmt::Title|Fmt::Item)>
    fmtStop(const Ctx&) {Base::nl();}

  };
};