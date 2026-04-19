#include <iostream>
using namespace std;

#include <hapi.h>

struct Nil {};

template<typename O=Nil> 
struct DataAPI:O {
  using Base=O;
  using Base::Base;
  static constexpr bool changed() {return false;}
  static constexpr void sync() {}
  // template<typename Out> static void printTo(Out& out,Ctx& ctx) {}
};

template<typename N>
struct Link:N {
  template<typename O>
  struct Part:N::Part<O> {
    using Base=typename N::template Part<O>;
    using Base::Base;
    constexpr bool changed() {return Base::changed()||O::changed();}
  };
};

template<typename... OO>
struct DataDef:APIOf<DataAPI<>,OO...>::template Map<Link> {
  using Base=typename APIOf<DataAPI<>,OO...>::template Map<Link>;
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
  };
};

template<typename T>
struct Data {
  template<typename O>
  struct Part:O {
    using Type=T;
    using Base=O;
    using Base::Base;
    Type data;
    template<typename... OO> Part(const Type& o,OO&... oo):data{o},Base{oo...}{}
    constexpr Type& get() {return data;} 
    constexpr Type& get() const {return data;} 
  };
};

struct Watch {
  template<typename O>
  struct Part:O {
    using Base=O;
    using Type=typename Base::Type;
    using Base::get;
    using Base::Base;
    std::remove_reference_t<Type> watched;
    constexpr bool changed() {return get()!=watched;}
    constexpr void sync() {watched=get();}
  };
};

//==================================================================--
const char* nome{"Rui"};
int ano{1967};
char notas[]{"Notes..."};

using A=DataDef<StaticData<int,11>>;
using B=DataDef<StaticRef<const char*,nome>>;
using C=DataDef<Watch,StaticRef<int,ano>>;
using D=DataDef<StaticRef<decltype(notas),notas>>;

DataDef<Watch,Data<int>> month{11};

int main() {
  month.sync();
  cout<<"month changed:"<<month.changed()<<endl;
  cout<<A::get()<<endl;
  cout<<B::get()<<endl;
  C::get()=12;
  cout<<C::get()<<endl;
  D::get()[0]='V';
  cout<<D::get()<<endl;
  month.get()=21;
  cout<<month.get()<<endl;
  cout<<"month changed:"<<month.changed()<<endl;
  return 0;
}