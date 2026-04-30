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

// #include "menu/item.h"

template<typename O,typename... OO>
struct StaticBody {
  using Item=O;
  using Body=StaticBody<OO...>;
  template<template<typename> class M>
  using Map=StaticBody<M<O>,M<OO>...>;
  Item m_item;
  Body m_body;
  constexpr StaticBody() {}
  template<typename... II> constexpr StaticBody(Item&& i,II&&... ii):m_item{std::forward<Item>(i)},m_body{std::forward<II>(ii)...}{}
  // template<typename... II> constexpr StaticBody(II&&... ii):m_body{std::forward<II>(ii)...}{}
  static constexpr const Depth depth() {return staticMax<Item::depth(),Body::depth()>();}
  static constexpr Sz size() {return 1+Body::size();}

  bool changed() {return m_item.changed()||m_body.changed();}
  
  template<typename Out> bool printMenu(Out& out,Ctx& ctx,Sz i)
    {return i?m_body.printMenu(out,ctx,i-1):m_item.printMenu(out,ctx);}

  template<typename Out> bool printBody(Out& out,Ctx& ctx,Sz bidx=0) {
    bool r=false;
    // if(m_item.isPad()) {//first pass
    //   dout.xy(0,1+bidx);dout<<colors<RED,BLACK><<ctx<<"|"<<cnt<>++<<padWith<10><<flush;out.resume();
    //   Ctx padCtx{
    //     ctx.path,//ctx.printAt>1?ctx.path.next():ctx.path,
    //     ctx.mode,
    //     ctx.printAt,
    //     0,
    //     ctx.tops
    //   };
    //   dout.xy(0,2+bidx);dout<<colors<BLUE,BLACK><<padCtx<<"|"<<cnt<>++<<padWith<10><<flush;out.resume();
    //   r=out.printItem(m_item,padCtx);
    // } else 
    // if(m_item.isPad()) {//first pass
      r=out.printItem(m_item,ctx);
    return m_body.printBody(out,ctx,bidx+1)||r;
  }

  template<typename Out> bool printItem(Out& out,Ctx& ctx,Sz i) {
    // ctx.pad=ctx&&m_item.isPad();
    // ctx.padIdx=size()-i;
    return i?m_body.printItem(out,ctx,i-1):m_item.print(out,ctx);
  }

  template<typename Nav>
  bool nav(Nav& n,const CKE& cke,Path path,Sz i)
    {return i?m_body.nav(n,cke,path,i-1):m_item.nav(n,cke,path);}
//Id, this is compile-time search/reference --
  template<int id>
  using HasId=std::integral_constant<bool,
    id==Item::getId()||typename Item::template HasId<id>{}||typename Body::template HasId<id>{}
  >;
  

  template<int id>
  using WithId=typename std::conditional<
    id==Item::getId(),
    Item,
    typename std::conditional<
      Item::template HasId<id>::value,
      typename Item::template WithId<id>,
      typename Body::template WithId<id>
    >::type
  >::type;

  template<int id>
  std::enable_if_t<id==Item::getId(),WithId<id>>& withId() {return m_item;}

  template<int id>
  std::enable_if_t<
    Item::template HasId<id>::value&&id!=Item::getId(),
    typename Item::template WithId<id>
  >& withId() {return m_item.template withId<id>();}

  template<int id>
  std::enable_if_t<
    Body::template HasId<id>::value&&id!=Item::getId(),
    typename Body::template WithId<id>
  >& withId() {return m_body.template withId<id>();}

};

template<typename O>
struct StaticBody<O> {
  using Item=O;
  template<template<typename> class M> using Map=StaticBody<M<O>>;
  Item m_item;
  // constexpr StaticBody(Item&& i):m_item{std::forward<Item>(i)}{}

  static constexpr const Depth depth() {return Item::depth();}
  static constexpr Sz size() {return 1;}

  bool changed() {return m_item.changed();}

  template<typename Out> bool printMenu(Out& out,Ctx& ctx,Sz i) 
    {assert(i==0);return m_item.printMenu(out,ctx);}

  template<typename Out> bool printBody(Out& out,Ctx& ctx,Sz bidx=0) {
    // ctx.pad=m_item.isPad();
    // ctx.padIdx=bidx;
    return out.printItem(m_item,ctx);
  }

  template<typename Out> bool printItem(Out& out,Ctx& ctx,Sz i)
    {return m_item.print(out,ctx);}

  template<typename Nav> bool nav(Nav& n,const CKE& cke,Path path,Sz i) 
    {assert(i==0);return m_item.nav(n,cke,path);}
//Id--
  template<int id>
  using HasId=std::integral_constant<bool,id==Item::getId()||typename Item::template HasId<id>{}>;

  template<int id>
  using WithId=typename std::conditional<id==Item::getId(),Item,typename Item::template WithId<id>>::type;

  template<int id>
  std::enable_if_t<
    id==Item::getId(),
    WithId<id>
  >& withId() {return m_item;}

  template<int id>
  std::enable_if_t<
    Item::template HasId<id>::value&&id!=Item::getId(),
    WithId<id>
  >& withId() {return Item::template withId<id>();}
};