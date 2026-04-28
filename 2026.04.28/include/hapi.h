/**
 * @file hapi.h
 * @author Rui Azevedo (ruihfazevedo@gamil.com)
 * @brief Happy API - Composition API build tools.
 * @version 5
 * @date 2026-04-14
 * 
 * @licence MIT
 */

#pragma once

#ifdef __AVR__
  #include <menu/sys/platform/avr/avr_std.h>
#else
  #include <type_traits>
#endif

//HAPI--
// non empty composition
template<typename O,typename... OO>
struct Chain {
  template<typename T>
  struct Part:O::template Part<typename Chain<OO...>::template Part<T>> {
    using Base=typename O::template Part<typename Chain<OO...>::template Part<T>>;
    using Base::Base;
    template<typename R> using Requires= typename R::template Requires<O,Base>;
    template<typename R> using Excludes= typename R::template Excludes<O,Base>;
  };
  template<typename... XX> using Ins=typename ::Chain<XX...,O,OO...>;
  template<typename... XX> using App=typename ::Chain<O,OO...,XX...>;
  template<template<typename> class M> using Map=Chain<M<O>,M<OO>...>;
};

template<typename O>
struct Chain<O> {
  template<typename T>
  struct Part:O::template Part<T>{
    using Base=typename O::template Part<T>;
    using Base::Base;
    template<typename R> using Requires=typename R::template Requires<O,Base>;
    template<typename R> using Excludes=typename R::template Excludes<O,Base>;
  };
  template<typename... XX> using Ins=typename ::Chain<XX...,O>;
  template<typename... XX> using App=typename ::Chain<O,XX...>;
  template<template<typename> class M> using Map=M<O>;
};

template<typename API,typename... OO> struct APIOf;

template<typename API,typename O,typename... OO>
struct APIOf<API,O,OO... >:Chain<O,OO...>::template Part<API> {
  using Base=typename Chain<O,OO...>::template Part<API>;
  using Base::Base;
  template<template<typename> class M> using Map=::APIOf<API,M<O>,M<OO>...>;
};

template<typename API>
struct APIOf<API>:API {
  using Base=API;
  using Base::Base;
  template<template<typename> class M> using Map=M<API>;
};

template<typename O> 
struct CRTP {
  using Obj=O;
  Obj& obj() {return *(O*)this;}
  const Obj& obj() const {return *this;}
};

//rule predicates------------------------------------------

//rules system, requires/excludes simple class (will use std::same_as<>) from the derivation chain
template<typename O> struct Class {
  template<typename Head,typename Base> using Requires=std::bool_constant<std::is_same<O,Head>::value||Base::template Requires<Class<O>>::value>;
  template<typename Head,typename Base> using Excludes=std::bool_constant<!Requires<Head,Base>::value>;
};

