#include <menu.h>
#include <menu/IO/streamOut.h>
#include <menu/fmt/textFmt.h>
#include <iostream>
using namespace std;

using MyMenu=ItemDef<
  Menu<Wraps::yes,Pad::no>
  ::template Title<ItemDef<Text>>
  ::template Body<
    StaticBody<
      ItemDef<Text>,
      ItemDef<Text>
    >
  >
>;

MyMenu myMenu("title",{"op1","op2"});

OutDef<
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
  >,
  TextFmt,
  ConsoleOut
> out;

int main(){
  out<<"testing...."<<endl;
  out.lockMode(LockMode::None);
  myMenu.printMenu(out,{{}});
  out.flush();
  return 0;
}