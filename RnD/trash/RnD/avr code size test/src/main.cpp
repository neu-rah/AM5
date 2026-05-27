#include <menu.h>

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
  template<Edge ss,Fmt tag> enable_if_t<tag==View> fmt(Ctx) {if(ss) cout<<"View start"; else cout<<"View stop";}
  template<Edge ss,Fmt tag> enable_if_t<tag==Menu> fmt(Ctx) {if(ss) cout<<"View start"; else cout<<"View stop";}
  template<Edge ss,Fmt tag> enable_if_t<tag==Item> fmt(Ctx) {if(ss) cout<<"View start"; else cout<<"View stop";}
  template<Edge ss,Fmt tag> enable_if_t<tag==Idx>  fmt(Ctx) {if(ss) cout<<"View start"; else cout<<"View stop";}
#endif

#ifndef TMP
  #ifdef TFT
    template<Edge ss,Fmt tag>
    void fmt(Ctx) {
  #endif
  #ifdef TT
    template<Fmt tag>
    void fmt(Edge ss,Ctx) {
  #endif
  #ifdef SF
    void fmt(Edge ss,Fmt tag,Ctx) {
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
    fmt<start,View>({});
    fmt<start,Menu>({});
    fmt<start,Item>({});
    cout<<"printing data here!"<<endl;
    fmt<stop,Item>({});
    fmt<stop,Menu>({});
    fmt<stop,View>({});
  #endif
  #ifdef TFT
    fmt<start,View>({});
    fmt<start,Menu>({});
    fmt<start,Item>({});
    cout<<"printing data here!"<<endl;
    fmt<stop,Item>({});
    fmt<stop,Menu>({});
    fmt<stop,View>({});
  #endif
  #ifdef TT
    fmt<View>(start,{});
    fmt<Menu>(start,{});
    fmt<Item>(start,{});
    cout<<"printing data here!"<<endl;
    fmt<Item>(stop,{});
    fmt<Menu>(stop,{});
    fmt<View>(stop,{});
  #endif
  #ifdef SF
    fmt(start,View,{});
    fmt(start,Menu,{});
    fmt(start,Item,{});
    cout<<"printing data here!"<<endl;
    fmt(stop,Item,{});
    fmt(stop,Menu,{});
    fmt(stop,View,{});
  #endif
  return 0;
}
