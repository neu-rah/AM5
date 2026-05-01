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
// #include "menu/sys/printers.h"
#include "tinyTimeUtils.h"

template <typename T, typename B,Wraps w=Wraps::no,Pad pad=Pad::no>
struct Menu {
  template <typename I>
  struct Part:ItemNav::template Part<I> {
    using Base=typename ItemNav::template Part<I>;
    using This=Part<I>;
    using Title=T;
    using Body = B;
    using Base::Base;
    Title m_title{};
    Body m_body;

    constexpr const bool wraps() {return w==Wraps::yes;}
    constexpr Part(Title&&t,B&&b):m_title{std::forward<Title>(t)},m_body{std::forward<B>(b)}{}
    template<typename... II>
    constexpr Part(Title&&t,II&&... oo):m_title{std::forward<Title>(t)},m_body{std::forward<II>(oo)...}{}

    static constexpr const Depth depth() {return 1+Body::depth();}

    // template<Sz n=0> static constexpr Sz cnt() {return Body::template cnt<n+1>();}
    constexpr Sz size() const {return m_body.size();}
    constexpr const bool isPad() {return pad==Pad::yes;}

     bool changed() {//TODO: change this into a "simple" print with `LockMode::Changed` insted!
      bool r=m_title.changed();
      return pad==Pad::yes?m_body.changed()||r:r;
    }

    template<typename Out> 
    void print(Out& out,Ctx& ctx) {
      m_title.print(out,ctx);
      if(pad==Pad::yes) {//<----- this is a pad... (second pass) lets print the body inplace, will need a new ctx thou, the original will be messed up
        // dout<<xy<0,21><<colors<GREEN,BLACK><<"ctx:"<<ctx<<" pad:"<<pad<<"|"<<cnt<>++<<::padWith<10><<flush;out.resume();
        Ctx padCtx{
          ctx.path,
          ctx.mode,
          ctx.pAt,
          ctx.enabled,
          ctx.tops,
          (Depth)(ctx.at+1),
          0,
          true,
          0
        };
        // dout<<xy<0,22><<colors<YELLOW,BLACK><<"padCtx:"<<padCtx<<" pad:"<<pad<<"|"<<cnt<>++<<::padWith<10><<flush;out.resume();
        m_body.printBody(out,padCtx);
        // out.template fmtStop<Fmt::Menu>(ctx);
      }
    }

    template<typename Out>
    bool printMenu(Out& out,Ctx&& ctx) {return printMenu(out,ctx);}

    template<typename Out>
    bool printMenu(Out& out,Ctx& ctx) {
      ctx.idx=0;
      out.resume();
      if(ctx.pAt!=ctx.at){//walk to print level
        Ctx tmp=ctx.next();
        return m_body.printMenu(out,tmp,ctx.path.data[(int)ctx.at]);
      }
      if(pad==Pad::yes) {//<----- this is a pad... (second pass) lets print the body inplace, will need a new ctx thou, the original will be messed up
        // dout<<xy<0,23><<colors<GREEN,BLACK><<"*ctx:"<<ctx<<" pad:"<<pad<<"|"<<cnt<>++<<flush;out.resume();
        Ctx padCtx{
          ctx.path,//ctx.printAt>0?ctx.path.next():ctx.path,
          ctx.mode,
          ctx.pAt,
          ctx.enabled,
          ctx.tops,
          (Depth)(ctx.at+1),
          0,
          true
        };
        // dout<<xy<0,24><<colors<YELLOW,BLACK><<"*padCtx:"<<padCtx<<" pad:"<<pad<<"|"<<cnt<>++<<flush;out.resume();
        m_body.printBody(out,padCtx);
      }
      bool r=out.printMenu(/*obj()*/*this,ctx);// print the target menu
      return r;
    }

    template<typename Out>
    bool printBody(Out& out,Ctx& ctx)  
      {return m_body.printBody(out,ctx);}

    template<typename Out> 
    bool printItem(Out& out,Ctx& ctx) const 
      {return Title::print(out,ctx);}
    
    template<typename Nav> 
    bool nav(Nav& n,const CKE& cke,Path p) {
      if(p.len>0&&m_body.nav(n,cke,p.next(),p.sel())) return true;//walk the path
      // if (m_title.nav(n,cke,p)) return true;
      if(Base::nav(n,cke,p)) return true;
      return p.len?n.doNav(cke,size(),w):false;
    }

    Body& body() {return m_body;}

  //Id--
    static constexpr bool hasId(int id) {return Body::hasId(id);}
    template<int id> using HasId=std::integral_constant<bool,id==Title::getId()||typename Body::template HasId<id>{}>;

    template<int id> using WithId=typename std::conditional<
      id==Title::getId(),
      This,
      typename Body::template WithId<id>
    >::type;

    template<int id> constexpr const std::enable_if_t<id==Title::getId(),This>& withId() const {return *this;}
    template<int id> std::enable_if_t<id==Title::getId(),This>& withId() {return *this;}

    template<int id>
    constexpr const std::enable_if_t<id!=Title::getId()&&HasId<id>::value,WithId<id>>&
    withId() const {return m_body.template withId<id>();}

    template<int id>
    std::enable_if_t<id!=Title::getId()&&HasId<id>::value,WithId<id>>&
    withId() {return m_body.template withId<id>();}
  };
};

template <typename T, typename B,Wraps wraps=Wraps::no>
using PadMenu=ItemDef<Menu<T,B,wraps,Pad::yes>>;

template <typename T, typename B,Wraps w,Pad p> using MenuDef=ItemDef<Menu<T,B,w,p>>;
template <typename T, typename B,Wraps w,Pad p> using IMenuDef=IItemDef<Menu<T,B,w,p>>;

template<typename... OO> using Title=ItemDef<OO.../*,ItemNav*/>; 
template<typename... OO> using Label=ItemDef<AsLabel<OO...>>; 