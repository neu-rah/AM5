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

#include "menu/sys/base.h"
#include <cassert>
#include <algorithm>

template<typename O,typename... OO>
struct StaticBody {
  using Item=O;
  using Body=StaticBody<OO...>;
  Item m_item;
  Body m_body;
  template<typename... II>
  StaticBody(const Item& i,const II&... ii):m_item{i},m_body{ii...}{}
  static constexpr Sz depth() {return std::max(Item::depth(),Body::depth());}
  static constexpr Sz size() {return 1+Body::size();}

  template<typename Out> void printBody(Out& out,Ctx& ctx) {
    out.printItem(m_item,ctx);
    m_body.printBody(out,ctx);
  }

  template<typename Out> void printItem(Out& out,Ctx& ctx,Sz i)
    {return i?m_body.printItem(out,ctx,i-1):m_item.print(out);}

  void nav(CKE cke,Path path,Sz i) {
    if(i) m_body.nav(cke,path,i-1);
    else m_item.nav(cke,path);
  }
};

template<typename O>
struct StaticBody<O> {
  using Item=O;
  Item m_item;
  static constexpr Sz depth() {return Item::depth();}
  static constexpr Sz size() {return 1;}

  template<typename Out> void printMenu(Out& out,Ctx& ctx,Sz i) {assert(i==0);return m_item.printMenu(out,ctx);}
  template<typename Out> void printBody(Out& out,Ctx& ctx) {return out.printItem(m_item,ctx);}
  template<typename Out> void printItem(Out& out,Ctx& ctx,Sz i){return m_item.printItem(out,ctx);}
  void nav(CKE cke,Path path,Sz i) {assert(i==0);m_item.nav(cke,path);}
};