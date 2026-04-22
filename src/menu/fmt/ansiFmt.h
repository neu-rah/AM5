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
    int itemBg(bool sel) {return sel?GREEN:BLUE;}
    int itemFg(bool sel,bool en) {return en?(sel?BLUE:GREEN):BLACK;}
    void itemColor(const Ctx& ctx) {setColors(itemFg(ctx,ctx.enabled),itemBg(ctx));}
    //TODO: consider fmt<Start/Stop> instead
    template<Fmt tag>
    void fmtStart(const Ctx& ctx) {
      switch(tag) {
        case Fmt::View: 
          setColors(RED,BLUE);
          clear();
          break;
        case Fmt::Title: setColors(BLACK,MAGENTA);break;
        case Fmt::Index:if(ctx.idx<9) put(ctx.idx+1);break;
        case Fmt::NavCursor:put(ctx?(ctx.enabled?'>':'-'):' ');break;
        case Fmt::Item: itemColor(ctx);break;
        case Fmt::Field: setColors(ctx?(ctx.enabled?WHITE:BLACK):(ctx.enabled?YELLOW:BLACK),itemBg(ctx));break;
        case Fmt::EditMode:
          setColors(RED,itemBg(ctx));
          if(ctx) switch(ctx.mode){
            case NavMode::Nav: put(':');break;
            case NavMode::Edit: put('=');break;
            case NavMode::Tune: put("·");break;
          } else put(' ');
          itemColor(ctx);
          break;
        case Fmt::Unit: setColors(BLACK,itemBg(ctx));break;
        default:break;
      }
      Base::template fmtStart<tag>(ctx);
    }
    template<Fmt tag>
    void fmtStop(const Ctx& ctx) {
      switch(tag) {
        case Fmt::Title:
          setColors(BLACK,MAGENTA);
          padWith(freeX());
          nl();
          break;
        case Fmt::Menu:
          nl();
          while(freeY()>0) {
            padWith(freeX());
            nl();
          }
          break;
        case Fmt::Item:
          itemColor(ctx);
          padWith(freeX());
          nl();
          setColors(itemFg(false,true),itemBg(false));
          break;
        case Fmt::Unit: itemColor(ctx);break;
        default:break;
      }
      Base::template fmtStop<tag>(ctx);
    }
  };
};

