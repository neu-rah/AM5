#include <menu.h>

#include <menu/IO/arduino/serialOut.h>
#include <menu/IO/arduino/serialIn.h>
#include <menu/IO/pcKbdIn.h>

#include <menu/fmt/textFmt.h>

#include <menu/sys/cArrayBody.h>

#include <tinyTimeUtils.h>
// using namespace TinyTimeUtils;

InDef<
  SerialIn,
  PCKbd
> in;

namespace text {
  static const CText op1{"#1"};
  static const CText op2{"#2"};
  static const CText op3{"#3"};
};

#ifdef IITEM
  IOutDef<
#else
  OutDef<
#endif
  ViewPrinter,// outermost format envelope
  MenuPrinter<// calls printMenu
    // TitlePrinter,// just print the title
    BodyPrinter,//stream/serial print
    ItemPrinter<//calls printItem:
      IndexPrinter,// print item index 1-9
      NavCursorPrinter,// use a text cursor on selected item.
      ItemBodyPrinter//→ printItem → printTo
    >
  >,
  TextFmt,
  SerialOut
> out;

bool action(Sz i) {out<<"action from #"<<(i+1)<<endl;return false;}

#ifdef CTEXT
  MenuDef<
    Title<BodyAction<action>>,
    StaticBody<
      ItemDef<StaticText<&text::op1>>,
      ItemDef<StaticText<&text::op2>>,
      ItemDef<StaticText<&text::op3>>
    >
  > menu{};

  NavDef<
    TreeNav,
    Root<decltype(menu),menu>
  > nav;

#endif

#ifdef STRTEXT
  MenuDef<
    Title<BodyAction<action>>,
    StaticBody<
      ItemDef<Text>,
      ItemDef<Text>,
      ItemDef<Text>
    >
  > menu{"#1","#2","#3"};

  NavDef<
    TreeNav,
    Root<decltype(menu),menu>
  > nav;

#endif

#ifdef ARRAYBODY
  ItemDef<Text> cBody[]{"#1","#2","#3"};
  MenuDef<
    Title<BodyAction<action>>,
    CArrayBody<ItemDef<Text>,cBody,sizeof cBody/sizeof *cBody>
  > menu;

  NavDef<
    TreeNav,
    Root<decltype(menu),menu>
  > nav;

#endif

#ifdef IITEM
  IItem* iBody[]{
    new IItemDef<StaticText<&text::op1>>{},
    new IItemDef<StaticText<&text::op2>>{},
    new IItemDef<StaticText<&text::op3>>{}
  };
  MenuDef<//sub menu with C array body of virtual `IItem` (not all of the same type)
    Title<BodyAction<action>>,
    CPtrArrayBody<IItem,iBody,sizeof(iBody)/sizeof(iBody[0])>
  > menu;

  INavDef<
    TreeNav,
    Root<decltype(menu),menu>
  > nav;
#endif

void setup() {
  Serial.begin(115200);
  while(!Serial);
  out.put("AM5.dev");
  out.nl();
  out.mode(LockMode::None);
  nav.navPrint(out);
}
void loop() {
  static TinyTimeUtils::FPS<50> menuFps;
  if(menuFps) {
    menuFps.reset();
    nav.in(in);
    if(nav.changed()) {
      nav.navPrint(out);
      nav.sync(out);
    }
  }
}