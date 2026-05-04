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

// F3 -----------------------------------------------------------------
struct ANSIFmt {
  template<typename O>
  struct Part:Formats::template Part<O> {
    using Base=typename Formats::template Part<O>;
    using Base::setColors;
    using Base::clear;
    using Base::nl;
    using Base::clearLine;
    using Base::clearFree;
    using Base::put;
    using Base::resume;

      // start ---
    template<Fmt tag>
    void fmtStart(const Ctx& ctx) {
      switch(tag) {
        case Fmt::View:
          setColors(WHITE,BLUE);
          clear();
          break;
        case Fmt::Field: setColors(YELLOW,BLUE);break;
        case Fmt::Title: setColors(BLUE,WHITE);break;
        case Fmt::Item: 
          setColors(ctx.enabled?WHITE:BLACK,ctx?GREEN:BLUE);
          break;
        case Fmt::NavCursor: put(ctx?ctx.enabled?'>':'-':' ');break;
        case Fmt::Index: if(!ctx.pad) put(ctx.idx<9?1+ctx.idx:' ');break;
        default:break;
      }
      Base::template fmtStart<tag>(ctx);
    }

    // stop --
    template<Fmt tag>
    void fmtStop(const Ctx& ctx) {
      Base::template fmtStop<tag>(ctx);
      switch(tag) {
        case Fmt::Field:
        case Fmt::Item: setColors(WHITE,BLUE);break;
        default:break;
      }
    }
  };
};

