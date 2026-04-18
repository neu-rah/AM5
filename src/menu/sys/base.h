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
#include <typedef.h>

using Sz=int;
using Depth=char;
using Key=int;

struct Nil{};
 
enum class Edge {start,stop};

enum class Fmt {
  View,Menu,Title,Body,Item,
  Cursor,Accel,EditMode,EditCursor,
  Label,Field,Unit,Data
};

enum class Cmd {
  Esc,Enter,
  Up,Down,Left,Right,
  Key, Go
};

struct CKE {
  Cmd cmd;
  Key key;
  bool ext;
};

struct Path {
  TypeDef<int>::Value len;
  TypeDef<Sz*>::Value path;
};

template<Depth n>
using PathType
  = typename TypeDef<Sz>
  ::Value
  ::template StaticNumRange<0,n>
  ::template Step<Wrap::no>;

template<Depth n>
struct PathData:PathType<n> {
  auto path(Depth from,Depth len) {return {len,&PathType<n>::get()[from]};}
};
