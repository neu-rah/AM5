#pragma once

#include "menu/menu.h"
#include "menu/out.h"
#include "menu/sys/printers.h"
#include "menu/item.h"
#include "menu/sys/charMask.h"

template<Sz sz,typename Mask=CharMask::ASCII8>
struct TextField {
  template<typename I>
  struct Part:I {
    using Base=I;
    char text[sz+1];
    char chk{0};
    constexpr Part():text{0} {}
    static constexpr Sz depth() {return 2;}
    template<typename Out>
    void print(Out& out,Ctx& ctx) {out.put(text);}
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
    template<typename Nav>
    bool nav(Nav& n,const CKE& cke,const Path& path) {
      I::nav(n,cke,path);
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
