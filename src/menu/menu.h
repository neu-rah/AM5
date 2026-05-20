/**
 * @file menu.h
 * @author Rui Azevedo (neu-rah) (ruihfazevedo@gmail.com)
 * @brief menu component 
 * @version 5
 * @date 2026-04-28
 * 
 * @copyright Copyright (c) 2026
 * 
*/
#pragma once

#include "menu/item.h"
#include "tinyTimeUtils.h"

struct WrapNav {
  template<typename I>
  struct Part:I {
    static constexpr bool wraps() {return true;}
  };
};

struct PadDraw {
  template<typename I>
  struct Part:ParentDraw::template Part<I> {
    static constexpr bool isPad() {return true;}
  };
};

template <typename T, typename B,typename... II>
struct Menu {
  template <typename I>
  struct Part:Chain<II...,ItemNav>::template Part<I> {
    using Base=typename Chain<II...,ItemNav>::template Part<I>;
    using This=Part<I>;
    using Title=T;
    using Body = B;
    using Base::Base;
    Title m_title{};
    Body m_body;

    constexpr Part():m_title{},m_body{} {}
    constexpr Part(Title&&t,B&&b):m_title{std::forward<Title>(t)},m_body{std::forward<B>(b)}{}
    constexpr Part(B&&b):m_title{},m_body{std::forward<B>(b)}{}
    template<typename... OO>
    constexpr Part(Title t,OO&&... oo):m_title{std::forward<Title>(t)},m_body{std::forward<OO>(oo)...}{}
    template<typename... OO>
    constexpr Part(OO&&... oo):m_title{},m_body{std::forward<OO>(oo)...}{}

    static constexpr const Depth depth() {return Body::depth()+1;}
    constexpr Sz size() const {return m_body.size();}

    bool changed() {//TODO: change this into a "simple" print with `LockMode::Changed` insted!
      bool r=m_title.changed();
      return Base::isPad()?m_body.changed()||r:r;
    }

    template<typename Out> 
    void print(Out& out,Ctx& ctx) {
      m_title.print(out,ctx);
      if(Base::isPad()) {//<----- this is a pad... (second pass) lets print the body inplace, will need a new ctx thou, the original will be messed up
        Ctx tmp{ctx.path,ctx.mode,ctx.pAt,ctx.enabled,ctx.tops,(Depth)(ctx.at+(Depth)1),0,true,0,ctx.idx};
        m_body.printBody(out,tmp);
      }
    }

    template<typename Out>
    bool printMenu(Out& out,Ctx& ctx) {
      if(ctx.pAt>ctx.at){//walk to print level
        //TODO: can this tmp be an update of ctx?
        Ctx tmp{ctx.path,ctx.mode,ctx.pAt,ctx.enabled,ctx.tops,(Depth)(ctx.at+1),0,ctx.pad,0, ctx.idx };
        Sz s=ctx.sel();
        return m_body.printMenu(out,tmp,s);
      }
      ctx.at++;
      bool r=out.printMenu(/*obj()*/*this,ctx);// print the target menu
      return r;
    }

    template<typename Out> 
    bool printBody(Out& out,Ctx& ctx)
      {return m_body.printBody(out,ctx);}

    template<typename Out> 
    bool printItem(Out& out,Ctx& ctx) const 
      {return Title::print(out,ctx);}
    
    template<bool isKbd,typename Nav> 
    bool nav(Nav& n,const CKE& cke,Path p) {
      if(p.len>0&&m_body.template nav<isKbd>(n,cke,p.next(),p.sel())) return true;//walk the path
      bool r=Base::template nav<isKbd>(n,cke,p);
      return p.len?n.doNav(cke,size(),Base::wraps())||r:r;
    }

    Body& body() {return m_body;}

    template<typename Id>
    constexpr auto withId() {
      if constexpr (Title::template has<Id>) return *this;
      else return m_body.template withId<Id>();
    }

  };
};

template <typename T, typename B,typename... OO> using PadMenu=ItemDef<Menu<T,B,PadDraw,OO...>>;
template <typename T, typename B,typename... OO> using MenuDef=ItemDef<Menu<T,B,OO...>>;
template <typename T, typename B,typename... OO> using IMenuDef=IItemDef<Menu<T,B,OO...>>;

template<typename... OO> using Title=ItemDef<OO...>; 
