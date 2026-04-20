// includes --
  #include <menu.h>
  #include <menu/fmt/textFmt.h>
  #include <menu/IO/pcKbdIn.h>
  #include <menu/IO/linuxKeyIn.h>
  #include <menu/IO/streamOut.h>

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
    ItemPrinter<
      IndexPrinter,
      NavCursorPrinter,
      ItemBodyPrinter
    >
  >,
  TextFmt,
  ConsoleOut
> out;

InDef<
  #ifdef ARDUINO
    SerialIn,
  #else
    LinuxKeyIn,
  #endif
  PCKbd
> in;

int power=55;

bool op1(Sz i) {cout<<"option 1 called!"<<endl;return true;}

using MainMenu=MenuDef<
  Title<ItemNav<Wraps::yes>,Text>,
  StaticBody<
    ItemDef<Text,Action<op1>>,
    ItemDef<Text>,
    ItemDef<Text>
  >//::Map<InsDataPrint>
>;

MainMenu menu{"Main menu",{"op1","op2","op3"}};

NavDef<TreeNav,Root<MainMenu,menu>> nav;

void run() {
  static TinyTimeUtils::FPS<30> fps;
  if(fps) {
    fps.reset();
    nav.in(in);
    if(nav.changed(out)) {
      nav.navPrint(out);
      nav.sync();
    }
  }
}

void setup(){
  #ifdef ARDUINO
    Serial.begin(115200);
    while(!Serial);
  #endif
  out.mode(LockMode::None);
  nav.navPrint(out);
}

#ifdef ARDUINO
  void loop() {run();}
#else
  int main() {setup();run();}
#endif