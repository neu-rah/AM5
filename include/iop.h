#pragma once

#ifdef __AVR__
  #include "platform/avr/avr_std.h"
#elif defined(__XTENSA__) 
  #include "platform/xtensa/xtensa_std.h"
#else
  #include <type_traits>
  #include <utility>
  #include <stddef.h>
  using std::size_t;
#endif

namespace hapi {
  struct Nil {};
};

namespace hapi {

  template<template<typename...> class Interface, typename... Ts>
  struct Composite;

  template<template<typename...> class Interface, typename T,typename... Ts>
  struct Composite<Interface,T,Ts...> {
    static constexpr size_t size = sizeof...(Ts)+1;

    using Head=T;
    using Tail=Interface<Ts...>;

    template<typename O>
    static constexpr bool has = std::is_same_v<O, T> ||(std::is_same_v<O, Ts> || ...);

    //this is a cargo transfer, overkill?
    template<template<typename...> class Template>
    using Build = Template<Ts...>;

    // Append / Prepend
    template<typename... OO > using App = Interface<T, Ts..., OO...>;
    template<typename... OO> using Ins = Interface<OO..., T, Ts...>;

    // Concat
    template<typename... Us> using Concat = Interface<T,Ts..., Us...>;

  };

  template<template<typename...> class Interface>
  struct Composite<Interface> {
    static constexpr size_t size = 0;
    template<typename T> static constexpr bool has = false;
    template<template<typename...> class Template>
    using Build = Template<>;

    // Append / Prepend
    template<typename... OO> using App = Interface<OO...>;
    template<typename... OO> using Ins = Interface<OO...>;

    // Concat
    template<typename... Us> using Concat = Interface<Us...>;

  };

  //generic composite
  // template<typename... Ts>
  // struct TypeList:Composite<TypeList,Ts...>{};
  
};//namespace hapi
/**
 * @file hapi.h
 * @brief The Happy API - Composition API build tools.
 * @author Rui Azevedo (ruihfazevedo@gmail.com)
 * @version beta
 * @contributor Grok (xAI) - architecture, cleanup & modern C++ patterns
 * @copyright MIT licence
*/

namespace hapi {
  
  //API specific composite assembler
  template<typename... Ts>
  struct Chain : Composite<Chain, Ts...> {
    
    using Base=Composite<Chain, Ts...>;
    using Head=typename Base::Head;
    using Tail=typename Base::Tail;

    template<typename Predicate>
    static constexpr const bool require=
      (Predicate::template check<Ts>()||...);

    template<typename T>
    struct Part:Head::template Part<typename Tail::template Part<T>> {
      using Base=typename Head::template Part<typename Tail::template Part<T>>;
      using Base::Base;
      template<typename Q>
      static constexpr const bool has{Chain::template has<Q>};
    };
  };

  //empty chain
  template<> struct Chain<>:Composite<Chain> {
    template<typename T> using Part=T;
    template<typename Predicate>
    static constexpr const bool require=false;
  };

  //optional, use only if your API needs it
  template<typename O>
  struct CRTP {
    O& obj() {return static_cast<O&>(*this);}
    const O& obj() const {return static_cast<const O&>(*this);}
    
    O* operator->() {return static_cast<O*>(this);}
    const O* operator->() const {return static_cast<const O*>(this);}
  };

  template<typename API, typename... OO>
  struct APIOf : Chain<OO...>::template Part<API> {
    using Base = typename Chain<OO...>::template Part<API>;
    using Base::Base;
  };


}; //namespace hapi 

/**
 * @file rules.h
 * @author Rui Azevedo (neu-rah) (ruihfazevedo@gmail.com)
 * @brief rules system for HAPI
 * @contributor Grok (xAI) - architecture, cleanup & modern C++ patterns
*/

namespace hapi {
  template<typename Crit>
  struct IsSame {
    template<typename... OO>
    static constexpr const bool check() {return (std::is_same_v<Crit,OO>||...);}
  };

  template<typename Predicate>
  struct Has {
    template<typename Ahead,typename Behind=Chain<>>
    static constexpr const std::enable_if_t<Ahead::size==0,bool> check() {return false;}
    
    template<typename Ahead,typename Behind=Chain<>>
    static constexpr const std::enable_if_t<Ahead::size!=0,bool> check() 
      {return Ahead::template Join<Behind>::template require<Predicate>;}

    struct Before {
      // template<typename Ahead,typename Behind=hapi::Chain<>>
      // static constexpr const std::enable_if_t<Ahead::size==0,bool> check() {return false;}

      template<typename Ahead,typename Behind=Chain<>>
      static constexpr const bool check()
        {return Ahead::template require<Predicate>;}
    };

    struct After {
      // template<typename Ahead,typename Behind=hapi::Chain<>>
      // static constexpr const std::enable_if_t<Ahead::size==0,bool> check() {return false;}

      template<typename Ahead,typename Behind=Chain<>>
      static constexpr const bool check()
        {return Behind::template require<Predicate>;}
    };
  };
};//namespace hapi
/**
 * @file oneList.h
 * @author Rui Azevedo (neu-rah)
 * @brief Minimal heterogeneous runtime list + TypeList
 * @date 2026-05-19
 * @contributor Grok (xAI) - architecture, cleanup & modern C++ patterns
 */

namespace hapi::one_list {

  // ====================== TypeList ======================
  template<typename... Ts>
  struct TypeList {
    static constexpr size_t size = sizeof...(Ts);

