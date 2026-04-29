/**
 * @file menu.h
 * @author Rui Azevedo (neu-rah) (ruihfazevedo@gmail.com)
 * @brief 
 * @version 5
 * @date 2026-04-28
 * 
 * @copyright Copyright (c) 2026
 * 
*/

#pragma once

#include "menu/sys/base.h"
#include "menu/body/staticBody.h"
#include "menu/sys/data.h"
#include "menu/sys/printers.h"
#include "menu/in.h"
#include "menu/out.h"
#include "menu/item.h"
#include "menu/menu.h"
#include "menu/nav.h"

template<typename... OO> constexpr 
StaticBody<OO...> staticBody(OO&&... oo)
  {return StaticBody<OO...>{std::forward<OO>(oo)...};}

template<Wraps wraps,typename T,typename B> 
constexpr MenuDef<T,B,wraps,Pad::no> menuDef(T&& t,B&& b)
  {return {std::forward<T>(t),std::forward<B>(b)};}

template<typename T,typename B,Wraps w=Wraps::no> 
constexpr PadMenu<T,B,w> padDef(T&& t,B&& b)
  {return {std::forward<T>(t),std::forward<B>(b)};}

