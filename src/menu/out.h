/**
 * @file out.h
 * @author Rui Azevedo (neu-rah) (ruihfazevedo@gmail.com)
 * @brief 
 * @version 5
 * @date 2026-04-17
 * 
 * @copyright Copyright (c) 2026
 * 
*/

#pragma once

template<typename Cfg=Nil>
struct OutAPI:Cfg {
  using Config=Cfg;
  using IsBuffer=std::false_type;
  using IsArea=std::false_type;
  using IsDataParser=std::false_type;
  using IsParser=std::false_type;
  using IsFormat=std::false_type;
  using IsPrinter=std::false_type;
  using RawDevice=std::false_type;
  using IsCursor=std::false_type;
  template<typename> using Requires=std::false_type;
  template<typename> using Excludes=std::true_type;
  using Cfg::obj;
  static constexpr Sz freeX() {return std::numeric_limits<Sz>::max();}
  static constexpr Sz freeY() {return std::numeric_limits<Sz>::max();}
  static constexpr Area free() {return {freeX(),freeY()};}
  static constexpr Sz posX() {return 0;}
  static constexpr Sz posY() {return 0;}
  static constexpr Pos pos() {return {posX(),posY()};}
  static void setPos(Sz,Sz) {}
  static void setPos(Pos p) {setPos(p.x,p.y);}
  static constexpr Sz orgX() {return 0;}
  static constexpr Sz orgY() {return 0;}
  static constexpr Pos org() {return {orgX(),orgY()};}
  static constexpr LockMode mode() {return LockMode::None;}
  static constexpr void mode(LockMode) {}
  template<typename Cor> static void setColors(Cor,Cor) {}

  static constexpr void clear() {}
  static constexpr void nl() {}
  static constexpr void flush() {}
  static constexpr void resume() {}
  template<typename T> static constexpr void put(const T&) {}
  template<Fmt tag> static constexpr void fmtStart(const Ctx& ctx) {}
  template<Fmt tag> static constexpr void fmtStop(const Ctx& ctx) {}
  template<typename Item> static constexpr bool printItem(Item& item,Ctx& ctx) {return false;}
  template<typename Item> static constexpr bool printMenu(Item& item,Ctx& ctx) {return false;}
};

template<typename API,typename... OO> struct DefinedOut;

template<typename API,typename O,typename... OO>
struct DefinedOut<API,O,OO...>:APIOf<API,O,OO...>{
  using Base=APIOf<API,O,OO...>;
  using Base::printItem;
  using Base::obj;
  // static_assert(Base::template Excludes<IsCursor>::value||Base::template Requires<IsDataParser>::value,"Cursor requires preseeding DataParser<>");
  // static_assert(Base::template Excludes<Class<class Clip>>::value||Base::template Requires<IsDataParser>::value,"Clip requires preseeding DataParser<>");
  // static_assert(Base::template Excludes<Class<class TexWrap>>::value||Base::template Requires<IsDataParser>::value,"TextWrap requires preseeding DataParser<>");
  // static_assert(Base::template Excludes<Class<class UTF8>>::value||Base::template Requires<IsDataParser>::value,"UTF8 requires preseeding DataParser<>");
  template<typename Item>
  bool printTitle(const Item& item,Ctx& ctx){
    Base::fmtStart(Fmt::Title,ctx);
    item.print(obj(),ctx);
    Base::fmtStop(Fmt::Title,ctx);
    return item.changed();
  }
};

template<typename API>
struct DefinedOut<API>:APIOf<API>
  {using Base=APIOf<API>;};

template<typename... OO> struct OutDef:DefinedOut<OutAPI<CRTP<OutDef<OO...>>>,OO...>{};

struct IOut {
  virtual void mode(LockMode) {}
  virtual LockMode mode() {return LockMode::None;}
  virtual void fmtStart(Fmt,Ctx&) {}
  virtual void fmtStop(Fmt,Ctx&) {}
  virtual Sz posX() const {return 0;}
  virtual Sz posY() const {return 0;}
  Pos pos() const {return {posX(),posY()};}
  virtual void setPos(Pos) {}
  virtual void put(const int) {}
  virtual void put(const double) {}
  virtual void put(const char) {}
  virtual void put(const char*) {}
  virtual void put(const char* const*) {}
  virtual void put(const char* const*& str) {}
  // virtual bool printItem(IItem& item,Ctx& ctx) {return false;}
  // virtual bool printMenu(IItem& item,Ctx& ctx) {return false;}
  // template<typename I> static constexpr bool printItem(I& item,Ctx& ctx) {return printItem(*reinterpret_cast<IItemDef<I>*>(&item),ctx);}
};

