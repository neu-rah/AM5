// includes --
  #include <menu.h>
  #include <menu/fmt/textFmt.h>
  #include <tinyTimeUtils.h>

  #ifdef ARDUINO
    #include <Arduino.h>
  #endif

  #ifdef __AVR__
    #include <avr_std.h>
    #include <streamFlow.h>
    using namespace StreamFlow;
    #define cout Serial
  #else
    #include <iostream>
    #include <type_traits>
    #include <cstring>
    using namespace std;
  #endif
  using namespace std;

OutDef<ItemPrinter,TextFmt,PrintItem,ConsoleOut> out;

int power=55;

//define data we can iteract with
TypeDef<int>
  // ::Value//hold data internally (functions get/set) or use
  ::Ref<power>//as a static compile time reference to external data, functions get/set
  ::template StaticNumRange<0,100>//valid range info
  ::Step<Wrap::yes> //step the value (assuming valid), functions up/down
n;

//define an Item using some data
ItemDef<
  Data<TypeDef<const char*>::Value>,
  Data<TypeDef<int>::Value>
> item1{"year: ",1967};

template<typename A>
struct Action {
  template<typename O>
  struct Part:A,O {
    using Base=O;
    static constexpr void nav(CKE cke,Path path) {
      if(cke.cmd==Cmd::Enter) A::get()(path.path[0]);
      Base::nav(cke,path);
    }
  };
};

void op1(Sz i) {cout<<"option 1 called!"<<endl;}

using Body=StaticBody<
  ItemDef<Data<TypeDef<const char*>::Value>,Action<TypeDef<void(*)(int)>::Static<op1>>>,
  ItemDef<Data<TypeDef<const char*>::Value>>,
  ItemDef<Data<TypeDef<const char*>::Value>>
>;

Body body{"op1","op2","op3"};

PathData<Body::depth()> pathData{0};

void run() {
  Ctx ctx{pathData.path(0,0)};
  body.printBody(out,ctx);
  body.nav({Cmd::Enter},pathData,0);
  cout<<endl;
}

void setup(){
  #ifdef ARDUINO
    Serial.begin(115200);
    while(!Serial);
  #endif
}

#ifdef ARDUINO
  void loop() {run();}
#else
  int main() {setup();run();}
#endif