/**
 * @file ansiFmt.h
 * @author Rui Azevedo (neu-rah) (ruihfazevedo@gmail.com)
 * @brief ANSI format file
 * @version 5
 * @date 2026-04-29
 * 
 * @copyright Copyright (c) 2026
 * 
*/

#include "menu/sys/base.h"
#include "menu/sys/formats.h"

struct ANSIFmt {
  template<typename O>
  struct Part:Formats<O>,UseEditCursorFmt::template Part<O> {
    using Base=typename UseEditCursorFmt::template Part<O>;
    using Base::nl;
    using Base::fmtStart;
    using Base::fmtStop;
    using Base::put;
    using Base::clear;

    template<Fmt tag>
    std::enable_if_t<tag&Fmt::View>
    fmtStart(const Ctx& ctx) {clear();}

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