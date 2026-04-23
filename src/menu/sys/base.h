/**
 * @file base.h
 * @author Rui Azevedo (ruihfazevedo@gmail.com)
 * @brief 
 * @version 5
 * @date 2026-04-17
 * 
 * @copyright Copyright (c) 2026
 * 
*/

#pragma once

#include <hapi.h>
#include "menu/sys/enums.h"

#ifdef ARDUINO
#include <Arduino.h>
#undef max
#endif

#ifdef __AVR__
  #include <assert.h>
  #include "menu/sys/platform/avr/avr_std.h"
#else
  #include <iostream>
  #include <cstdint>
  #include <cassert>
  #include <type_traits>
  #include <utility>
  #include <cstring>
  #include <cstdlib>
  #include <cstdio>
  #include <limits>
  #include <algorithm>
#endif

using Sz=int;//must be signed
using Depth=char;//must be signed
using Key=int;

/// @brief compile time `max(a,b)` function
/// @tparam a value
/// @tparam b value
/// @return Sz
template<const Sz a,const Sz b> constexpr Sz staticMax() {return a>b?a:b;}

struct Nil{};

struct IItem;
struct IOut;
struct INav;

struct XY{Sz x;Sz y;};
using Pos=XY;
using Area=XY;
 
struct CKE {
  Cmd cmd;
  Key key;
  bool ext;
};

template<typename Cor> struct Colors{Cor fg;Cor bg;};

struct Path {
  Depth len;
  Sz* data;
  operator bool() const {return len>0;}
  Path focus(Depth l) const {assert(l<=len);return {l,(Sz*)&data[0]};}
  Path next() const {assert(len);return Path{(Depth)(len-1),&data[1]};}
  Sz sel() const {return len?data[0]:0;}
  Sz last() const {assert(len-1>=0);return data[len-1];}
};

template<Depth depth>
struct PathData {
  static_assert(depth>0,"expecting this to have at least one item");
  Sz data[depth]{0};
  Path focus(Depth o) {return Path{o,data};}
  Sz sel(Depth level=0) const {return data[(int)level];}
  Path path(Depth from=0) {return Path{depth-from,&data[from]};}
  const Path path(Depth from=0) const {return Path{depth-from,&data[from]};}
  Path path(Depth from,Depth len) {return Path{len,&data[from]};}
  const Path path(Depth from,Depth len) const {return Path{len,&data[from]};}
  operator Path() {return path();}
  operator const Path() const {return path();}
  Sz operator[](Depth i) const {return data[i];}
  Sz& operator[](Depth i) {return data[i];}
};

struct Ctx {
  Path path{};
  NavMode mode{NavMode::Nav};
  Sz printAt{path.len};
  Sz prevSel{0};
  Sz* tops{0};/// out device scroll pos (if used),
  Pad pad{false};
  Sz idx{0};
  bool enabled{true};
  Sz sel() const {return path.sel();}
  Depth len() const {return path.len;}
  Sz top() const {return tops?tops[0]:0;}
  void top(Sz i){if(tops) tops[0]=i;}
  Ctx next() const {return Ctx{path.next(),mode,printAt-1,0,&tops[1],pad,idx,enabled};}
  Ctx(Path p,NavMode nm,Sz pl,Sz ps=0,Sz* t=nullptr,Pad pad=Pad::no,Sz idx=0,bool en=true)
    :path{p},mode{nm},printAt{pl},prevSel{ps},tops{t},pad{pad},idx{idx},enabled{en}{}
  operator bool() const {return path.len>0?idx==path.data[0]:idx==0;}
};

#ifdef MENU_DEBUG
  template<typename Out>
  Out& operator<<(Out& out,const Path o) {
    out<<"{";
    for(Sz i=0;i<o.len;i++) {
      if(i) out<<",";
      out<<o.data[i];
    }
    return out<<"}";
  }

  template<typename Out>
  Out& operator<<(Out& out,const Ctx& o) {
  return out
    <<"path:"<<o.path
    <<" mode:"<<o.mode
    <<" printAt:"<<o.printAt
    <<" prevSel:"<<o.prevSel
    // <<" tops:"<<o.tops
    <<" pad:"<<o.pad
    <<" idx:"<<o.idx
    <<" enabled:"<<o.enabled;
  }
#endif

//rule predicates------------------------------------------
struct IsCursor {
  template<typename Head,typename Base> using Requires=typename Base::IsCursor;
  template<typename Head,typename Base> using Excludes=std::bool_constant<!Requires<Head,Base>::value>;
};

struct RawDevice {
  template<typename Head,typename Base> using Requires=typename Base::RawDevice;
  template<typename Head,typename Base> using Excludes=std::bool_constant<!Requires<Head,Base>::value>;
};

struct IsFormat {
  template<typename Head,typename Base> using Requires=typename Base::IsFormat;
  template<typename Head,typename Base> using Excludes=std::bool_constant<!Requires<Head,Base>::value>;
};

struct IsPrinter {
  template<typename Head,typename Base> using Requires=typename Base::IsPrinter;
  template<typename Head,typename Base> using Excludes=std::bool_constant<!Requires<Head,Base>::value>;
};

struct IsDataParser {
  template<typename Head,typename Base> using Requires=typename Base::IsDataParser;
  template<typename Head,typename Base> using Excludes=std::bool_constant<!Requires<Head,Base>::value>;
};

struct IsParser {
  template<typename Head,typename Base> using Requires=typename Base::IsParser;
  template<typename Head,typename Base> using Excludes=std::bool_constant<!Requires<Head,Base>::value>;
};

struct IsArea {
  template<typename Head,typename Base> using Requires=typename Base::IsArea;
  template<typename Head,typename Base> using Excludes=std::bool_constant<!Requires<Head,Base>::value>;
};

struct IsBuffer {
  template<typename Head,typename Base> using Requires=typename Base::IsBuffer;
  template<typename Head,typename Base> using Excludes=std::bool_constant<!Requires<Head,Base>::value>;
};

//debug ---
#include "menu/sys/debug.h"


