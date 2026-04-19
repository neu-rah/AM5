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

OutDef<
  ViewPrinter,
  MenuPrinter<
    TitlePrinter,
    BodyPrinter,
    ItemPrinter<PrintItem>
  >,
  TextFmt,
  ConsoleOut
> out;

int power=55;

void op1(Sz i) {cout<<"option 1 called!"<<endl;}

using MainMenu=MenuDef<
  Title<Text>,
  StaticBody<
    ItemDef<Text,Action<op1>>,
    ItemDef<Text>,
    ItemDef<Text>
  >//::Map<InsDataPrint>
>;

MainMenu menu{"Main menu",{"op1","op2","op3"}};

NavDef<TreeNav,Root<MainMenu,menu>> nav;

void run() {
  nav.navPrint(out);
  // menu.print(out);
  // Ctx ctx{};
  // menu.body().printBody(out,ctx);
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