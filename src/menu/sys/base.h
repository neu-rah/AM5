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
  Path next() {return Path{len-1,&path[1]};}
};

template<Depth depth>
struct PathData {
  Sz data[depth]{0};
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
  Path path;
  Sz idx;
};

