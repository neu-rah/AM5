/**
 * @file item.h
 * @author Rui Azevedo (ruihfazevedo@gmail.com)
 * @brief item API
 * @version 5
 * @date 2026-04-17
 * 
 * @copyright Copyright (c) 2026
 * 
*/

#pragma once

#include "menu/sys/base.h"

template<typename Def=Nil>
struct ItemAPI:Def {
  constexpr ItemAPI() {}
  // static constexpr const Wraps wraps{Wraps::no};
  template<typename> using Requires=std::false_type;
  template<typename> using Excludes=std::true_type;
  static constexpr Depth depth() {return 1;}
  static constexpr bool enabled() {return true;}
  constexpr const bool wraps() {return false;}
  constexpr const bool isPad() {return false;}
  static constexpr void enable(bool=true) {}
  static constexpr bool changed() {return false;}
  static constexpr void sync() {}
  template<typename Out> static constexpr void sync(Out&) {}
  static constexpr bool up() {return false;}
  static constexpr bool down() {return false;}
  template<typename Out> static constexpr bool printMenu(Out&,Ctx&) {return false;}
  template<typename Out> static constexpr bool printBody(Out&,Ctx&) {return false;}
  template<typename Out> static constexpr bool printItem(Out&,Ctx&) {return false;}
  template<typename Out> static constexpr void print(Out&,Ctx&) {}
  template<typename Nav> static constexpr bool nav(Nav& n,const CKE& cke,Path) {return false;}
  //Id--
  static constexpr int getId() {return -1;}
  template<int> using HasId=std::false_type;
  template<int> using WithId=ItemAPI<CRTP<ItemAPI<Nil>>>;
};

// template<typename N>
// struct ItemLink:N {
//   template<typename O>
//   struct Part:N::template Part<O> {
//     using Base=typename N::template Part<O>;
//     using Base::Base;
//     // constexpr void sync() {Base::sync();O::sync();}
//     constexpr bool changed() const {return Base::changed()||O::changed();}
//     //API chain calls for nav function
//     // template<typename Nav>
//     // bool nav(Nav& n,const CKE& cke,const Path p) 
//     //   {return Base::nav(n,cke,p)||O::nav(n,cke,p);}
//   };
// };

template<typename... OO>
struct ItemDef:APIOf<ItemAPI<>,OO...>{//::template Map<ItemLink> {
  using Base=APIOf<ItemAPI<>,OO...>;//::template Map<ItemLink>;
  using Base::Base;
  using Base::printMenu;
  using Base::enabled;
  using Base::print;
  template<typename Out> void printMenu(Out& out,Ctx&& ctx)
    {Base::printMenu(out,ctx);}
  void enter(Path path) {nav({Cmd::Enter},path);}

  template<typename Nav>
  bool nav(Nav& n,const CKE& cke,const Path p)
    {return enabled()?Base::nav(n,cke,p):cke.cmd==Cmd::Enter;}

  template<typename Out> bool print(Out& out,Ctx&& ctx={})  
    {return print(out,std::forward<Ctx&>(ctx));}

  template<typename Out> bool print(Out& out,Ctx& ctx) {
    // out.fmtStart(Fmt::Data,ctx);
    // out.put(Base::get());
    Base::print(out,ctx);//<----------------------------------
    // out.fmtStop(Fmt::Data,ctx);
    return Base::changed();
  }

  template<typename... XX> using Ins=::ItemDef<XX...,OO...>;
  template<typename... XX> using App=::ItemDef<OO...,XX...>;

};

struct IItem {
  virtual bool printMenu(IOut& out,Ctx& ctx)=0;
  virtual bool printBody(IOut& out,Ctx&)=0;
  virtual void print(IOut& out,Ctx&)=0;
  virtual bool enabled() const=0;
  virtual void enable(bool=true)=0;
  virtual bool changed()=0;
  // virtual bool changed(IOut& out)=0;
  virtual void sync()=0;
  virtual void sync(IOut& out)=0;
  virtual bool up() const=0;
  virtual bool down() const=0;
  virtual bool nav(INav& n,const CKE& cke,const Path p)=0;

