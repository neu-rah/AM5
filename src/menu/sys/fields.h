#pragma once

#include "menu/menu.h"
#include "menu/out.h"
#include "menu/sys/printers.h"
#include "menu/item.h"

/// @brief toggle enumerated field values on enter key
/// is implicitly: a RecallDraw and Recall
struct ToggleBehave {
  template<typename I>
  struct Part:RecallNavPos::Part<I> {
    using Base=typename RecallNavPos::template Part<I>;
    using Base::Base;
    static constexpr const Wraps wraps{Wraps::yes};
    bool changed() const {return m_changed/*||Base::changed()*/;}
    bool sync() {return m_changed=false;Base::sync();}
    template<typename Nav>
    bool nav(Nav& n,const CKE& cke,const Path& path) {
      I::nav(n,cke,path);
      if(cke.cmd==Cmd::Enter) {
        n.open();
        n.go(Base::m_sel);
        n.doNav({Cmd::Up},Base::size(),Wraps::yes);
        m_changed=Base::m_sel!=n.sel();
        Base::m_sel=n.sel();
        n.close();
      }
      return changed();
    }
    protected: bool m_changed{true};
  };
};

template<typename T,typename B>
using ToggleFieldDef=ItemDef<ToggleBehave,Menu<T,B>>;

template<typename T,typename B,Wraps w=Wraps::no>
using SelectFieldDef=ItemDef<
  RecallNavPos,
  Menu<typename T::template Ins<EditField,ParentDraw>::template App<ItemNav<w>>,B>
>;

template<typename T,typename B,Wraps w=Wraps::no>
using ChooseFieldDef=ItemDef<
  RecallNavPos,
  Menu<typename T::template App<ItemNav<w>>,B>
>;

template<typename T,typename O,typename... OO>
using NumFieldDef
  =ItemDef<typename T::template Ins<EditField,ParentDraw>::template App<AsEditMode<>,O,OO...>>;