    template<typename T>
    static constexpr bool has = (std::is_same_v<std::decay_t<T>, Ts> || ...);
  };

  // ====================== Runtime List ======================
  template<typename O, typename... OO>
  struct List:TypeList<O, OO...> {
    using Head = O;
    using Tail = List<OO...>;
    using Types = TypeList<O, OO...>;
    Head head;
    Tail tail;

    constexpr List() noexcept : head{},tail{}{}

    constexpr List(O&& o,OO&&... oo) noexcept :
      head{std::forward<O>(o)},
      tail{std::forward<OO>(oo)...}
      {}

    constexpr List(OO&&... oo) noexcept :
      head{},
      tail{std::forward<OO>(oo)...}
      {}

    template<typename Id>
    constexpr const auto& withId() const {
      if constexpr (Head::template has<Id>) return head;
      else return tail.template withId<Id>();
    }
  };

  template<typename O>
  struct List<O>:TypeList<O> {
    using Head = O;
    using Tail = void;
    using Types = TypeList<O>;
    Head head;

    constexpr List() noexcept : head{} {}
    constexpr List(Head&& h) noexcept : head(std::forward<Head>(h)) {}

    template<typename Id>
    constexpr const auto& withId() const {
      static_assert(Types::template has<Id>, "Id not found in List");
      return head;
    }
  };

  // ====================== Factory ======================
  template<typename... OO>
  constexpr auto list(OO&&... oo) {
    return List<std::decay_t<OO>...>{std::forward<OO>(oo)...};
  }

  // ====================== Helpers ======================
  template<typename L, typename F>
  constexpr void forEach(const L& l, F&& f, int i = 0) {
    f(l.head, i);
    if constexpr (!std::is_same_v<typename L::Tail, void>)
      forEach(l.tail, std::forward<F>(f), i + 1);
  }

} // namespace hapi::list
/**
 * @file data.h
 * @author Rui Azevedo (neu-rah)
 * @brief Data API - HAPI data components
 * @contributor Grok (xAI) - architecture, cleanup & modern C++ patterns
*/

namespace hapi { namespace data {
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
  struct DefaultDataDef : APIOf<DataAPI<>, OO...> {
    using Base = APIOf<DataAPI<>, OO...>;
    using Base::Base;
  };

  // use alias or customize the DataDef
  // template <typename... OO> using DataDef = DefaultDataDef<OO...>;

  // ====================== Owned Data ======================
  template <typename T>
  struct Data {
    template <typename O>
    struct Part : O {
      using Base = O;
      using Type = T;

      Type data{};

      template <typename V, typename... OO>
      constexpr Part(V&& value, OO &&...oo)
          : Base{std::forward<OO>(oo)...}
          , data{std::forward<V>(value)} {}

      template <typename... OO>
      constexpr Part(OO &&...oo)
          : Base{std::forward<OO>(oo)...} {}

      const std::decay_t<Type>& get() const { return data; }

      template <typename V>
      void set(V&& v) {data = std::forward<V>(v);}

      operator std::decay_t<Type>&()             { return data; }
      operator const std::decay_t<Type>&() const { return data; }
    };
  };

  // ====================== Data Reference ======================
  template <typename T, T &Ref>
  struct DataRef {
    template <typename O>
    struct Part : O {
      using Base = O;
      using Type = T;
      using Base::Base;
      static T &get() { return Ref; }
      static void set(const T &v) { Ref = v; }

      operator Type &() { return get(); }
      operator const Type &() const { return get(); }
    };
  };

  template <const CText &Text>
  using TextRef = DataRef<const CText, Text>;

  // ====================== Watch ======================
  template <typename W>
  struct Watch {
    template <typename O>
    struct Part : W::template Part<O> {
      using Base = typename W::template Part<O>;
      using Type = typename Base::Type;
      using Base::Base;
      using Base::get;
      using Base::set;

      std::remove_reference_t<Type> watched{};

      constexpr bool changed() const { return get() != watched; }
      void sync() { watched = get(); }
    };
  };

  // ====================== Number Range ======================
  template <typename N>
  struct NumRange {
    template <typename O>
    struct Part : O {
      using Base = O;
      using Type = N;
      using NRP = std::decay_t<Type>;

      using Base::get;
      using Base::set;

      NRP m_low, m_high;
      bool wraps;

      template <typename... OO>
      constexpr Part(NRP low, NRP high, bool w, OO &&...oo)
          : Base{std::forward<OO>(oo)...}, m_low{low}, m_high{high}, wraps{w} {}

      constexpr bool valid(NRP v) const { return v >= m_low && v <= m_high; }
      constexpr NRP clamp(NRP v) const { return v < m_low ? m_low : v > m_high ? m_high : v; }

      constexpr NRP stepUp(NRP o,NRP s) {return m_high-o>=s?o+s:wraps?m_low:m_high;}
      constexpr NRP stepDown(NRP s,NRP o) {return o-m_low>=s?o-s:wraps?m_high:m_low;}

      void up(NRP s=1) {set(stepUp(s,get()));}
      void down(NRP s=1) {set(stepDown(s,get()));}

    };
  };

  // ====================== Static Number Range ======================
  template <typename N, N Low, N High, bool Wraps = false>
  struct StaticNumRange {
    template <typename O>
    struct Part : O {
      using Type = N;
      using Base = O;
      using Base::Base;
      using Base::get;
      using Base::set;

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
      using Base::Base;

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
  using Int  = Data<int>;

  template <const CText &text> using StaticText=TextRef<text>;
}};//namespace hapi::data
