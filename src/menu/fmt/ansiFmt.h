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

/*******************************
* note: using this metaprog. style requires the breaking of this class into several ones
* and them to be called in chain, because we can have th same tag to appear on multiple conditions
********************************/

struct OldANSIFmt {
  template<typename O>
  struct Part:Formats::template Part<O> {
    using Base=typename Formats::template Part<O>;
    using Base::nl;
    using Base::fmtStart;
    using Base::fmtStop;
    using Base::put;
    using Base::clear;
    using Base::setColors;
    using Base::resume;

    // start ---
    template<Fmt tag>
    std::enable_if_t<tag&Fmt::View>
    fmtStart(const Ctx& ctx) {
      setColors(WHITE,BLUE);
      clear();
    }

    template<Fmt tag>
    std::enable_if_t<tag&Fmt::Title>
    fmtStart(const Ctx& ctx) {setColors(BLUE,WHITE);}

    template<Fmt tag>
    std::enable_if_t<tag&Fmt::Item>
    fmtStart(const Ctx& ctx) {setColors(ctx.enabled?WHITE:BLACK,ctx?GREEN:BLUE);}

    template<Fmt tag>
    std::enable_if_t<tag&Fmt::NavCursor>
    fmtStart(const Ctx& ctx) {put(ctx?ctx.enabled?'>':'-':' ');}

    // stop ---
    template<Fmt tag>
    std::enable_if_t<tag&(Fmt::View)>
    fmtStop(const Ctx&) {Base::clearFree();}

    template<Fmt tag>
    std::enable_if_t<tag&(Fmt::Title|Fmt::Item)>
    fmtStop(const Ctx&) {Base::clearLine();setColors(WHITE,BLUE);}

  };
};

struct ViewFmt {
  template<typename O>
  struct Part:Formats::template Part<O> {
    using Base=typename Formats::template Part<O>;
    using Base::nl;
    using Base::fmtStart;
    using Base::fmtStop;
    using Base::put;
    using Base::clear;
    using Base::setColors;
    using Base::resume;
    template<Fmt tag>
    std::enable_if_t<tag&Fmt::View>
    fmtStart(const Ctx& ctx) {
      setColors(WHITE,BLUE);
      clear();
    }
  };
};

struct TitleFmt {
  template<typename O>
  struct Part:Formats::template Part<O> {
    using Base=typename Formats::template Part<O>;
    using Base::nl;
    using Base::fmtStart;
    using Base::fmtStop;
    using Base::put;
    using Base::clear;
    using Base::setColors;
    using Base::resume;
    template<Fmt tag>
    std::enable_if_t<tag&Fmt::Title>
    fmtStart(const Ctx& ctx) {setColors(BLUE,WHITE);}
  };
};

struct ItemFmt {
  template<typename O>
  struct Part:Formats::template Part<O> {
    using Base=typename Formats::template Part<O>;
    using Base::nl;
    using Base::fmtStart;
    using Base::fmtStop;
    using Base::put;
    using Base::clear;
    using Base::setColors;
    using Base::resume;
    template<Fmt tag>
    std::enable_if_t<tag&Fmt::Item>
    fmtStart(const Ctx& ctx) {setColors(ctx.enabled?WHITE:BLACK,ctx?GREEN:BLUE);}
  };
};

struct NavCursorFmt {
  template<typename O>
  struct Part:Formats::template Part<O> {
    using Base=typename Formats::template Part<O>;
    using Base::nl;
    using Base::fmtStart;
    using Base::fmtStop;
    using Base::put;
    using Base::clear;
    using Base::setColors;
    using Base::resume;
    template<Fmt tag>
    std::enable_if_t<tag&Fmt::NavCursor>
    fmtStart(const Ctx& ctx) {put(ctx?ctx.enabled?'>':'-':' ');}
  };
};

using ANSIFmt=Chain<ViewFmt,TitleFmt,ItemFmt,NavCursorFmt>;

//---------------------
struct ANSIFuncsFmt {
  template<typename O>
  struct PartOf:Formats::template Part<O> {
    using Base=typename Formats::template Part<O>;
    using Base::setColors;
    using Base::clear;
    using Base::nl;
    using Base::clearLine;
    using Base::clearFree;
    using Base::put;

    // start --
    void viewStart(const Ctx& ctx) {
      setColors(WHITE,BLUE);
      clear();
    }
    void titleStart(const Ctx& ctx) {setColors(BLUE,WHITE);}
    void itemStart(const Ctx& ctx) {setColors(ctx.enabled?WHITE:BLACK,ctx?GREEN:BLUE);}

    void navCursorStart(const Ctx& ctx) 
      {put(ctx?ctx.enabled?'>':'-':' ');}

    // stop --
    void titleStop(const Ctx& ctx) {clearLine();}
    void itemStop(const Ctx& ctx) {clearLine();setColors(WHITE,BLUE);}
    void viewStop(const Ctx& ctx) {clearFree();}
  };
  template<typename O> using Part=UseFmtFuncs::template Part<PartOf<O>>;
};

struct ANSICaseFmt {
  template<typename O>
  struct Part:O {
    using Base=O;
    using Base::setColors;
    using Base::clear;
    using Base::nl;
    using Base::clearLine;
    using Base::clearFree;
    using Base::put;

      // start ---
    template<Fmt tag>
    void fmtStart(const Ctx& ctx) {
      switch(tag) {
        case Fmt::View:
          setColors(WHITE,BLUE);
          clear();
          break;
        case Fmt::Title:setColors(BLUE,WHITE);break;
        case Fmt::Item:setColors(ctx.enabled?WHITE:BLACK,ctx?GREEN:BLUE);break;
        case Fmt::NavCursor: put(ctx?ctx.enabled?'>':'-':' ');break;
        default:break;
      }
    }

    // stop --
    template<Fmt tag>
    void fmtStop(const Ctx& ctx) {
      switch(tag) {
        case Fmt::NavCursor: put(ctx?ctx.enabled?'>':'-':' ');break;
        case Fmt::Item:
          clearLine();
          setColors(WHITE,BLUE);
          break;
        case Fmt::View:
        case Fmt::Title:
          clearLine();
        default:break;
      }
    }
  };
};