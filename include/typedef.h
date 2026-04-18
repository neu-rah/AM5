/**
 * @file typedef.h
 * @author Rui Azevedo (ruihfazevedo@gmail.com)
 * @brief 
 * @version 5
 * @date 2026-04-17
 * 
 * @copyright Copyright (c) 2026
 * @licence MIT
 */
#pragma once

#include <type_traits>

enum class Wrap {no,yes};

template<typename T>
struct TypeDef {
  using Type=T;
  using Def=std::remove_reference_t<T>;

  template<typename O,Wrap w>
  struct RangeStep:O {
    using O::get;
    using O::set;
    using O::high;
    using O::low;
    using O::O;
    void up(Type o) {high()-get()>o?set(get()+o):set(w==Wrap::yes?low():high());}
    void down(Type o) {get()-low()>o?set(get()-o):set(w==Wrap::yes?high():low());}
  };

  template<typename O,Def l,Def h>
  struct StaticNumRange:O {
    template<Wrap w=Wrap::no>
    using Step=TypeDef::RangeStep<StaticNumRange,w>;
    using O::O;
    static constexpr Def low() {return l;}
    static constexpr Def high() {return h;}
    static constexpr bool chk(const Def& o) {return l<=o&&o<=h;}
    static constexpr Def validate(const Def& o) {return o<=l?l:o>=h?h:o;}
    constexpr bool chk() const {return chk(O::get());}
    constexpr void valid() {O::set(validate(O::get()));}
  };

  template<Type data>
  struct Static {
    template<Def l,Def h> using StaticNumRange=TypeDef::StaticNumRange<Static,l,h>;
    static Type get() {return data;}
    operator Type() {return get();}
  };
  template<Type& data>
  struct Ref {
    template<Def l,Def h> using StaticNumRange=TypeDef::StaticNumRange<Ref,l,h>;
    static constexpr Type& get() {return data;}
    static constexpr void set(const Type& o) {data=o;}
    constexpr operator Type&() {return get();}
  };
  struct Value {
    Def data;
    constexpr Value():data{}{}
    constexpr Value(Def o):data{o}{}
    template<Type l,Type h> using StaticNumRange=TypeDef::StaticNumRange<Value,l,h>;
    constexpr Type& get() {return data;}
    constexpr Type& get() const {return data;}
    constexpr void set(const Type& o) {data=o;}
    constexpr operator Type&() {return get();}
    constexpr operator Type&() const {return get();}
  };
};