template<typename... OO>
struct IOutDef:IOut,DefinedOut<OutAPI<CRTP<IOutDef<OO...>>>,OO...>{
  using IOut::pos;
  using Base=DefinedOut<OutAPI<CRTP<IOutDef<OO...>>>,OO...>;
  virtual void mode(LockMode m) {Base::mode(m);}
  virtual LockMode mode() {return Base::mode();}
  virtual void fmtStart(Fmt tag,Ctx& ctx) override {Base::fmtStart(tag,ctx);}
  virtual void fmtStop(Fmt tag,Ctx& ctx) override {Base::fmtStop(tag,ctx);}
  virtual Sz posX() const override {return Base::posX();}
  virtual Sz posY() const override {return Base::posY();}
  virtual void setPos(Pos p) override {Base::setPos(p);}
  virtual void put(const int n) override {Base::put(n);}
  virtual void put(const double n) override {Base::put(n);}
  virtual void put(const char c) override {Base::put(c);}
  virtual void put(const char* str) override {Base::put(str);}
  virtual void put(const char* const* str) override {Base::put(str);}
  virtual void put(const char* const*& str) override {Base::put(str);}
  // virtual bool printItem(IItem& item,Ctx& ctx) override {return Base::printItem(item,ctx);}
  // virtual bool printMenu(IItem& item,Ctx& ctx) override {return Base::printMenu(item,ctx);}
  // template<typename I> static constexpr bool printItem(I& item,Ctx& ctx) {return printItem(*reinterpret_cast<IItemDef<I>*>(&item),ctx);}
};

template<typename... OO,typename T> 
DefinedOut<OO...>& operator<<(DefinedOut<OO...>& out,const T& o) {out.put(o);return out;}

template<template<typename...> class T,typename... NN,typename... OO> 
DefinedOut<OO...>& operator<<(DefinedOut<OO...>& out,const T<NN...> o) {o.printItem(out);return out;}

template<typename... OO> DefinedOut<OO...>& endl (DefinedOut<OO...>& s) {s.nl();return s;}

template<typename... OO> DefinedOut<OO...>& flush(DefinedOut<OO...>& s) {s.flush();return s;}

template<Sz x,Sz y,typename... OO> DefinedOut<OO...>& xy(DefinedOut<OO...>& s) {s.xy(x,y);return s;}

template<size_t n,typename... OO> 
DefinedOut<OO...>& operator<<(DefinedOut<OO...>& out,const char t[n]){for(int i=0;i<n;i++) out.put(t[i]);}

template<typename... OO> 
DefinedOut<OO...>& operator<<(DefinedOut<OO...>& out,DefinedOut<OO...>&(f)(DefinedOut<OO...>&))
  {f(out);return out;}

template<typename Dev,Dev& dev>
struct StreamOut {
  template<typename O>
  struct Part:O {
    using Base=O;
    static constexpr void nl() {endl(dev);}
    template<typename T> static constexpr void put(const T& o) {dev<<o;}
  };
};

#ifndef __AVR__
  #include <iostream>
  using ConsoleOut=StreamOut<decltype(std::cout),std::cout>;
#endif

// struct ViewPrinter {
//   template<typename O>
//   struct Part:O {
//     using Base=O;
//     void printView(Ctx& ctx) {
//       Base::template fmt<Edge::start,Fmt::View>(ctx);
//       Base::printView(ctx);
//       Base::template fmt<Edge::stop,Fmt::View>(ctx);
//     }
//   };
// };

// /// @brief groups some printer parts to form menu
// /// @tparam ...OO the body parts
// template<typename... OO>
// struct MenuPrinter {
//   template<typename O>
//   struct Part:Chain<OO...>::template Term<O> {
//     using IsPrinter=std::true_type;
//     using Base=typename Chain<OO...>::template Term<O>;
//     using Base::fmtStart;
//     using Base::fmtStop;
//     using Base::obj;
//     template<typename I>
//     bool printMenu(I& i,Ctx& ctx) {
//       ctx.idx=0;
//       Base::template fmtStart<Fmt::Menu>(ctx);
//       // i.printTo(Base::obj());
//       bool r=Base::printMenu(i,ctx);
//       Base::template fmtStop<Fmt::Menu>(ctx);
//       return r;
//     }
//   };
// };

