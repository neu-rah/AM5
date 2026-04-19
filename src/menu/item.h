/**
 * @file item.h
 * @author Rui Azevedo (ruihfazevedo@gmail.com)
 * @brief 
 * @version 5
 * @date 2026-04-17
 * 
 * @copyright Copyright (c) 2026
 * 
*/

#pragma once

#include "menu/sys/base.h"

template<typename Def>
struct ItemAPI:Def {
  static constexpr Sz depth() {return 1;}
  static constexpr bool enabled() {return true;}
  static constexpr void enable(bool=true) {}
  static constexpr bool changed() {return false;}
  static constexpr void sync() {}
  template<typename Out> static constexpr bool printMenu(Out&,Ctx&) {return false;}
  template<typename Out> static constexpr bool printBody(Out&,Ctx&) {return false;}
  template<typename Out> static constexpr bool printItem(Out&,Ctx&) {return false;}
  template<typename Out> static constexpr void print(Out&) {}
  static constexpr void nav(CKE cke,Path) {}

  // template <typename Out>
  // static constexpr bool printMenu(Out& out,Ctx& ctx) {return false;}
  
  // template<typename Out>
  // static constexpr bool printBody(Out& out,Ctx&) {return false;}

  // template<typename Out> 
  // static constexpr void print(Out& out,Ctx& ctx) {}

  template<bool kbd,typename Nav> 
  static constexpr bool nav(Nav& n,const CKE& cke,const Path p) 
    {return false;}

};

template<typename... OO>
struct ItemDef:APIOf<ItemAPI<Nil>,OO...> {
  using Base=APIOf<ItemAPI<Nil>,OO...>;
  using Base::Base;
  using Base::printMenu;
  using Base::nav;
  template<typename Out> void printMenu(Out& out,Ctx&& ctx) {Base::printMenu(out,ctx);}
  void enter(Path path) {nav({Cmd::Enter},path);}
};

struct IItem {
  // virtual Depth depth() const {return 0;}
  virtual bool printMenu(IOut& out,Ctx& ctx) {return false;}
  virtual bool printBody(IOut& out,Ctx&) {return false;}
  virtual void print(IOut& out,Ctx& ctx) {}
  virtual bool enabled() const {return true;}
  virtual void enable(bool=true) {}
  virtual bool changed() const {return false;}
  virtual bool changed(IOut& out) const {return false;}
  virtual void sync() const {}
  virtual void sync(IOut& out) {}
  virtual bool up() const {return false;}
  virtual bool down() const {return false;}
  virtual bool nav(bool kbd,INav& n,const CKE& cke,const Path p) {return false;}

  template <typename Out>
  static constexpr bool printMenu(Out& out,Ctx& ctx) 
    {return printMenu(out,ctx);}

  template<bool kbd>
  bool nav(INav& n,const CKE& cke,const Path p) {
    return kbd?
      nav(true,n,cke,p):
      nav(false,n,cke,p);
  }
};

template<typename... II>
struct IItemDef:IItem, ItemDef<II...> {
  using Base=ItemDef<II...>;
  using Base::Base;

  // virtual Depth depth() const override {return Base::depth();}
  virtual bool changed() const override {return Base::changed();}
  virtual void sync() const override {Base::sync();}
  virtual bool printMenu(IOut& out,Ctx& ctx) override {return Base::printMenu(out,ctx);}
  virtual bool printBody(IOut& out,Ctx& ctx) override {return Base::printBody(out,ctx);}
  virtual void print(IOut& out,Ctx& ctx) override {Base::print(out,ctx);}
  template <typename Out> static constexpr bool printMenu(Out& out,Ctx& ctx) {return Base::printMenu(out,ctx);}
  template<typename Out> static constexpr void print(Out& out,Ctx& ctx) {return Base::print(out,ctx);}

  template<bool kbd>
  bool nav(INav& n,const CKE& cke,const Path p) {
    return kbd?
      nav(true,n,cke,p):
      nav(false,n,cke,p);
  }
  //Id--
  static constexpr int getId() {return -1;}
  template<int> using HasId=std::false_type;
  template<int> using WithId=ItemAPI<CRTP<ItemAPI<Nil>>>;
};

//---------------------------------------------------------------------------------------------
// template<typename T>
// struct Data {
//   template<typename O>
//   struct Part:T,O {
//     using Base=O;
//     using Base::Base;
//     using Def=typename T::Def;
//     using Type=typename T::Type;
//     auto get() {return T::get();}
//     template<typename... OO>
//     Part(const Def& def,OO&&... oo):T{def},Base{std::forward<OO>(oo)...}{}
//     template<typename Out>
//     void print(Out& out) {
//       Out::fmtStart(Fmt::Data,{});
//       out.put(get());
//       Out::fmtStop(Fmt::Data,{});
//       Base::print(out);
//     }
//     static constexpr void nav(CKE cke,Path path) {
//       switch(cke.cmd) {
//         case Cmd::Up: T::up();break;
//         case Cmd::Down: T::down();break;
//       }
//       Base::nav(cke,path);
//     }
//   };
// };

// using Text=Data<typename TypeDef<const char*>::Value>;
// using Int=Data<typename TypeDef<int>::Value>;
// template<int& o> using IntRef=Data<typename TypeDef<int>::template Ref<o>>;
// // using Float=Data<TypeDef<double>::Value>;

using ActionFunc=void(*)(int);

template<ActionFunc action>
struct Action {
  template<typename O>
  struct Part:O {
    using Base=O;
    static constexpr void nav(CKE cke,Path path) {
      if(cke.cmd==Cmd::Enter) action(path.sel());
      Base::nav(cke,path);
    }
  };
};




