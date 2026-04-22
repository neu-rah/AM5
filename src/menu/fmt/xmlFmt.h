#pragma once

#include "menu/sys/base.h"

struct XmlFmt {
  template<typename O>
  struct Part:Formats<O>,O {
    using Base=O;
    using Base::nl;
    using Base::put;

    int data{};//count levels of data sections
    bool tagOpen{0};//we are on tag opening, atributes are allowed here, second open is illegal
    bool tagClose{0};//just closing the tag
    int level{0};//tag indent level
    // Fmt tag;//we only use this tags!

    template<Fmt tag>
    void putTag() {
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
    void indent() {for(int n=0;n<level;n++) put("  ");}

    template<Fmt tag>
    void fmtStart(const Ctx& ctx) {
      if(tag&(Fmt::EditCursor|Fmt::EditMode|Fmt::Index|Fmt::Data)){
        put("<![CDATA[");
        data++;
        switch(tag) {
          case Fmt::EditCursor: 
        }
      } else {
        if(tag&(Fmt::Title|Fmt::Item|Fmt::View|Fmt::Menu|Fmt::Body)) {
          indent();
          put('<');putTag<tag>();put('>');
          // nl();
        }
        if(tag&(Fmt::View|Fmt::Menu|Fmt::Body)){
          //level incrementing tags, nl after
          nl();
          level++;
        }
      }
      Base::template fmtStart<tag>(ctx);
    }

    template<Fmt tag>
    void fmtStop(const Ctx& ctx) {
      if(tag&(Fmt::EditCursor|Fmt::EditMode|Fmt::Index|Fmt::Data)){
        put("]]>");
        data--;
      } else {
        if(tag&(Fmt::View|Fmt::Menu|Fmt::Body)) {
          // nl();
          level--;
          indent();
        }
        if(tag&(Fmt::Title|Fmt::Item|Fmt::View|Fmt::Menu|Fmt::Body)) {
          put('<');put('/');putTag<tag>();put('>');
          nl();
        }
      }
      Base::template fmtStop<tag>(ctx);
    }

  };
};
