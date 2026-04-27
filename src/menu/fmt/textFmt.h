/**
 * @file textFmt.h
 * @author Rui Azevedo (neu-rah) (ruihfazevedo@gmail.com)
 * @brief 
 * @version 5
 * @date 2026-04-18
 * 
 * @copyright Copyright (c) 2026
 * 
*/

#pragma once

#include "menu/sys/base.h"

struct TextFmt {
  template<typename O>
  struct Part:O {
    using Base=O;
    using Base::nl;
    using Base::put;
    using Base::fmtStart;
    using Base::fmtStop;
    template<Fmt tag>
    void fmtStart(const Ctx& ctx) {
      switch(tag) {
        case Fmt::Index: if(!ctx.pad) put((char)(ctx.idx<9?'1'+ctx.idx:' '));break;
        case Fmt::NavCursor:
          if(ctx.focus()&&((!ctx.pad)||(ctx.path.len>1)))
            put(ctx.enabled?'>':'-');
          else if(!ctx.pad) put(' ');
          break;
        case Fmt::EditMode: switch(ctx.mode) {
          case NavMode::Nav: put(':');break;
          case NavMode::Edit: put('=');break;
          case NavMode::Tune: put('.');break;
          default: break;
        };break;
        default: break;
      }
      Base::template fmtStart<tag>(ctx);
    }
    template<Fmt tag>
    void fmtStop(const Ctx& ctx) {
      switch(tag) {
        case Fmt::Menu: Base::nl(); break;
        case Fmt::Title:
        case Fmt::Item: 
          if(!ctx.pad) Base::nl(); 
          break;
        default:break;
      }
      Base::template fmtStop<tag>(ctx);
    }
  };
};