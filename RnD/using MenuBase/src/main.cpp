#include <menu.h>
#include <menu/IO/streamOut.h>
#include <menu/fmt/textFmt.h>
#include <menu/fmt/xmlFmt.h>
#include <menu/IO/pcKbdIn.h>
#include <menu/IO/linuxKeyIn.h>
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
  XmlFmt,
  ConsoleOut
> out;

NavDef<
  TreeNav,
  Root<MyMenu,myMenu>
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
  cout<<nav.changed(out)<<endl;
  while(run());
  return 0;
}