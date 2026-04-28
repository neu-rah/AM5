#ifdef __AVR__
  #include <streamFlow.h>
  using namespace StreamFlow;
  #define cout Serial
  #include <avr_std.h>
#else
  #include <iostream>
  #include <type_traits>
#endif
using namespace std;

using Sz=int;
using Depth=int;

enum Fmt {View,Menu,Title,Body,Item,Idx,Accel,NavCursor,EditMode,EditCursor,Label,Unit};

struct Ctx {
  Sz idx;
  Depth lel;
};

#ifdef TMP
  template<bool ss,Fmt tag> enable_if_t<tag==View> fmt(Ctx) {if(ss) cout<<"View start"; else cout<<"View stop";}
  template<bool ss,Fmt tag> enable_if_t<tag==Menu> fmt(Ctx) {if(ss) cout<<"View start"; else cout<<"View stop";}
  template<bool ss,Fmt tag> enable_if_t<tag==Item> fmt(Ctx) {if(ss) cout<<"View start"; else cout<<"View stop";}
  template<bool ss,Fmt tag> enable_if_t<tag==Idx> fmt(Ctx) {if(ss) cout<<"View start"; else cout<<"View stop";}
#endif

#ifndef TMP
  #ifdef TFT
    template<bool ss,Fmt tag>
    void fmt(Ctx) {
  #endif
  #ifdef TT
    template<Fmt tag>
    void fmt(bool ss,Ctx) {
  #endif
  #ifdef SF
    void fmt(bool ss,Fmt tag,Ctx) {
  #endif
    if(ss) {
      switch(tag) {
        case View: cout<<"View start";break;
        case Menu: cout<<"Menu start";break;
        case Item: cout<<"Item start";break;
        // case Idx: cout<<"Idx start";break;
        default:break;
      };
    } else {
      switch(tag) {
        case View: cout<<"View stop";break;
        case Menu: cout<<"Menu stop";break;
        case Item: cout<<"Item stop";break;
        // case Idx: cout<<"Idx stop";break;
        default:break;
      };
    }
  }
#endif

void setup() {
  Serial.begin(115200);
  while(!Serial);
}

int main() {
  #ifdef TMP
    fmt<true,View>({});
    fmt<true,Menu>({});
    fmt<true,Item>({});
    cout<<"printing data here!"<<endl;
    fmt<false,Item>({});
    fmt<false,Menu>({});
    fmt<false,View>({});
  #endif
  #ifdef TFT
    fmt<true,View>({});
    fmt<true,Menu>({});
    fmt<true,Item>({});
    cout<<"printing data here!"<<endl;
    fmt<false,Item>({});
    fmt<false,Menu>({});
    fmt<false,View>({});
  #endif
  #ifdef TT
    fmt<View>(true,{});
    fmt<Menu>(true,{});
    fmt<Item>(true,{});
    cout<<"printing data here!"<<endl;
    fmt<Item>(false,{});
    fmt<Menu>(false,{});
    fmt<View>(false,{});
  #endif
  #ifdef SF
    fmt(true,View,{});
    fmt(true,Menu,{});
    fmt(true,Item,{});
    cout<<"printing data here!"<<endl;
    fmt(false,Item,{});
    fmt(false,Menu,{});
    fmt(false,View,{});
  #endif
  return 0;
}
