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
    // static constexpr const padNav(const Ctx& ctx) {return}
    template<Fmt tag>
    void fmtStart(const Ctx& ctx) {
      if(tag==Fmt::Item) {
        dout.xy(0,1+ctx.idx+3*ctx.pad);
        dout.setColors(BLUE+ctx.idx,BLACK);
        dout<<tag<<" Fmt ctx:"<<ctx<<::padWith<10><<flush;resume();
      }
      // if(!onPad(ctx)) 
      switch(tag) {
        case Fmt::View: 
          // setColors(RED,BLUE);
          clear();
          break;
        case Fmt::Title: setColors(BLACK,CYAN);break;
        // case Fmt::Index:if((!ctx.pad)&&ctx.idx<9) put(ctx.idx+1);break;
        case Fmt::NavCursor:
          // if(ctx) {//<-- selected? --
          //   if(!ctx.pad) put(ctx.enabled?'>':'-');
          // } else if(!onPad(ctx)) put(' ');
          break;
        case Fmt::Item: 
          if(ctx) {
            if((ctx.path.len==1&&!ctx.pad)||(ctx.pad&&ctx.printAt<0)) {
              itemColor(ctx);
              put('%');
            } else {
              setColors(itemFg(true,ctx.enabled),itemBg(true));
              put('$');
            }
          } else setColors(itemFg(false,ctx.enabled),itemBg(false));
          break;
        // case Fmt::Field:
        //   if(!onPad(ctx)) setColors(ctx?(ctx.enabled?WHITE:BLACK):(ctx.enabled?YELLOW:BLACK),itemBg(ctx));break;
        // case Fmt::EditMode:
        //   if(!onPad(ctx)) setColors(RED,itemBg(ctx));
        // if(ctx&&!onPad(ctx)) switch(ctx.mode){
        //     case NavMode::Nav: if(!onPad(ctx)) put(':');break;
        //     case NavMode::Edit: put('=');break;
        //     case NavMode::Tune: put("·");break;
        //   } else if(!onPad(ctx)) put(' ');
        //   if(!onPad(ctx)) itemColor(ctx);
        //   break;
        // case Fmt::Unit: setColors(BLACK,itemBg(ctx));break;
        default:break;
      }
      Base::template fmtStart<tag>(ctx);
    }

    template<Fmt tag>
    void fmtStop(const Ctx& ctx) {
      // if(!onPad(ctx)) 
      switch(tag) {
        case Fmt::Title:
          if(!onPad(ctx)) {
            setColors(BLACK,CYAN);
            padWith(freeX(),'-');
            nl();
          }
          break;
        case Fmt::Menu:
          // nl();
          while(freeY()>0) {
            padWith(freeX(),'*');
            nl();
          }
          break;
        case Fmt::Item:
          if(ctx.path.len==0) {
            itemColor(ctx);
            padWith(freeX(),'+');
          }
          if(!onPad(ctx)) nl();
          setColors(itemFg(false,true),itemBg(false));
          break;
        // case Fmt::Unit: itemColor(ctx);break;
        default:break;
      }
      Base::template fmtStop<tag>(ctx);
    }
  };
};

