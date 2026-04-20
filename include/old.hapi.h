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
  // template<typename... XX> using Ins=typename ::Chain<XX...,O,OO...>;
  // template<typename... XX> using App=typename ::Chain<O,OO...,XX...>;
  // template<template<typename> class M> using Map=Chain<M<O>,M<OO>...>;
};

template<typename O>
struct Chain<O>:O {
  using O::O;
  template<typename T>
  using Term=typename O::template Part<T>;
  // template<typename... XX> using Ins=typename ::Chain<XX...,O>;
  // template<typename... XX> using App=typename ::Chain<O,XX...>;
  // template<template<typename> class M> using Map=M<O>;
};

template<typename O>
struct CRTP {
  using Obj=O;
  constexpr Obj& obj() {return *(O*)this;}
  constexpr const Obj& obj() const {return *this;}
};

template<typename API,typename... OO> struct APIOf;

template<typename API,typename O,typename... OO>
struct APIOf<API,O,OO... >:Chain<O,OO...>::template Term<API> {
  using Base=typename Chain<O,OO...>::template Term<API>;
  using Base::Base;
  // template<template<typename> class M> using Map=::APIOf<API,M<O>,M<OO>...>;
};

template<typename API>
struct APIOf<API>:API {
  using Base=API;
  using Base::Base;
  // template<template<typename> class M> using Map=M<API>;
};


