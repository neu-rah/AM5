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

struct Nil{};
 
enum class Edge {start,stop};

enum class Fmt {
  View,Menu,Title,Body,Item,
  Cursor,Accel,EditMode,EditCursor,
  Label,Field,Unit,Data
};