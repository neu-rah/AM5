#pragma once

#include "menu/item.h"
#include "tinyTimeUtils.h"

template <typename T, typename B>
struct Menu {
  template <typename I>
  struct Part : T::template Part<I> {
    using This=Part<I>;
    using Base = typename T::template Part<I>;
    using Body = B;
    using Base::Base;
    Body m_body;
    template<typename... OO> Part(OO... oo):m_body{oo...}{}
    
  //api--
    constexpr Part(Base&&t,B&&b):Base{std::forward<Base>(t)},m_body{std::forward<B>(b)}{}
    template<typename... OO>
    constexpr Part(Base&&t,OO&&... oo):Base{std::forward<Base>(t)},m_body{std::forward<OO>(oo)...}{}

    static constexpr const Depth depth() {return 1+Body::depth();}

    template<Sz n=0> static constexpr Sz cnt() {return Body::template cnt<n+1>();}
    constexpr Sz len() const {return m_body.len();}

    template<typename Out>
    bool printMenu(Out& out,Ctx&& ctx) {return printMenu(out,ctx);}
    template<typename Out>
    bool printMenu(Out& out,Ctx& ctx) {
      // cout<<"Menu::printmenu "<<ctx.path<<" p_lvl:"<<ctx.printAt<<endl;
      ctx.idx=0;
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
      {return (Base::print(out,ctx),false);}
    
    void nav(CKE cke,Path p) {
      if(p.len&&m_body.nav(cke,p.next(),p.sel())) return true;//walk the path
      if (Base::template nav(cke,p)) return true;
      return p.len?n.doNav(cke,len(),Base::s_wraps):false;
    }

    Body& body() {return m_body;}

  //Id--
    // static constexpr bool hasId(int id) {return Body::hasId(id);}
    // template<int id> using HasId=Any<std::integral_constant<bool,id==Base::getId()>,typename Body::template HasId<id>>;

    // template<int id> using WithId=typename std::conditional<
    //   id==Base::getId(),
    //   This,
    //   typename Body::template WithId<id>
    // >::type;

    // template<int id> constexpr const When<id==Base::getId(),This>& withId() const {return *this;}
    // template<int id> When<id==Base::getId(),This>& withId() {return *this;}

    // template<int id>
    // constexpr const When<id!=Base::getId()&&HasId<id>::value,WithId<id>>&
    // withId() const {return m_body.template withId<id>();}

    // template<int id>
    // When<id!=Base::getId()&&HasId<id>::value,WithId<id>>&
    // withId() {return m_body.template withId<id>();}
  };
};

template <typename T, typename B> using MenuDef=ItemDef<Menu<T,B>>;
template <typename T, typename B> using IMenuDef=IItemDef<Menu<T,B>>;

template<typename... OO> using Title=Chain<OO...>; 