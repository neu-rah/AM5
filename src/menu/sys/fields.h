#pragma once

#include "menu/menu.h"
#include "menu/out.h"
#include "menu/sys/printers.h"
#include "menu/item.h"
#include "menu/sys/charMask.h"

template<Sz sz,typename Mask=CharMask::ASCII8>
struct TextField {
  template<typename I>
  struct Part:PadDraw::template Part<I> {
    using Base=typename PadDraw::template Part<I>;
    char text[sz+1]{"Rui"};
    char chk{0};
    bool edited{false};
    // constexpr Part():text{0} {}
    static constexpr Sz size() {return sz;}
    static constexpr Sz depth() {return 2;}
    bool changed() const {return edited;}
    void sync() {edited=false;}
    template<typename Out>
    void print(Out& out,Ctx& ctx) {
      Sz i=ctx.sel();
      if (ctx) {
        out.put(&text[0],i);
        out.template fmtStart<Fmt::EditCursor>(ctx);
        out.put(text[i]);
        out.template fmtStop<Fmt::EditCursor>(ctx);
        out.put(&text[i+1]);
      } else out.put(text,sz);
      Base::print(out,ctx);
    }
    template<bool isKbd,typename Nav>
    bool nav(Nav& n,const CKE& cke,const Path& path) {
      if(cke.cmd==Cmd::Key) {
        if(Mask::chk(cke.key)) {
          if(strlen(text)<sz) memccpy(&text[path.sel()+1],&text[path.sel()],0,sz);
          text[path.sel()]=cke.key;
          edited=true;
          // dout<<xy<50,3><<"sz:"<<sz<<padWith<3><<flush;
          return n.doNav({Cmd::Up},sz,Base::wraps());
          // return n.up();
        }
      }
      return n.doNav(cke,strnlen(text,sz),Base::wraps())||Base::template nav<isKbd>(n,cke,path);
    }
  };
};

/// @brief toggle enumerated field values on enter key
/// is implicitly: a RecallDraw and Recall
struct ToggleBehave {
  template<typename I>
  struct Part:RecallNavPos::template Part<I> {
    using Base=typename RecallNavPos::template Part<I>;
    using Base::Base;
    // static constexpr const Wraps wraps{Wraps::yes};
    bool changed() const {return m_changed/*||Base::changed()*/;}
    bool sync() {return m_changed=false;Base::sync();}
    template<bool isKbd,typename Nav>
    bool nav(Nav& n,const CKE& cke,const Path& path) {
      I::template nav<isKbd>(n,cke,path);
      if(cke.cmd==Cmd::Enter) {
        n.go(Base::m_sel);
        n.doNav({Cmd::Up},Base::Body::size(),Base::wraps());
        m_changed=Base::m_sel!=n.sel();
        Base::m_sel=n.sel();
        n.close();
      }
      return changed();
    }
    protected: bool m_changed{true};
  };
};

template<typename T,typename B,typename... OO>
using ToggleFieldDef=ItemDef<
  ToggleBehave,
  Menu<T,B,ParentDraw,WrapNav,OO...>
>;

template<typename T,typename B,typename... OO>
using SelectFieldDef=ItemDef<
  RecallNavPos,
  Menu<T,B,EditField,ParentDraw,OO...>
>;

template<typename T,typename B,typename... OO>
using ChooseFieldDef=ItemDef<
  RecallNavPos,
  Menu<T,B,OO...>
>;

template<typename T,typename O,typename... OO>
using NumFieldDef
  =ItemDef<T,AsEditMode<>,EditField,ParentDraw,O,OO...>;
