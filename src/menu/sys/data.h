/**
 * @file data.h
 * @author Rui Azevedo (neu-rah) (ruihfazevedo@gmail.com)
 * @brief data API
 * @version 5
 * @date 2026-04-19
 * 
 * @copyright Copyright (c) 2026
 * 
*/

#pragma once

#include "menu/sys/base.h"

using CText=const char*;

template<typename O=Nil> 
struct DataAPI:O {
  using Base=O;
  using Base::Base;
  static constexpr bool changed() {return false;}
  static constexpr void sync() {}
};

template<typename... OO>
struct DataDef:APIOf<DataAPI<>,OO...> {
  using Base=APIOf<DataAPI<>,OO...>;
  using Base::Base;
};

//compile-time data --
template<typename T,T data>
struct StaticData {
  template<typename O>
  struct Part:O {
    using Type=T;
    using Base=O;
    using Base::Base;
    static constexpr T get() {return data;} 
    static constexpr void set(const Type& o) {data=o;}
    constexpr operator Type&() {return get();}
    constexpr operator const Type&() const {return get();}
    template<typename Out> void print(Out& out,Ctx& ctx) const {
      out.template fmtStart<Fmt::Data>(ctx);
      out.put(get());
      out.template fmtStop<Fmt::Data>(ctx);
      ;Base::print(out,ctx);
    }
  };
};

//runtime data --
template<typename T,T& data>
struct StaticRef {
  template<typename O>
  struct Part:O {
    using Type=T;
    using Base=O;
    using Base::Base;
    static constexpr T& get() {return data;} 
    static constexpr void set(const Type& o) {data=o;}
    operator Type&() {return get();}
    operator Type&() const {return get();}
    template<typename Out> void print(Out& out,Ctx& ctx) {
      out.template fmtStart<Fmt::Data>(ctx);
      out.put(get());
      out.template fmtStop<Fmt::Data>(ctx);
      Base::print(out,ctx);
    }
  };
};

template<const CText& text>
using StaticText=StaticRef<const CText,text>;

template<typename T>
struct Data {
  template<typename O>
  struct Part:O {
    using Type=T;
    using Base=O;
    using Base::Base;
    Type data;
    template<typename... OO> Part(const Type& o,OO&&... oo):Base{std::forward<OO>(oo)...},data{o}{}
    // template<typename... OO> Part(OO&&... oo):Base{std::forward<OO>(oo)...}{}
    const Type& get() const {return data;} 
    void set(const Type& o) {data=o;}
    operator Type&() {return data;}
    operator const Type&() const {return data;}
    template<typename Out> void print(Out& out,Ctx& ctx) {
      out.template fmtStart<Fmt::Data>(ctx);
      out.put(data);
      out.template fmtStop<Fmt::Data>(ctx);
      Base::print(out,ctx);
    }
  };
};

// template<const char*& text> using Text=StaticRef<const char*,text>;
using Text=Data<const char*>;
using Bool=Data<bool>;
using Int=Data<int>;

template<typename W>
struct Watch {
  template<typename O>
  struct Part:W::template Part<O> {
    using Base=typename W::template Part<O>;
    using Type=typename Base::Type;
    using Base::get;
    using Base::Base;
    std::remove_reference_t<Type> watched;
    constexpr bool changed() const {return get()!=watched/*||Base::changed()*/;}
    void sync() {watched=get();}
  };
};

template<typename N,N l,N h,Wraps w>
struct StaticNumRange {
  template<typename O>
  struct Part:O {
    // static constexpr const Wraps wraps{w};
    using Base=O;
    using Base::get;
    using Base::set;
    using Type=typename Base::Type;
    using Base::Base;
    static constexpr bool valid(N o) {return o<=l&&o<=h;}
    static constexpr N clamp(N o) {return o<l?l:o>h?h:o;}
    static constexpr N stepUp(N o,N s) {return h-o>s?o+s:w==Wraps::yes?l:h;}
    static constexpr N stepDown(N s,N o) {return o-l>s?o-s:w==Wraps::yes?h:l;}
    static constexpr N step(N s,N o) {return s<0?stepDown(-s,o,w):stepUp(s,o,w);}
    void up(N s=1) {set(stepUp(s,get()));}
    void down(N s=1) {set(stepDown(s,get()));}
  };
};

template<typename N>
struct NumRange {
  template<typename O>
  struct Part:O {
    using Base=O;
    using Base::get;
    using Type=N;
    Type m_low;
    Type m_high;
    bool wraps;
    template<typename... OO>
    Part(Type l,Type h,bool w,OO&&... oo)
      :Base{std::forward<OO>(oo)...},m_low{l},m_high{h},wraps{w}{}
    constexpr bool valid(N o) const {return o<=m_low&&o<=m_high;}
    constexpr N clamp(N o) const {return o<m_low?m_low:o>m_high?m_high:o;}
    constexpr N stepUp(N o,N s) {return m_high-o>=s?o+s:wraps?m_low:m_high;}
    constexpr N stepDown(N s,N o) {return o-m_low>=s?o-s:wraps?m_high:m_low;}
    void up(N s=1) {get()=stepUp(s,get());}
    void down(N s=1) {get()=stepDown(s,get());}
  };
};

template<typename T,T def_val>
struct Default {
  template<typename I>
  struct Part:I {
    using Base=I;
    template<typename... OO> constexpr Part(OO... oo):I{def_val,oo...}{}
    template<typename... OO> constexpr Part(decltype(def_val) o, OO... oo):I{o,oo...}{}
  };
};
