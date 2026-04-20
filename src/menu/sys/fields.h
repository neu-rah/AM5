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
    static constexpr const Wraps s_wraps{Wraps::yes};
    template<bool kbd,typename Nav>
    bool nav(Nav& n,const CKE& cke,const Path& path) {
      if(cke.cmd==Cmd::Enter) {
        n.open();
        // n.doNav({Cmd::Enter},Base::len(),Wraps::yes);
        n.go(Base::m_sel);
        n.doNav({Cmd::Up},Base::len(),Wraps::yes);
        Base::m_sel=n.sel();
        // n.doNav({Cmd::Enter},Base::len(),Wraps::yes);
        // n.close();
        return Base::template nav<kbd>(n,cke,path);
      }
      bool r=Base::template nav<kbd>(n,cke,path);
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
