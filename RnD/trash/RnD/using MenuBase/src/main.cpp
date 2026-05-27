#include <menu.h>

#include <menu/fmt/textFmt.h>
#include <menu/fmt/xmlFmt.h>
#include <menu/IO/pcKbdIn.h>
#ifdef __AVR__
#else
  #include <menu/IO/streamOut.h>
  #include <menu/IO/linuxKeyIn.h>
#endif

#ifdef ARDUINO
#include "menu/IO/arduino/serialIn.h"
#include "menu/IO/arduino/serialOut.h"
#endif

namespace text {
  static constexpr const CText title{"Title"};
  static constexpr const CText op1{"op1"};
  static constexpr const CText op2{"op2"};
  static constexpr const CText op3{"op3"};
};

#ifdef STATIC
/*
```text
RAM:   [=         ]  12.6% (used 258 bytes from 2048 bytes)
Flash: [=         ]   9.3% (used 2996 bytes from 32256 bytes)
```
*/

  using MyMenu=MenuDef<
    ItemDef<StaticText<text::title>>,
    StaticBody<
      ItemDef<StaticText<text::op1>>,
      ItemDef<StaticText<text::op2>>,
      ItemDef<StaticText<text::op3>>
    >,
    WrapNav,
    PadDraw
  >;
  
  MyMenu myMenu;
#endif

#ifdef TEXT
/*
```text
RAM:   [=         ]  13.0% (used 267 bytes from 2048 bytes)
Flash: [=         ]   9.0% (used 2894 bytes from 32256 bytes)
```
*/

  using MyMenu=MenuDef<
    ItemDef<Text>,
    StaticBody<
      ItemDef<Text>,
      ItemDef<Text>,
      ItemDef<Text>
    >,
    WrapNav,
    PadDraw
  >;
  
  MyMenu myMenu(text::title,{text::op1,text::op2,text::op3});
#endif

#ifdef ALT
/*
```text
AM:   [=         ]  13.0% (used 266 bytes from 2048 bytes)
Flash: [=         ]   9.3% (used 3014 bytes from 32256 bytes)
```
*/
  //alternative construction
  auto myMenu=menuDef(
    ItemDef<StaticText<text::title>>{},
    staticBody(
      ItemDef<Text>{text::op1},
      ItemDef<Text>{text::op2},
      ItemDef<Text>{text::op3}
    )
  );
#endif

InDef<
  #ifdef ARDUINO
    SerialIn,
  #else
    LinuxKeyIn,
  #endif
  PCKbd
> in;

OutDef<
  FullPrinter,
  // XmlFmt,
  TextFmt,
  #ifdef ARDUINO
    SerialOut
  #else
    ConsoleOut
  #endif
> out;

NavDef<
  TreeNav,
  Root<decltype(myMenu),myMenu>
> nav;

bool running{true};

bool run() {
  static TinyTimeUtils::FPS<60> fps;
  if(fps) {
    fps.reset();
    nav.in(in);
    if(nav.changed(out)) {
      out.lockMode(LockMode::None);
      nav.printTo(out);
      nav.sync(out);
    }
  }
  return running;
}

int main(){
  out<<"testing...."<<endl;
  nav.printTo(out);
  while(run());
  return 0;
}