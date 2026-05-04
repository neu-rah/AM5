/**
 * @file xmlFmt.h
 * @author Rui Azevedo (neu-rah) (ruihfazevedo@gmail.com)
 * @brief 
 * @version 5
 * @date 2026-05-04
 * 
 * @copyright Copyright (c) 2026
 * 
*/

#include "menu/sys/base.h"
#include "menu/sys/formats.h"

struct XmlFmt {
  template<typename O>
  struct Part:Formats::template Part<O> {
    using Base=typename Formats::template Part<O>;
    using Base::nl;
    using Base::put;

    int indent{0};

    template<Fmt tag>
    static void name() {
      switch(tag) {
        case Fmt::View: Base::put("view");break;
        case Fmt::Title: Base::put("title");break;
        case Fmt::Menu: Base::put("menu");break;
        case Fmt::Body: Base::put("body");break;
        case Fmt::Item: Base::put("item");break;
        case Fmt::Index: Base::put("index");break;
        case Fmt::Accel: Base::put("accel");break;
        case Fmt::NavCursor: Base::put("nav_cursor");break;
        case Fmt::Field: Base::put("field");break;
        case Fmt::Label: Base::put("label");break;
        case Fmt::EditMode: Base::put("edit_mode");break;
        case Fmt::EditCursor: Base::put("edit_cursor");break;
        case Fmt::Data: Base::put("data");break;
        case Fmt::Unit: Base::put("unit");break;
        default: put("fmt");break;
      }
    }
    template<Fmt tag>
    void fmtStart(const Ctx& ctx) {
      if(tag&(Fmt::View|Fmt::Menu|Fmt::Body|Fmt::Title|Fmt::Item))
        for(int n=indent;n>0;n--) put("  ");
      put('<');
      name<tag>();
      put('>');
      if(tag&(Fmt::View|Fmt::Menu|Fmt::Body)) {
        indent++;
        nl();
      }
      // if(tag&(Fmt::Item|Fmt::Title)) nl();
      Base::template fmtStart<tag>(ctx);
    }
    template<Fmt tag>
    void fmtStop(const Ctx& ctx) {
      Base::template fmtStop<tag>(ctx);
      if(tag&(Fmt::View|Fmt::Menu|Fmt::Body)) indent--;
      if(tag&(Fmt::View|Fmt::Menu|Fmt::Body))
        for(int n=indent;n>0;n--) put("  ");
      put('<');
      put('/');
      name<tag>();
      put('>');
      if(tag&(Fmt::View|Fmt::Menu|Fmt::Body|Fmt::Item|Fmt::Title)) nl();
    }

  };
};
