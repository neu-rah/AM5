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
    static constexpr const Wraps wraps{Wraps::yes};
    template<typename Nav>
    bool nav(Nav& n,const CKE& cke,const Path& path) {
      if(cke.cmd==Cmd::Enter) {
        n.open();
        n.go(Base::m_sel);
        n.doNav({Cmd::Up},Base::size(),Wraps::yes);
        Base::m_sel=n.sel();
        return Base::nav(n,cke,path);
      }
      bool r=Base::nav(n,cke,path);
      return r;
    }
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
