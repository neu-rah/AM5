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
    using Base::fmtStart;
    using Base::fmtStop;
    template<Edge edge,Fmt tag> static void fmt(const Ctx& ctx) {edge==Edge::start?fmtStart(tag,ctx):fmtStop(tag,ctx);}
    // void fmtStart(Fmt tag,const Ctx& ctx) {
    //   Base::fmtStart(tag,ctx);
    // }
    static void fmtStop(Fmt tag,const Ctx& ctx) {
      switch(tag) {
        case Fmt::Menu:
        case Fmt::Item:
          Base::nl();
          break;
        default:break;
      }
      Base::fmtStop(tag,ctx);
    }
  };
};