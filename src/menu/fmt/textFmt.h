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
        case Fmt::Index: put((char)(ctx.idx<9?'1'+ctx.idx:' '));break;
        case Fmt::NavCursor: put(ctx?(ctx.enabled?'>':'-'):' ');break;
        case Fmt::EditMode: switch(ctx.mode) {
          case NavMode::Nav: put(' ');break;
          case NavMode::Edit: put(':');break;
          case NavMode::Tune: put('>');break;
          default: break;
        };break;
        default: break;
      }
      Base::template fmtStart<tag>(ctx);
    }
    template<Fmt tag>
    static void fmtStop(const Ctx& ctx) {
      switch(tag) {
        case Fmt::Menu:
        case Fmt::Title:
        case Fmt::Item:
          Base::nl();
          break;
        default:break;
      }
      Base::template fmtStop<tag>(ctx);
    }
  };
};