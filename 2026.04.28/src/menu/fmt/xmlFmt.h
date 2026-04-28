#pragma once

#include "menu/sys/base.h"

struct XmlFmt {
  template<typename O>
  struct Part:Formats<O>,O {
    using Base=O;
    using Base::nl;
    using Base::put;

    int data{};//count levels of data sections
    // bool tagOpen{0};//we are on tag opening, atributes are allowed here, second open is illegal
    // bool tagClose{0};//just closing the tag
    int level{0};//tag indent level
    Fmt target{Fmt::None};//we only use this tags!

    template<Fmt tag>
    void putTag() {
      switch(tag) {
        case Fmt::None: put("None");break;
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
      static int cnt;
      if(tag==Fmt::View) {
        level=0;
        data=0;
        target=Fmt::None;
        // return;
      }
      if(((target&(Fmt::Item))|(tag|(Fmt::Data|Fmt::Title)))&&!(tag&(Fmt::Index|Fmt::NavCursor|Fmt::EditMode))) {
        put("•");
        put('>');
        target=Fmt::None;
      }
      if(tag&(Fmt::Data)){
        if(data==0) put("<![CDATA[");
        data++;
      } else {
        if(tag&(Fmt::Title|Fmt::Item|Fmt::View|Fmt::Menu|Fmt::Body)) {
          indent();
          put('<');
          putTag<tag>();
          target=tag;
          switch(tag) {
              default: break;
            case Fmt::View: 
              put(" sn=\"");
              put(cnt++);
              put('"');
              break;
            case Fmt::Body:
              put(" selidx=\"");
              put(ctx.sel());
              put('"');
              break;
          }
        }
        if(tag&(Fmt::Index|Fmt::NavCursor|Fmt::EditMode)) {
          if(target==Fmt::Item) {
            switch(tag) {
              default: break;
              case Fmt::Index: put(" idx=\"");put((char)(ctx.idx<9?'1'+ctx.idx:' '));put('"');break;
              case Fmt::NavCursor: put(" sel=\"");put(ctx?(ctx.enabled?'>':'-'):' ');put('"');break;
              case Fmt::EditMode:
                put(" mode=\"");
                switch(ctx.mode) {
                  case NavMode::Nav: put(':');break;
                  case NavMode::Edit: put('=');break;
                  case NavMode::Tune: put('.');break;
                  default: break;
                }
                put('"');
            }
            return Base::template fmtStart<tag>(ctx);;
          }
        }
        if(tag&(Fmt::View|Fmt::Menu|Fmt::Body)){
          //level incrementing tags, nl after
          put('>');
          nl();
          level++;
          Base::template fmtStart<tag>(ctx);
        }
      }
    }

    template<Fmt tag>
    void fmtStop(const Ctx& ctx) {
      if(tag&(Fmt::Data)) {
        if(data>0) data--;
        if(data==0) put("]]>");
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