// /// @brief print the title + format
// struct TitlePrinter {
//   template<typename O>
//   struct Part:O {
//     using IsPrinter=std::true_type;
//     using Base=O;
//     template<typename I>
//     bool printMenu(I& i,Ctx& ctx) {
//       O::template fmtStart<Fmt::Title>(ctx);
//       i.print(O::obj());//,ctx);//title
//       O::template fmtStop<Fmt::Title>(ctx);
//       return O::printMenu(i,ctx);
//     }
//   };
// };

// /// @brief start body printing process by redirecting to the item
// struct BodyPrinter {
//   template<typename O>
//   struct Part:O {
//     using IsPrinter=std::true_type;
//     using Base=O;
//     using Base::obj;
//     template<typename I>
//     bool printMenu(I& i,Ctx& ctx) {
//       Base::template fmtStart<Fmt::Body>(ctx);
//       bool r=i.printBody(O::obj(),ctx);
//       Base::template fmtStop<Fmt::Body>(ctx);
//       return r;
//     }
//   };
// };

// /// @brief print scroll menu body
// struct ScrollBodyPrinter {
//   template<typename P>
//   struct Part:BodyPrinter::Part<P> {
//     using IsPrinter=std::true_type;
//     using Base=typename BodyPrinter::Part<P>;
//     using Base::mode;
//     using Base::pos;
//     using Base::freeY;
//     using Base::setPos;

//     template<typename I>
//     void printMenu(I& i,Ctx& ctx) {
//       LockMode om=mode();
//       Sz x=Base::posX();
//       Sz y=Base::posY();
//       if(ctx.sel()<ctx.top()) {
//         ctx.top(ctx.sel());//--scroll down
//         om=LockMode::None;//scroll => full redraw
//       } else for(;;) {
//         mode(LockMode::Measure);
//         Base::printMenu(i,ctx);//body measure
//         Sz f=freeY();
//         Sz ci=ctx.idx;
//         ctx.idx=0;
//         if(ctx.sel()<ci&&(!(ctx.sel()==(ci-1)&&f<0))) break;
//         setPos(x,y);
//         ctx.top(ctx.top()+1);//--scroll up
//         om=LockMode::None;//scroll => full redraw
//       };
//       mode(om);
//       setPos(x,y);
//       Base::printMenu(i,ctx);
//     }
//     template<typename I>
//     void printItem(I& i,Ctx& ctx) {
//       if(ctx.idx<ctx.top()) ctx.idx++;
//       else Base::freeY()>0?Base::printItem(i,ctx):false;
//     }
//   };
// };

// /// @brief groups some printer parts to form a item body, will be formatted as a item
// /// also checks LockMode and act appropriately
// template<typename... OO>
// struct ItemPrinter {
//   template<typename O>
//   struct Part:Chain<OO...>::template Term<O> {
//     using IsPrinter=std::true_type;
//     using Base=typename Chain<OO...>::template Term<O>;
//     using Base::fmtStart;
//     using Base::fmtStop;
//     using Base::mode;
//     using Base::setPos;
//     using Base::posX;
//     using Base::posY;
//     template<typename I>
//     void printItem(I& i,Ctx& ctx) {
//       if(mode()==LockMode::Update
//         &&(i.changed()||(ctx.prevSel!=ctx.sel()&&(ctx.idx==ctx.prevSel||ctx.idx==ctx.sel())))
//       ) {
//         mode(LockMode::None);
//         setPos({posX(),posY()});
//       }
//       ctx.enabled=i.enabled();
//       Base::template fmtStart<Fmt::Item>(ctx);
//       Base::printItem(i,ctx);
//       Base::template fmtStop<Fmt::Item>(ctx);
//       ctx.idx++;
//       if(mode()==LockMode::Sync) i.sync();
//     }
//   };
// };

// /// @brief print the item
// struct PrintItem {
//   template<typename O>
//   struct Part:O {
//     using IsPrinter=std::true_type;
//     using Base=O;
//     template<typename I>
//     void printItem(I& i,Ctx& ctx) {
//       i.print(Base::obj());
//       Base::printItem(i,ctx);
//     }
//   };
// };

