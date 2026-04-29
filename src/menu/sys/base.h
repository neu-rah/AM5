/**
 * @file base.h
 * @author Rui Azevedo (neu-rah) (ruihfazevedo@gmail.com)
 * @brief 
 * @version 5
 * @date 2026-04-28
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
#ifdef __AVR__
  using Depth=char;//must be signed
  using Key=unsigned char;
#else
  using Depth=int;//must be signed
  using Key=unsigned int;
#endif 

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
  Sz sel(Depth i=0) const {return data[i];}
  Sz last() const {return sel(len-1);}
};

template<Depth depth> struct PathData {
  Sz data[depth]{0};
  Path focusAt(Depth at)  {assert(at<depth);return {at,data};}
  Sz operator[](Depth i) const {assert(i<depth);return data[i];}
};

struct Ctx {
  Path path{};//full path
  NavMode mode{NavMode::Nav};
  Depth pAt{0};//print level
  bool enabled{true};
  Sz* tops{nullptr};
  //--------
  Depth at{0};
  Sz prev{0};
  bool pad{false};
  Sz idx{0};
  Ctx next() const {return {path,mode,pAt,enabled,tops,at+1,0,pad,0};}
  Sz sel() const {assert(at<path.len);return path.sel(at);}
  Sz top() const {return tops[at];}
  Sz top(Sz i) {return tops[at]=i;}
  operator bool() const {return sel()==idx;}
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
    <<"#"<<o.idx
    <<" path:"<<o.path
    <<" mode:"<<o.mode
    <<" pAt:"<<o.pAt
    <<" en:"<<o.enabled
    // <<" tops:"<<o.tops
    <<" prev:"<<o.prev
    <<" pad:"<<o.pad;
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


