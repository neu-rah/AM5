/**
 * @file ansiFmt.h
 * @author Rui Azevedo (ruihfazevedo@gmail.com)
 * @brief ANSI output format
 * 
 */

#pragma once

#include "menu/IO/ansiOut.h"
#include "menu/sys/formats.h"

struct ANSIFmt {
  template<typename OutPart>
  struct Part:Formats<OutPart>,UseEditCursorFmt::template Part<OutPart> {
    using This=ANSIFmt;
    using Base=typename UseEditCursorFmt::template Part<OutPart>;
    using Base::nl;
    using Base::put;
    using Base::setColors;
    using Base::padWith;
    using Base::freeX;
    using Base::freeY;
    using Base::pos;
    using Base::setPos;
    using Base::mode;
    using Base::clear;
    using Base::unlocked;
    using Base::resume;
    int itemBg(bool sel) {return sel?GREEN:BLUE;}
    int itemFg(bool sel,bool en) {return en?(sel?WHITE:GREEN):BLACK;}
    void itemColor(const Ctx& ctx) {setColors(itemFg(ctx,ctx.enabled),itemBg(ctx));}
    //TODO: consider fmt<Start/Stop> instead
    static constexpr bool onPad(const Ctx& ctx) {return ctx.pad;}

    template<Fmt tag>
    void fmtStart(const Ctx& ctx) {
      // if(tag==Fmt::Item) {
      //   dout.xy(0,1+ctx.idx+10*ctx.pad);
      //   dout.setColors(BLUE+ctx.idx,BLACK);
      //   dout<<tag<<":"<<ctx<<::padWith<10><<flush;resume();
      // }
      switch(tag) {
        case Fmt::View: clear(); break;
        case Fmt::Title: setColors(BLACK,CYAN);break;
        case Fmt::Item:
          // dout.xy(0,1+ctx.idx+11*ctx.pad);dout<<colors<BLACK,RED><<ctx<<::padWith<10><<flush;resume();
          if(ctx.lvl==ctx.printAt) {
            // if(ctx.focus()) {
              put('[');
              setColors(itemFg(ctx.focus(),ctx.enabled),itemBg(ctx.path.data[0]==ctx.idx));
              put(']');
            // } else put('?');
          } else setColors(itemFg(ctx.focus(),ctx.enabled),itemBg(ctx.focus()));
          break;
        case Fmt::Index: if(!ctx.pad) put((char)(ctx.idx<9?'1'+ctx.idx:' '));break;
        case Fmt::NavCursor:
          if(ctx.focus()&&((!ctx.pad)||(ctx.printAt<0&&ctx.mode==NavMode::Nav)))
            put(ctx.enabled?'>':'-');
          else if(!ctx.pad) put(' ');
          break;
        case Fmt::EditMode: switch(ctx.mode) {
          case NavMode::Nav: if(!ctx.pad) put(':');break;
          case NavMode::Edit: if(ctx.focus()) put('=');break;
          case NavMode::Tune: if(ctx.focus()) put('.');break;
          default: break;
        };break;
        default: break;
      }
      Base::template fmtStart<tag>(ctx);
    }

    template<Fmt tag>
    void fmtStop(const Ctx& ctx) {
      // if(!onPad(ctx)) 
      switch(tag) {
        case Fmt::Title:
          setColors(BLACK,CYAN);
          padWith(freeX());
          nl();
          break;
        case Fmt::Menu:
          setColors(itemFg(false,false),itemBg(false));
          // put('#');
          // while(freeY()>0) {
          //   padWith(freeX(),'*');
          //   nl();
          // }
          break;
        case Fmt::Item:
          if(!onPad(ctx)) {
            padWith(freeX());
            nl();
          }
          break;
        case Fmt::Unit: itemColor(ctx);break;
        default:break;
      }
      Base::template fmtStop<tag>(ctx);
    }
  };
};

