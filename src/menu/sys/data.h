/**
 * @file data.h
 * @author Rui Azevedo (neu-rah)
 * @brief Data API - HAPI data components
 * @version 2026-05
 */

#pragma once

#include "menu/sys/base.h"
#include <type_traits>
#include <utility>

using CText = const char *;

// ====================== Base ======================
template <typename O = Nil>
struct DataAPI : O {
  using Base = O;
  using Base::Base;

  static constexpr bool changed() { return false; }
  static constexpr void sync() {}
};

template <typename... OO>
struct DataDef : APIOf<DataAPI<>, OO...> {
  using Base = APIOf<DataAPI<>, OO...>;
  using Base::Base;
};

// ====================== Static Data ======================
template <typename T, T Value>
struct StaticData
{
  template <typename O>
  struct Part : O
  {
    using Base = O;
    using Type = T;

    static constexpr T get() { return Value; }
    // static constexpr void set(T v) { /* optional: ignore or compile error */ }

    constexpr operator Type() const { return get(); }

    template <typename Out, typename Ctx>
    void print(Out &out, Ctx &ctx) const {
      out.template fmtStart<Fmt::Data>(ctx);
      out.put(get());
      out.template fmtStop<Fmt::Data>(ctx);
      Base::print(out, ctx);
    }
  };
};

// ====================== Static Reference ======================
template <typename T, T &Ref>
struct StaticRef
{
  template <typename O>
  struct Part : O
  {
    using Base = O;
    using Type = T;

    static constexpr T &get() { return Ref; }
    static constexpr void set(const T &v) { Ref = v; }

    operator Type &() { return get(); }
    operator const Type &() const { return get(); }

    template <typename Out, typename Ctx>
    void print(Out &out, Ctx &ctx)
    {
      out.template fmtStart<Fmt::Data>(ctx);
      out.put(get());
      out.template fmtStop<Fmt::Data>(ctx);
      Base::print(out, ctx);
    }
  };
};

template <const CText &Text>
using StaticText = StaticRef<const CText, Text>;

// ====================== Runtime Data ======================
template <typename T>
struct Data {
  template <typename O>
  struct Part : O {
    using Base = O;
    using Type = T;

    Type data;

    // Perfect forwarding constructor
    template <typename... OO>
    constexpr Part(Type value, OO &&...oo)
      : Base{std::forward<OO>(oo)...}, data{std::move(value)} {}

    const Type &get() const { return data; }
    // void set(const Type &v) { data = v; }
    void set(Type &&v) { data = std::move(v); }

    operator Type &() { return data; }
    operator const Type &() const { return data; }

    template <typename Out, typename Ctx>
    void print(Out &out, Ctx &ctx) {
      out.template fmtStart<Fmt::Data>(ctx);
      out.put(data);
      out.template fmtStop<Fmt::Data>(ctx);
      Base::print(out, ctx);
    }
  };
};

// ====================== Watch ======================
template <typename W>
struct Watch {
  template <typename O>
  struct Part : W::template Part<O> {
    using Base = typename W::template Part<O>;
    using Type = typename Base::Type;
    using Base::get;
    std::remove_reference_t<Type> watched;
    constexpr bool changed() const {return get() != watched;}
    void sync() {watched = get();}
  };
};

// ====================== Number Range ======================
template <typename N>
struct NumRange {
  template <typename O>
  struct Part : O {
    using Base = O;
    using Type = N;
    using Base::get;

    Type m_low, m_high;
    bool wraps;

    template <typename... OO>
    constexpr Part(Type low, Type high, bool w, OO &&...oo)
      : Base{std::forward<OO>(oo)...}, m_low{low}, m_high{high}, wraps{w} {}

    constexpr bool valid(Type v) const { return v >= m_low && v <= m_high; }
    constexpr Type clamp(Type v) const { return v < m_low ? m_low : v > m_high ? m_high : v; }

    void up(Type step = 1) { set(clamp(get() + step)); }
    void down(Type step = 1) { set(clamp(get() - step)); }
  };
};

template <typename N, N Low, N High, bool Wraps = false>
struct StaticNumRange {
  template <typename O>
  struct Part : O {
    using Base = O;
    using Type = N;
    using Base::get;

    static constexpr bool valid(N v) { return v >= Low && v <= High; }
    static constexpr N clamp(N v) { return v < Low ? Low : v > High ? High : v; }

    void up(N step = 1) { set(clamp(get() + step)); }
    void down(N step = 1) { set(clamp(get() - step)); }
  };
};

// ====================== Default ======================
template <typename T, T DefaultValue>
struct Default {
  template <typename O>
  struct Part : O {
    using Base = O;

    template <typename... OO>
    constexpr Part(OO &&...oo)
      : Base{DefaultValue, std::forward<OO>(oo)...} {}

    template <typename... OO>
    constexpr Part(T value, OO &&...oo)
      : Base{value, std::forward<OO>(oo)...} {}
  };
};

// ====================== Aliases ======================
using Text = Data<const char *>;
using Bool = Data<bool>;
using Int = Data<int>;