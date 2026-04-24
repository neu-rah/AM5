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
    static constexpr bool onPad(const Ctx& ctx) {return ctx.pad&&ctx.path.len==0;}
    template<Fmt tag>
    void fmtStart(const Ctx& ctx) {
      if(!onPad(ctx)) switch(tag) {
        case Fmt::View: 
          // setColors(RED,BLUE);
          clear();
          break;
        case Fmt::Title: setColors(BLACK,MAGENTA);break;
        case Fmt::Index:if((!ctx.pad)&&ctx.idx<9) put(ctx.idx+1);break;
        case Fmt::NavCursor:
          if(ctx) {
            if((!ctx.pad)||ctx.mode==NavMode::Nav) put(ctx.enabled?'>':'-');
          } else if(!ctx.pad) put(' ');
          break;
        case Fmt::Item: 
          // if(ctx.pad==Pad::no) dout<<xy<0,1><<colors<GREEN,BLACK><<ctx<<" "<<(cnt<>++)<<::padWith<10><<flush;
          // else dout<<xy<0,2><<colors<RED,BLACK><<ctx<<" "<<(cnt<>++)<<::padWith<10><<flush;
          // Base::resume();
          // if(ctx.path>1) setColors(itemFg(false,ctx.enabled),itemBg(ctx));
          // else setColors(itemFg(ctx,ctx.enabled),itemBg(ctx));
          if(ctx.path.len<=1) itemColor(ctx);
          else setColors(itemFg(false,ctx.enabled),itemBg(false));
          break;
        case Fmt::Field: setColors(ctx?(ctx.enabled?WHITE:BLACK):(ctx.enabled?YELLOW:BLACK),itemBg(ctx));break;
        case Fmt::EditMode:
          setColors(RED,itemBg(ctx));
          if(ctx) switch(ctx.mode){
            case NavMode::Nav: if(!ctx.pad) put(':');break;
            case NavMode::Edit: put('=');break;
            case NavMode::Tune: put("·");break;
          } else if(!ctx.pad) put(' ');
          itemColor(ctx);
          break;
        case Fmt::Unit: setColors(BLACK,itemBg(ctx));break;
        default:break;
      }
      Base::template fmtStart<tag>(ctx);
    }
    template<Fmt tag>
    void fmtStop(const Ctx& ctx) {
      if(!onPad(ctx)) switch(tag) {
        case Fmt::Title:
          if(!ctx.pad) {
            setColors(BLACK,MAGENTA);
            padWith(freeX());
            nl();
          }
          break;
        case Fmt::Menu:
          nl();
          while(freeY()>0) {
            padWith(freeX());
            nl();
          }
          break;
        case Fmt::Item:
          if(ctx.path==0) {
            itemColor(ctx);
            padWith(freeX());
          }
          if(!ctx.pad) nl();
          setColors(itemFg(false,true),itemBg(false));
          break;
        case Fmt::Unit: itemColor(ctx);break;
        default:break;
      }
      Base::template fmtStop<tag>(ctx);
    }
  };
};

