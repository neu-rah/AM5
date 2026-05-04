#include <menu.h>
#include <menu/IO/streamOut.h>
#include <menu/fmt/textFmt.h>
#include <menu/fmt/xmlFmt.h>
#include <iostream>
using namespace std;

using MyMenu=ItemDef<
  MenuBase<
    ItemDef<Text>,
    StaticBody<
      ItemDef<Text>,
      ItemDef<Text>,
      ItemDef<Text>
    >,
    WrapNav,
    PadDraw
  >
>;

MyMenu myMenu("title",{"op1","op2","op3"});

OutDef<
  FullPrinter,
  // TextFmt,
  XmlFmt
  ,
  ConsoleOut
> out;

NavDef<
  TreeNav,
  Root<MyMenu,myMenu>
> nav;

int main(){
  out<<"testing...."<<endl;
  nav.printTo(out);
  // myMenu.printMenu(out,{{}});
  out.flush();
  cout<<myMenu.depth()<<endl;
  cout<<myMenu.size()<<endl;
  return 0;
}