  template <typename Out>
  static constexpr bool printMenu(Out& out,Ctx& ctx) 
    {return printMenu(out,ctx);}

};

template<typename... II>
struct IItemDef:IItem, ItemDef<II...> {
  using Base=ItemDef<II...>;
  using Base::Base;

  // virtual Depth depth() const override {return Base::depth();}
  virtual bool printMenu(IOut& out,Ctx& ctx) override {return Base::printMenu(out,ctx);}
  virtual bool printBody(IOut& out,Ctx& ctx) override {return Base::printBody(out,ctx);}
  virtual void print(IOut& out,Ctx& ctx) override {Base::print(out,ctx);}
  virtual bool enabled() const override {return Base::enabled();}
  virtual void enable(bool o=true) override {return Base::enable(o);}
  virtual bool changed() override {return Base::changed();}
  // virtual bool changed(IOut& out) override {return Base::changed(out);}
  virtual void sync() override {Base::sync();}
  virtual void sync(IOut& out) override {Base::sync(out);}
  virtual bool up() const {return Base::up();};
  virtual bool down() const {return Base::down();};
  virtual bool nav(INav& n,const CKE& cke,const Path p) override {return Base::nav(n,cke,p);}
  template <typename Out> static constexpr bool printMenu(Out& out,Ctx& ctx) {return Base::printMenu(out,ctx);}
  template<typename Out> static constexpr void print(Out& out,Ctx& ctx) {return Base::print(out,ctx);}

  //Id--
  static constexpr int getId() {return -1;}
  template<int> using HasId=std::false_type;
  template<int> using WithId=ItemAPI<CRTP<ItemAPI<Nil>>>;
};

//---------------------------------------------------------------------------------------------
using ActionFunc=bool(&)(int);

template<ActionFunc action>
struct Action {
  template<typename I>
  struct Part:I {
    using Base=I;
    using Base::Base;
    constexpr Part(){}
    template<typename Nav>
    static constexpr bool nav(Nav& n,const CKE& cke,Path path) 
      {return cke.cmd==Cmd::Enter&&action(path.sel());}
  };
};

//attach an action on enter
template <ActionFunc f>
struct BodyAction {
  template <typename I>
  struct Part : I {
    using Base=I;
    using Base::Base;
    using Base::enabled;
    template<typename Nav>
    bool nav(Nav& n,const CKE& cke,const Path p) {
      if(cke.cmd==Cmd::Enter&&p.len) f(p.last());
      return Base::nav(n,cke,p);
    }
  };
};

template<typename... II>
struct Hidden {
  template<typename I>
  struct Part:Chain<II...>::template Part<I> {
    using Base=typename Chain<II...>::template Part<I>;
    using Base::Base;
    template<typename Out>
    void print(Out& out,Ctx& ctx) {I::print(out,ctx);}
  };
};

template<bool ens>
struct EnDis {
  template<typename I>
  struct Part:Chain<Default<bool,ens>,Hidden<Bool>>::template Part<I> {
    using Base=typename Chain<Default<bool,ens>,Hidden<Bool>>::template Part<I>;
    using Base::Base;
    bool enabled(const Path ={}) const {return Base::get();}
    void enable(bool e=true) {Base::set(e);}
    template<typename Nav>
    bool nav(Nav& n,const CKE& cke,const Path path)
      {return enabled()?Base::nav(n,cke,path):false;}
  };
};

/// @brief just closes the nav level on enter
/// this is now the default behavior if returning false (fallback)
// struct CloseOnSelect {
//   template<typename I>
//   struct Part:I {
//     using I::I;
//     template<typename Nav>
//     bool nav(Nav& n,const CKE& cke,const Path path) {
//       if(cke.cmd==Cmd::Enter) return n.close();
//       return I::nav(n,cke,path);
//     }
//   };
// };

