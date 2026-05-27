/**
 * @file staticBody.h
 * @author Rui Azevedo (neu-rah) (ruihfazevedo@gmail.com)
 * @brief static body for tiny menus
*/

#pragma once

#include "menu/sys/base.h"
#include <oneList.h>
using hapi::one_list::List;

template<typename O,typename... OO>
struct StaticBody:List<O,OO...> {
  using Base=List<O,OO...>;
  using Base::Base;
  using Tail=StaticBody<OO...>;
  using Base::head;
  using Base::tail;

  template<typename T>
  static constexpr const bool has{has<T,O>||has<T,Tail>};

  static constexpr Sz size() {return Base::size;}

  static constexpr const Depth depth() {return staticMax<Base::Head::depth(),Tail::depth()>();}

  bool changed() {return head.changed()||((Tail&)tail).changed();}
  
  template<typename Out> bool printMenu(Out& out,Ctx& ctx,Sz i)
    {return i?((Tail&)tail).printMenu(out,ctx,i-1):head.printMenu(out,ctx);}

  template<typename Out> bool printBody(Out& out,Ctx& ctx,Sz bidx=0) {
    bool r=out.printItem(head,ctx);
    return ((Tail&)tail).printBody(out,ctx,bidx+1)||r;
  }

  template<typename Out> bool printItem(Out& out,Ctx& ctx,Sz i) 
    {return i?((Tail&)tail).printItem(out,ctx,i-1):head.print(out,ctx);}

  template<bool isKbd,typename Nav>
  bool nav(Nav& n,const CKE& cke,Path path,Sz i)
    {return i?((Tail&)tail).template nav<isKbd>(n,cke,path,i-1):head.template nav<isKbd>(n,cke,path);}
};

template<typename O>
struct StaticBody<O>:List<O> {
  using Base=List<O>;
  using Base::Base;
  using Base::head;
  using Base::size;

  template<typename T>
  static constexpr const bool has{O::template has<T>};

  static constexpr const Depth depth() {return Base::Head::depth();}

  bool changed() {return head.changed();}

  template<typename Out> bool printMenu(Out& out,Ctx& ctx,Sz i) 
    {assert(i==0);return head.printMenu(out,ctx);}

  template<typename Out> bool printBody(Out& out,Ctx& ctx,Sz bidx=0) {return out.printItem(head,ctx);}

  template<typename Out> bool printItem(Out& out,Ctx& ctx,Sz i)
    {return head.print(out,ctx);}

  template<bool isKbd,typename Nav> bool nav(Nav& n,const CKE& cke,Path path,Sz i) 
    {assert(i==0);return head.template nav<isKbd>(n,cke,path);}
};