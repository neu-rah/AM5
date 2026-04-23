// includes --
  #include <menu.h>
  #include <menu/fmt/textFmt.h>
  #include <menu/fmt/ansiFmt.h>
  #include <menu/IO/pcKbdIn.h>
  #include <menu/fmt/xmlFmt.h>
  #include <menu/sys/cArrayBody.h>
  #ifdef ARDUINO
    #include <Arduino.h>
    #include <menu/IO/arduino/serialIn.h>
    #include <menu/IO/arduino/serialOut.h>
  #else
    #include <menu/IO/linuxKeyIn.h>
    #include <menu/IO/streamOut.h>
    #include <menu/sys/stdBody.h>
    #include <vector>
    #include <forward_list> 
  #endif

  #include <tinyTimeUtils.h>

  #ifdef __AVR__
    #include "menu/sys/platform/avr/avr_std.h"
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
    // ScrollBodyPrinter,//scroll till focus is visible
    BodyPrinter,//stream/serial print
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

bool running{true};

namespace text {
  static constexpr const CText dateSep{"."};
};

namespace action {
  bool subIdx(Sz i) {return false;}
  bool quit(Sz) {
    running=false;
    return true;
  }
}

auto menu=menuDef<Wraps::yes>(
  ItemDef<Text,ItemNav>{"AM5 R&D"},
  staticBody(
    ItemDef<Text>{"op1"},
    menuDef<Wraps::yes>(
      ItemDef<Text,ItemNav>{"Birth:"},
      staticBody(
        ItemDef<EditField,ParentDraw,AsEditMode<>,NumField<StaticNumRange<int,1900,2050>,ItemNav,Watch<AsField<Default<int,2026>,Int>>>>{2026},
        ItemDef<StaticText<text::dateSep>,EditField,ParentDraw,AsEditMode<>,NumField<StaticNumRange<int,1,12>,ItemNav,Watch<AsField<Int>>>>{1},
        ItemDef<StaticText<text::dateSep>,EditField,ParentDraw,AsEditMode<>,NumField<StaticNumRange<int,1,31>,ItemNav,Watch<AsField<Int>>>>{1}
      )
    ),
    ItemDef<Text,Action<action::quit>>{"Exit"}
  )
);

INavDef<
  TreeNav,
  Root<decltype(menu),menu>
> nav;

//==========================================================================================--
bool run() {
  static TinyTimeUtils::FPS<60> fps;
  if(fps) {
    fps.reset();
    nav.in(in);
    if(nav.changed(out)) {
      nav.navPrint(out);
      nav.sync(out);
    }
  }
  return running;
}

using Empty=ItemDef<>;

void setup(){
  #ifdef ARDUINO
    Serial.begin(115200);
    while(!Serial);
  #endif
  cout<<"ArduinoMenu R&D"<<endl;
  out.mode(LockMode::None);
  nav.navPrint(out);
}

#ifdef ARDUINO
  void loop() {run();}
#else
  int main() {
    setup();
    while(run());
    dout<<xy<0,24><<"end."<<endl;
  }
#endif