#include <menu.h>
#include <menu/IO/streamOut.h>
#include <menu/fmt/textFmt.h>

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

namespace action {
  bool quit(Sz) {
    // running=false;
    return true;
  }
};

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
  ItemDef<Text,ItemNav>{"title"},
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

int main(){
  out.put("AM5 R&D");
  out.nl();
  // tinyMenu.printMenu(out,{});
  nav.printTo(out);
  cout<<"end."<<endl;
  return 0;
}