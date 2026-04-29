#include <menu.h>
#include <menu/IO/streamOut.h>
#include <menu/fmt/textFmt.h>
#include <menu/IO/pcKbdIn.h>
#include <menu/IO/linuxKeyIn.h>

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

bool running=true;

namespace action {
  bool quit(Sz) {
    running=false;
    return true;
  }
};

InDef<
  #ifdef ARDUINO
    SerialIn,
  #else
    LinuxKeyIn,
  #endif
  PCKbd
> in;

using Printer=Chain<
  ViewPrinter,// outermost format envelope
  MenuPrinter<// calls printMenu
    TitlePrinter,// just print the title
    // ScrollBodyPrinter,//scroll till focus is visible
    BodyPrinter,//stream/serial print
    ItemPrinter<//calls printItem:
      IndexPrinter,// print item index 1-9
      NavCursorPrinter,// use a text cursor on selected item.
      ItemBodyPrinter//→ printItem → Item::print
    >
  >
>;

OutDef<
  Printer,
  TextFmt,
  ConsoleOut
> out;

auto tinyMenu=menuDef<Wraps::yes>(
  ItemDef<Text,ItemNav>{"Title"},
  staticBody(
    ItemDef<Text>{"yawn!"},
    // dateField("date:"),
    ItemDef<Text>{"wtf!"},
    ItemDef<Text,Action<action::quit>>{"exit"}
  )
);

NavDef<
  TreeNav,
  Root<decltype(tinyMenu),tinyMenu>
> nav;

bool run() {
  static TinyTimeUtils::FPS<60> fps;
  if(fps) {
    fps.reset();
    nav.in(in);
    if(nav.changed(out)) {
      nav.printTo(out);
      nav.sync(out);
    }
  }
  return running;
}

void setup() {
  cout<<"AM5 R&D"<<endl;
  out.lockMode(LockMode::None);
  nav.printTo(out);
}

int main(){
  setup();
  // cout<<"nav changed:"<<nav.changed(out)<<endl;
  while(run());
  cout<<"end."<<endl;
  return 0;
}