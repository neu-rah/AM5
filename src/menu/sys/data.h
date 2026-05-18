#pragma once

#include "menu/sys/base.h"

/*
OR leave DataDef to be user side patched by renaming the current to DefaultDataDef
with easy alias to DataDef if no patch needed

here the second case, much cleaner and can expand both sides of the API, the cap Def layer and the API base
*/

template<typename Cfg=Nil>
struct DataPrintAPI:DataAPI<Cfg> {
  template<typename Out>
  static constexpr void print(Out& out,Ctx& ctx) {}
};

template <typename... OO>
struct DataDef : APIOf<DataPrintAPI<>, OO...> {
  using Base = APIOf<DataPrintAPI<>, OO...>;
  using Base::Base;
  using Base::get;
  template<typename Out> void print(Out& out,Ctx&& ctx={}) {print(out,ctx);}
  template<typename Out> void print(Out& out,Ctx& ctx) {
    out.template fmtStart<Fmt::Data>(ctx);
    out.put(get());
    out.template fmtStop<Fmt::Data>(ctx);
    Base::print(out,ctx);
  }
};


/*
the case of OneData API expansion before usage

this expansion overlaps the data, no CRTP needed

we have to hide all hapi namespace... a case for `one` or `odata` or `hapi::one::data`

alternative for this position would be a DataDef patch (above)
*/
// struct DataPrint {
//   template<typename O>
//   struct Part:O {
//     using Base=O;
//     using Base::Base;
//     using Base::get;
//     template<typename Out> void print(Out& out,Ctx& ctx) {
//       out.template fmtStart<Fmt::Data>(ctx);
//       out.put(get());
//       out.template fmtStop<Fmt::Data>(ctx);
//       Base::print(out,ctx);
//     }
//   };
// };

// template <typename T> using Data=Chain<DataPrint,hapi::Data<T>>;
// template <typename T, T &Ref> using DataRef=Chain<DataPrint,hapi::DataRef<T,Ref>>;
// template <const CText &Text> using TextRef = DataRef<const CText, Text>;
// template <const CText &text > using StaticText=TextRef<text>;

// using Text = Data<const char *>;
// // using Bool = Data<bool>;
// using Int  = Data<int>;
