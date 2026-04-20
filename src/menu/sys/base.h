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


using Sz=int;
using Depth=char;
using Key=int;

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
  operator bool() const {return len==0;}
  Path focus(Depth l) const {assert(l<=len);return {l,(Sz*)&data[0]};}
  Path next() const {return Path{(Depth)(len-1),&data[1]};}
  Sz sel() const {return len?data[0]:0;}
  Sz last() const {return data[len-1];}
};

template<Depth depth>
struct PathData {
  Sz data[depth]{0};
  Path focus(Depth o) {return Path{o,data};}
  Sz sel(Depth level=0) const {return data[level];}
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
  Sz idx{0};
  bool enabled{true};
  Sz sel() const {return path.sel();}
  Sz len() const {return path.len;}
  Sz top() const {return tops?tops[0]:0;}
  void top(Sz i){if(tops) tops[0]=i;}
  Ctx next() const {return Ctx{path.next(),mode,printAt-1,0,&tops[1]};}
  // Ctx(Path p,NavMode nm,Sz pl,Sz ps=0,Sz* t=nullptr):path{p},mode{nm},printAt{pl},prevSel{ps},tops{t}{}
  operator bool() const {return path.len>0?idx==path.data[0]:idx==0;}
};

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


