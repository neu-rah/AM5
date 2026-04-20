#pragma once

#include "menu/sys/base.h"

struct XmlFmt {
  template<typename O>
  struct Part:Formats<O>,O {
    using Base=O;
    // using Base::nl;
    using Base::put;
    void putTag(Fmt tag) {
      switch(tag) {
        case Fmt::View: put("view");break;
        case Fmt::Title: put("title");break;
        case Fmt::Menu: put("menu");break;
        case Fmt::Body: put("body");break;
        case Fmt::Item: put("item");break;
        case Fmt::Index: put("index");break;
        case Fmt::Accel: put("accel");break;
        case Fmt::NavCursor: put("navCursor");break;
        case Fmt::Field: put("field");break;
        case Fmt::Label: put("label");break;
        case Fmt::EditMode: put("EditMode");break;
        case Fmt::EditCursor: put("editCursor");break;
        case Fmt::Data: put("data");break;
        case Fmt::Unit: put("unit");break;
        default: put("fmt");break;
      }
    }
    void fmtStart(Fmt tag,Ctx& ctx) {
      if(tag!=Fmt::Data) {
        put('<');
        putTag(tag);
        put('>');
      }
      switch(tag) {
        case Fmt::EditCursor:
        case Fmt::EditMode:
        case Fmt::Index:
        case Fmt::NavCursor:
        case Fmt::Data: Base::put("<![CDATA[");
      }
      Base::fmtStart(tag,ctx);
    }
    void fmtStop(Fmt tag,Ctx& ctx) {
      Base::fmtStop(tag,ctx);
      switch(tag) {
        case Fmt::EditCursor:
        case Fmt::EditMode:
        case Fmt::Index:
        case Fmt::NavCursor:
        case Fmt::Data: Base::put("]]>");
      }
      if(tag!=Fmt::Data) {
        put('<');
        put('/');
        putTag(tag);
        put('>');
      }
    }
  };
};

