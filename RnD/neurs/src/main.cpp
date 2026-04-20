// includes --
  #include <menu.h>
  // #include <menu/fmt/textFmt.h>
  #include <menu/fmt/ansiFmt.h>
  // #include <menu/fmt/xmlFmt.h>
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

//this are responsible for format
using Printer=Chain<
  ViewPrinter,// outermost format envelope
  MenuPrinter<// calls printMenu
    TitlePrinter,// just print the title
    ScrollBodyPrinter,//scroll till focus is visible
    // BodyPrinter,//stream/serial print
    ItemPrinter<//calls printItem:
      IndexPrinter,// print item index 1-9
      NavCursorPrinter,// use a text cursor on selected item.
      ItemBodyPrinter//→ printItem → printTo
    >
  >
>;

IOutDef<
  Printer,//user defined format sequence
  ANSIFmt,//add some ANSI colors and format to the output
  // ClearFree,//clear free space after menu print
  DataParser<>,//put all data into characters
  CtrlChars,
  UTF8,//bypass UTF8 surrogate codes
  TextWrap,//long texts continue next line
  Clip,//keep content inside area
  ColorTrack<int>,//track color setting for device resume
  Cursor,//account for cursor movement, single character only, tracks pos for resume also
  Gate,//locks output for measuring and other operations
  ANSIOut,//inject ansi codes into the next output device
  #ifdef ARDUINO
    SerialOut,
  #else
    ConsoleOut,
    // Debug_MinimalDrawConsole,
  #endif
  StaticPos<40,5>,
  StaticArea<20,6>
> out;

InDef<
  #ifdef ARDUINO
    SerialIn,
  #else
    LinuxKeyIn,
  #endif
  PCKbd
> in;

bool op1(Sz i) {cout<<"option 1 called!"<<endl;return true;}

bool running{true};

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

bool run() {
  static TinyTimeUtils::FPS<60> fps;
  if(fps) {
    fps.reset();
    nav.in(in);
    if(nav.changed(out)) {
      nav.navPrint(out);
      nav.sync();
    }
  }
  return running;
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
  int main() {
    setup();
    while(run());
  }
#endif