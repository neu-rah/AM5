#pragma once

#include "menu/sys/base.h"

template<typename... OO> using DataDef=DefaultDataDef<OO...>;

/*
the case of OneData API expansion before usage

this expansion overlaps the data, no CRTP needed

we have to hide all hapi namespace... a case for `one` or `odata` or `hapi::one::data`

alternative for this position would be a DataDef patch

MUST BE THIS CASE WHEN WE WANT TO INTEGRATE WITH ANOTHER API (Item)
*/

struct DataPrint {
  template<typename O>
  struct Part:O {
    using Base=O;
    using Base::Base;
    using Base::get;
    template<typename Out> void print(Out& out,Ctx& ctx) {
      out.template fmtStart<Fmt::Data>(ctx);
      out.put(get());
      out.template fmtStop<Fmt::Data>(ctx);
      Base::print(out,ctx);
    }
  };
};

template <typename T> using Data=Chain<DataPrint,hapi::data::Data<T>>;
template <typename T, T &Ref> using DataRef=Chain<DataPrint,hapi::data::DataRef<T,Ref>>;
template <const CText &Text> using TextRef = ::DataRef<const CText, Text>;
template <const CText &text > using StaticText=::TextRef<text>;

using Text = ::Data<const char *>;
using Int  = ::Data<int>;
using Bool = ::Data<bool>;

/*
USE THIS CASE WHEN YOU JUST WANT TO ADD MEMBERS TO THE API
*/
// template<typename Cfg=Nil>
// struct DataPrintAPI:DataAPI<Cfg> {
//   template<typename Out>
//   static constexpr void print(Out& out,Ctx& ctx) {}
// };

// template <typename... OO>
// struct DataDef : APIOf<DataPrintAPI<>, OO...> {
//   using Base = APIOf<DataPrintAPI<>, OO...>;
//   using Base::Base;
//   using Base::get;
//   template<typename Out> void print(Out& out,Ctx&& ctx={}) {print(out,ctx);}
//   template<typename Out> void print(Out& out,Ctx& ctx) {
//     out.template fmtStart<Fmt::Data>(ctx);
//     out.put(get());
//     out.template fmtStop<Fmt::Data>(ctx);
//     Base::print(out,ctx);
//   }
// };


