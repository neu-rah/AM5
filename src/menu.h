/**
 * @file menu.h
 * @author Rui Azevedo (ruihfazevedo@gmail.com)
 * @brief 
 * @version 5
 * @date 2026-04-17
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#pragma once

#include "menu/sys/enums.h"
#include "menu/sys/base.h"
#include "menu/sys/data.h"
#include "menu/sys/fields.h"
#include "menu/sys/printers.h"
#include "menu/sys/staticBody.h"
#include "menu/out.h"
#include "menu/item.h"
#include "menu/menu.h"
#include "menu/nav.h"

template<typename... OO> constexpr 
StaticBody<OO...> staticBody(OO&&... oo)
  {return StaticBody<OO...>{std::forward<OO>(oo)...};}

template<Wraps wraps,typename T,typename B> 
constexpr MenuDef<T,B,wraps> menuDef(T&& t,B&& b)
  {return MenuDef<T,B,wraps>{std::forward<T>(t),std::forward<B>(b)};}

