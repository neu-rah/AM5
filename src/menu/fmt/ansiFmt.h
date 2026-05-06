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
  struct Part:UseEditCursorFmt::template Part<O> {
    using Base=typename UseEditCursorFmt::template Part<O>;
    using Base::setColors;
    using Base::clear;
    using Base::nl;
    using Base::clearLine;
    using Base::clearFree;
    using Base::put;
    using Base::resume;

    static constexpr int fg(bool en) {return en?WHITE:BLACK;}
    static constexpr int bg(bool sel) {return sel?GREEN:BLUE;}
    static constexpr bool psel(const Ctx& ctx) {return ctx.sel(ctx.pAt)==ctx.pIdx;}// <=> parent is selected?
    static constexpr Depth after(const Ctx& ctx) {return ctx.path.len-ctx.pAt;}// <=> depth after print root, 1=>menu nav, 2=>pad menu nav, 3=>pad menu edit
    
    static constexpr Colors<int> fb(const Ctx& ctx) {
        int a = after(ctx);
        bool b = ctx;
        bool c = ctx.pad;
        bool d = psel(ctx);

        if(c&&d) {//pad focus
          if(b) {//selected
            switch(a) {//pad focus
              default: return {ctx?BLACK:WHITE,GREEN};//nav
              case 2: return {GREEN,WHITE};//pad nav
              case 3: return {RED,WHITE};//pad edit
            }
          } else return {BLACK,GREEN};//on pad with parent selected
        } else if((b&&!c)||(b&&c&&d&&a>1)) return {WHITE,GREEN};//pad parent focus
        return {WHITE,BLUE};//unselected
    }

    // start ---
    template<Fmt tag>
    void fmtStart(const Ctx& ctx) {
      switch(tag) {
        case Fmt::View:
          setColors(WHITE,BLUE);
          clear();
          break;
        case Fmt::Title: setColors(BLUE,WHITE);break;
        // case Fmt::Field: setColors(ctx.enabled?YELLOW:BLACK,bg(psel(ctx)));
        case Fmt::Item: {
          Colors<int> o=fb(ctx);
          setColors(o.fg,o.bg);
          // put((int)after(ctx));
          // put((bool)ctx);
          // put(ctx.pad);
          // put(psel(ctx));
        } break;
      }
      switch(tag){//for text
        case Fmt::EditMode:
          // setColors(fg(ctx.enabled),bg(ps));
          if((ctx&&!ctx.pad)||(ctx&&ctx.pad&&psel(ctx))) switch(ctx.mode) {
            default: break;
            case NavMode::Nav: put(':');break;
            case NavMode::Edit: put('=');break;
            case NavMode::Tune: put('.');break;
          } else if(!ctx.pad) put(' ');
          break;
        case Fmt::NavCursor: 
          // setColors(fg(ctx.enabled),bg(ps));
          if(!ctx.pad) {
            if (ctx) put(ctx.enabled?'>':'-');
            else put(' ');
          }
          break;
        case Fmt::Index: 
          if(!ctx.pad) put(ctx.idx<9?1+ctx.idx:' ');
          break;
        default:break;
      }
      Base::template fmtStart<tag>(ctx);
    }

    // stop --
    template<Fmt tag>
    void fmtStop(const Ctx& ctx) {
      Base::template fmtStop<tag>(ctx);
    }
  };
};