//fields ---
struct EditField {
  template<typename I>
  struct Part:I {
    using Base=I;
    using Base::Base;
    template<typename Nav>
    bool nav(Nav& n,const CKE& cke,const Path& path) {
      bool r=false;
      if(cke.cmd==Cmd::Enter) {
        n.navMode(path.len?NavMode::Nav:NavMode::Edit);
        r=true;
      }
      return I::nav(n,cke,path)||r;
    }
  };
};

//use range and data field to change value
template<typename...II>
struct NumField {
  template<typename I>
  struct Part:Chain<II...>::template Part<I> {
    using Base=typename Chain<II...>::template Part<I>;
    using Base::Base;
    using Base::set;
    using Base::get;
    using Base::up;
    using Base::down;
    using Type=typename Base::Type;
    // bool up(DataType n=1) {set(Base::up(Base::get(),n));return Base::up();}
    // bool down(DataType n=1) {set(Base::down(get(),n));return Base::down();}
    template<typename Nav>
    bool nav(Nav& n,const CKE& cke,const Path path) {
      if(n.navMode()==NavMode::Edit) switch(cke.cmd){
        case Cmd::Up: Base::up(); return true;
        case Cmd::Down: Base::down(); return true;
        default: break;
      }
      return Base::nav(n,cke,path);
    }
  };
};

/// @brief store and restore navigation position
struct RecallNavPos {
  /// @brief provide a default value to a index Recall
  /// @tparam val Sz index
  template<Sz val>
  using Default=Chain<Default<Sz,val>>;
  template<typename I>
  struct Part:I {
    using Base=I;
    using Base::Base;
    template<typename Out> 
    void print(Out& out,Ctx& ctx) {
      Base::print(out,ctx);
      out.setPos(out.pos());
      if(ctx) {
        if(ctx.mode==NavMode::Edit) Base::m_body.printItem(out,ctx,ctx.path.last());
        else Base::m_body.printItem(out,ctx,m_sel);
      }
      else Base::body().printItem(out,ctx,m_sel);
    }
    template<typename Nav>
    bool nav(Nav& n,const CKE& cke,const Path& path) {
      if(cke.cmd==Cmd::Enter) {
        if(path.len) m_sel=path.sel();//store selected item index
        else {
          bool r=I::nav(n,cke,path);//still check base
          n.go(m_sel);//restore selected index
          return r;
        }
      }
      return I::nav(n,cke,path);//still check base
    }
    protected: Sz m_sel{0};
  };
};

struct ParentDraw {
  template<typename I>
  struct Part:I {
    using I::I;
    static_assert(I::template Excludes<Class<RecallNavPos>>::value,"Recall must preseed ParentDraw");
    template<typename Nav>
    bool nav(Nav& n,const CKE& cke,const Path& path) {
      if(cke.cmd==Cmd::Enter) return path.len?n.close():n.padOpen();
      return  I::nav(n,cke,path);
    }
  };
};

/// @brief put nav focus on this item on Cmd::Enter
struct ItemNav {
  template<typename I>
  struct Part:I {
    using Base=I;
    using Base::Base;
    static_assert(I::template Excludes<Class<RecallNavPos>>::value,"Recall must preseed ItemNav");
    static_assert(I::template Excludes<Class<ParentDraw>>::value,"ParentDraw must preseed ItemNav");
    template<typename Nav>
    bool nav(Nav& n,const CKE& cke,const Path path) {
      bool r=Base::nav(n,cke,path);
      if(cke.cmd==Cmd::Enter) {
        if(path.len==0) return n.open();
        else if(path.len==1) return n.close();
      }
      return r;
    }
  };
};

/// @brief alternative representation for a value
/// @tparam Title title type
/// @tparam Value value type
/// @todo why not derive from the target and redirect only the print? Enum depends on this!
template<typename Title,typename Value>
struct Alias {
  template<typename O>
  struct Part:Title::template Part<O> {
    using Base=typename Title::template Part<O>;
    ItemDef<Value> value{};
    using Base::Base;
    template<typename... OO>
    Part(Value&& v,OO&&... oo):value{v},Base{std::forward<OO&&>(oo)...}{}
  };
};

