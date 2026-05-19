/**
 * @file staticBody.h
 * @author Rui Azevedo (neu-rah) (ruihfazevedo@gmail.com)
 * @brief 
 * @version 5
 * @date 2026-04-18
 * 
 * @copyright Copyright (c) 2026
 * 
*/

#pragma once

#include <oneList.h>
using hapi::one_list::List;

template<typename O,typename... OO>
struct StaticBody:List<O,OO...> {
  using Base=List<O,OO...>;
  using Base::Base;
  using Tail=StaticBody<OO...>;
  using Base::head;
  using Base::tail;

  bool changed() {return head.changed()||tail.changed();}
  
  template<typename Out> bool printMenu(Out& out,Ctx& ctx,Sz i)
    {return i?tail.printMenu(out,ctx,i-1):head.printMenu(out,ctx);}

  template<typename Out> bool printBody(Out& out,Ctx& ctx,Sz bidx=0) {
    bool r=out.printItem(head,ctx);
    return tail.printBody(out,ctx,bidx+1)||r;
  }

  template<typename Out> bool printItem(Out& out,Ctx& ctx,Sz i) 
    {return i?tail.printItem(out,ctx,i-1):head.print(out,ctx);}

  template<bool isKbd,typename Nav>
  bool nav(Nav& n,const CKE& cke,Path path,Sz i)
    {return i?tail.template nav<isKbd>(n,cke,path,i-1):head.template nav<isKbd>(n,cke,path);}
};

template<typename O>
struct StaticBody<O>:List<O> {
  using Base=List<O>;
  using Base::Base;
  using Base::head;

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