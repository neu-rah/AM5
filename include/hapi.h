/**
 * @file hapi.h
 * @author Rui Azevedo (neu-rah) (ruihfazevedo@gmail.com)
 * @brief 
 * @version 5
 * @date 2026-04-17
 * 
 * @copyright Copyright (c) 2026
 * 
*/

#pragma once

template<typename O,typename... OO>
struct Chain:O {
  using O::O;
  template<typename T>
  using Term=typename O::template Part<typename Chain<OO...>::template Term<T>>;
};

template<typename O>
struct Chain<O>:O {
  using O::O;
  template<typename T>
  using Term=typename O::template Part<T>;
};

template<typename O>
struct CRTP {
  using Obj=O;
  constexpr Obj& obj() const {return *this;}
};

template<typename API,typename... OO>
using APIOf=typename Chain<OO...>::template Term<API>;

