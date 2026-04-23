#pragma once

#include "menu/item.h"
#include "menu/sys/printers.h"
#include "tinyTimeUtils.h"

template <typename T, typename B,Wraps wraps=Wraps::no,Pad pad=Pad::no>
struct Menu {
  template <typename I>
  struct Part : I {
    using Base = I;
    using This=Part<I>;
    using Title=T;
    using Body = B;
    using Base::Base;
    Title m_title{};
    Body m_body;

    constexpr Part(Title&&t,B&&b):m_title{std::forward<Title>(t)},m_body{std::forward<B>(b)}{}
    template<typename... OO>
    constexpr Part(Title&&t,OO&&... oo):m_title{std::forward<Title>(t)},m_body{std::forward<OO>(oo)...}{}

    static constexpr const Depth depth() {return 1+Body::depth();}

    template<Sz n=0> static constexpr Sz cnt() {return Body::template cnt<n+1>();}
    constexpr Sz size() const {return m_body.size();}

     bool changed() {
      if(pad==Pad::no) return m_title.changed();
      return m_body.changed();
    }


    template<typename Out> 
    void print(Out& out,Ctx& ctx) {
      m_title.print(out,ctx);
      if(pad==Pad::yes) {
        Ctx padCtx{ctx.path.next(),ctx.mode,ctx.printAt,ctx.prevSel,ctx.tops,Pad::yes};
        m_body.printBody(out,padCtx);
      }
    }

    template<typename Out>
    bool printMenu(Out& out,Ctx&& ctx) {return printMenu(out,ctx);}

    template<typename Out>
    bool printMenu(Out& out,Ctx& ctx) {
      // cout<<"Menu::printmenu "<<ctx.path<<" p_lvl:"<<ctx.printAt<<endl;
      ctx.idx=0;
      ctx.pad=pad;
      out.resume();
      if(ctx.printAt){ //(ctx.len()>1) {
        Ctx tmp=ctx.next();
        return m_body.printMenu(out,tmp,ctx.sel());
      }
      // ctx.padIdx=ctx.path.len>0?ctx.path.parent().last():0;
      bool r=out.printMenu(/*obj()*/ *this,ctx);// print the target menu
      // TinyTimeUtils::ms_delay(5000);
      // out.mode(LockMode::Update);//return to update mode
      return r;
    }

    template<typename Out>
    bool printBody(Out& out,Ctx& ctx)  
      {return m_body.printBody(out,ctx);}

    template<typename Out> 
    bool printItem(Out& out,Ctx& ctx) const 
      {return (Title::print(out,ctx),false);}
    
    template<typename Nav> 
    bool nav(Nav& n,const CKE& cke,Path p) {
      if(p.len&&m_body.nav(n,cke,p.next(),p.sel())) return true;//walk the path
      if (m_title.nav(n,cke,p)) return true;
      return p.len?n.doNav(cke,size(),wraps):false;
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

template <typename T, typename B,Pad pad=Pad::no>
using PadMenu=ItemDef<Menu<T,B,Wraps::no,pad>>;

template <typename T, typename B,Wraps w,Pad p> using MenuDef=ItemDef<Menu<T,B,w,p>>;
template <typename T, typename B,Wraps w,Pad p> using IMenuDef=IItemDef<Menu<T,B,w,p>>;

template<typename... OO> using Title=ItemDef<OO...>; 
template<typename... OO> using Label=ItemDef<AsLabel<OO...>>; 