template<typename R,R& ref>
struct ItemRef {
  template<typename O>
  struct Part:O {
    using Base=O;
    using Base::Base;
    using RefType=R;
    operator RefType&() const {return ref;}
    static constexpr const Depth depth() {return ref.depth();}
    static constexpr Sz size(Path p={}) {return ref.size(p);}
    static constexpr bool enabled() {return ref.enable(); }
    static constexpr void enable(bool o=true) {ref.enable(o);}
    static constexpr bool changed() {return ref.changed();}
    static constexpr void sync() {ref.sync();}
    static constexpr bool up() {return ref.up();}
    static constexpr bool down() {return ref.down();}

    template <typename Out>
    static constexpr bool printMenu(Out& out,Ctx& ctx) 
      {return ref.printMenu(out.ctx);}
    
    template<typename Out>
    static constexpr bool printBody(Out& out,Ctx&ctx)
      {return ref.printBody(out,ctx);}

    template<typename Out> 
    static constexpr void print(Out& out,Ctx& ctx) 
      {ref.print(out,ctx);}

    template<typename Nav> 
    static constexpr bool nav(Nav& n,const CKE& cke,const Path p) 
      {return ref.nav(n,cke,p);}

    //Id--
    static constexpr int getId() {return -1;}
    template<int i> using HasId=typename R::template HasId<i>;
    template<int i> using WithId=typename R::template WithId<i>;
  };
};

template<typename... OO>
struct Put {
  template<typename Alt,Alt& alt,Clear clr=Clear::no>
  struct ToOut {
    struct End {
      template<typename O>
      struct Part:O {
        using Base=O;
        using Base::Base;
        template<typename Out> static constexpr void print(Out& out,Ctx& ctx) {}
      };
    };
    template<typename O>
    struct Part:Chain<OO...,End>::template Part<O> {
      using Base=typename Chain<OO...,End>::template Part<O>;
      using Base::Base;
      template<typename Out> 
      void print(Out& out,Ctx& ctx) {
        if(!(out.mode()==LockMode::None||out.mode()==LockMode::Update)) return;
        alt.resume();
        if(clr==Clear::yes) alt.clear();
        Base::print(alt,ctx);
        out.resume();
        O::print(out,ctx);
      }
    };
  };
};

template<typename... OO>
struct OnFocus {
  struct End {
    template<typename O>
    struct Part:O {
      using Base=O;
      using Base::Base;
      template<typename Out> static constexpr void print(Out& out,Ctx& ctx) {}
    };
  };
  template<typename O>
  struct Part:Chain<OO...,End>::template Part<O> {
    using Base=typename Chain<OO...,End>::template Part<O>;
      using Base::Base;
    template<typename Out> void print(Out& out,Ctx& ctx) {
      if(ctx) Base::print(out,ctx);
      O::print(out,ctx);
    }
  };
};

// compile-time ids --------------
  /// @brief typelevel/compile-time id
  /// references to items with this type of Id can be retrieved at compile-time
  /// @tparam id depended type int
  template<int id>
  struct Id {
    template<typename I>
    struct Part:I {
      using This=Part<I>;
      using Base=I;
      using Base::Base;
      template<int i> using HasId=std::integral_constant<bool,id==i||Base::template HasId<i>::value>;
      static constexpr int getId() {return id;}
      template<int i>
      using WithId=typename std::conditional<
        id==i,
        This,
        typename Base::template WithId<i>
      >::type;

      template<int i> const std::enable_if_t<i==id,This>& withId() const {return *this;}
      template<int i> std::enable_if_t<i==id,This>& withId() {return *this;}

      template<int i>
      const std::enable_if_t<
        Base::template HasId<i>::value,
        typename Base::template WithId<i>
      >& withId() const {return Base::template withId<i>();}


      template<int i>
      std::enable_if_t<
        Base::template HasId<i>::value,
        typename Base::template WithId<i>
      >& withId() {return Base::template withId<i>();}
    };
  };

#ifdef MENU_DEBUG
#endif
 