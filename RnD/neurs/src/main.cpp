// includes --
  #include <menu.h>
  #include <menu/fmt/textFmt.h>
  #include <menu/fmt/ansiFmt.h>
  #include <menu/fmt/xmlFmt.h>
  #include <menu/sys/cArrayBody.h>
  #include <menu/sys/stdBody.h>
  #include <menu/IO/pcKbdIn.h>
  #include <menu/IO/linuxKeyIn.h>
  #include <menu/IO/streamOut.h>
  #include <vector>
  #include <forward_list> 

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

OutDef<
  TextFmt,
  DataParser<>,//put all data into characters
  CtrlChars,
  // UTF8,//bypass UTF8 surrogate codes
  TextWrap,//long texts continue next line
  Clip,//keep content inside area
  Buffer<>,
  ColorTrack<int>,
  Cursor,//track and report cursor movement
  Gate,//locks output for measuring and other operations
  ANSIOut,//inject ansi codes into the next output device
  #ifdef __AVR__
    SerialOut,
  #else
    ConsoleOut,
  #endif
  StaticPos<35,15>,
  StaticArea<40,3>
> syslog;

OutDef<
  TextFmt,
  DataParser<>,//put all data into characters
  CtrlChars,//catch \n into nl()
  TextWrap,//long texts continue next line
  Clip,//keep content inside area
  ColorTrack<int>,
  Cursor,//track and report cursor movement
  Gate,//locks output for measuring and other operations
  ANSIOut,//inject ansi codes into the next output device
  #ifdef __AVR__
    SerialOut,
  #else
    ConsoleOut,
  #endif
  StaticPos<40,11>,
  StaticArea<20,3>
> footer;

IOutDef<
  ViewPrinter,// outermost format envelope
  MenuPrinter<// calls printMenu
    TitlePrinter,// just print the title
    // ScrollBodyPrinter,//scroll till focus is visible
    BodyPrinter,//stream/serial print
    ItemPrinter<//calls printItem:
      NavCursorPrinter,// use a text cursor on selected item.
      ItemBodyPrinter//→ printItem → printTo
    >
  >,
  XmlFmt,
  DataParser<>,//put all data into characters
  CtrlChars,//catch \n into nl()
  TextWrap,//long texts continue next line
  Clip,//keep content inside area
  ColorTrack<int>,
  Cursor,//track and report cursor movement
  Gate,//locks output for measuring and other operations
  ANSIOut,//inject ansi codes into the next output device
  #ifdef __AVR__
    SerialOut,
  #else
    ConsoleOut,
  #endif
  StaticPos<0,20>,
  StaticArea<100,20>
> web;

bool op1(Sz i) {cout<<"option 1 called!"<<endl;return true;}

namespace text {
  static const CText main_menu{"Main menu"};
  static const CText back{"<Back"};
  static const CText quit{"Exit!"};
  static const CText op1{"Option 1"};
  static const CText op2{"Option 2"};
  static const CText op3{"Option 3"};
  static const CText fields_menu{"Fields..."};
  static const CText array_sub_menu{"Same type array>"};
  static const CText sub_ibody{"IItem* array"};
  static const CText sub_sbody{"std::container"};
  static const CText sub1{"10"};
  static const CText sub2{"40"};
  static const CText sub3{"60"};
  static const CText sub4{"80"};
  static const CText sub5{"100"};
  static const CText power{"Power"};
  static const CText percent{"%"};
  static const CText off{"Off"};
  static const CText on{"On"};
  static const CText yes{"Yes"};
  static const CText no{"No"};
  static const CText toggle_demo{"Toggle"};
  static const CText select_demo{"Select"};
  static const CText choose_demo{"Choose"};
};

namespace desc {
  static const CText back{"return to previous menu."};
  static const CText quit{"terminate the program."};
  static const CText dot{"."};
  static const CText op1{"simple option with action function.\n`Enter` to activate function."};
  static const CText op2{"toggles Option 3\nenabled/disabled state."};
  static const CText op3{"simple option\ncan be enabled/disabled at runtime."};
  static const CText fields_menu{"some field examples..."};
  static const CText array_sub_menu{"pure C-Array as menu body, no virtual functions, all items the same"};
  static const CText sub_ibody{"pure C-Array with virtual derived items"};
  static const CText sub_sbody{"std::container with virtual items"};
};

bool running=true;

enum ids {op3,power,container};

namespace action {
  bool op1(Sz) {
    // syslog.setColors(GREEN,BLACK);
    // syslog.clear();
    syslog<<"option #1 action called."<<endl;
    return true;
  }
  bool op2(Sz);
  bool op3(Sz) {
    // syslog.setColors(GREEN,BLACK);
    // syslog.clear();
    syslog<<"option #3 action called."<<endl;
    return true;
  }
  bool quit(Sz) {
    syslog.setColors(RED,BLACK);
    syslog.erase();
    syslog<<"Bye!"<<endl;
    running=false;
    return true;
  }
  bool subIdx(Sz i) {
    // syslog.setColors(GREEN,BLACK);
    // syslog.clear();
    syslog<<"sub option #"<<i<<" selected."<<endl;
    return false;
  }
}

template<typename... OO> using Desc=OnFocus<typename Put<OO...>::template ToOut<decltype(footer),footer,Clear::yes>>;

using Back=ItemDef</*CloseOnSelect,*/StaticText<text::back>,Desc<StaticText<desc::back>>>;
using Quit=ItemDef<Action<action::quit>,AsLabel<StaticText<text::quit>,Desc<StaticText<desc::quit>>>>;

using CItem=ItemDef<Text>;

CItem cBody[]{
  "..",
  ".pio/",
  "src/",
  "all of the same type.txt",
  "platformio.ini",
  "README.md"
};

bool stay(int i) {return true;}

IItem* iBody[]{
  new IItemDef<StaticText<text::op1>>{},
  new IItemDef<StaticText<text::op2>>{},
  new IItemDef<StaticText<text::op3>>{},
  new IItemDef<Text>{"what else..."},
  new IItemDef<Text,Desc<Text>,Action<stay>>{"Stay","select and stay!"},
  new IItemDef<StaticText<text::back>>{}
  // new Back::As<IItemDef>{}
};

using ChooseDemo=ChooseFieldDef<
  Title<
    StaticText<text::choose_demo>,
    AsEditMode<>,//edit mode indicator
    BodyAction<action::subIdx>
  >,
  StaticBody<//sub menu static body
    ItemDef<AsField<StaticText<text::sub1>,AsUnit<StaticText<text::percent>>>>,
    ItemDef<AsField<StaticText<text::sub2>,AsUnit<StaticText<text::percent>>>>,
    ItemDef<AsField<StaticText<text::sub3>,AsUnit<StaticText<text::percent>>>>,
    ItemDef<AsField<StaticText<text::sub4>,AsUnit<StaticText<text::percent>>>>,
    ItemDef<AsField<StaticText<text::sub5>,AsUnit<StaticText<text::percent>>>>
    //,Back <--//TODO: extend the enter nav to this level and respect the handling (same as esc)
  >,
  Wraps::yes
>;

using SelectDemo=SelectFieldDef<
  Title<
    AsLabel<StaticText<text::select_demo>>,
    AsEditMode<>,//edit mode indicator
    BodyAction<action::subIdx>
  >,
  StaticBody<//sub menu static body
    ItemDef<AsField<StaticText<text::sub1>>>,
    ItemDef<AsField<StaticText<text::sub2>>>,
    ItemDef<AsField<StaticText<text::sub3>>>,
    ItemDef<AsField<StaticText<text::sub4>>>,
    ItemDef<AsField<StaticText<text::sub5>>>
  >,
  Wraps::yes
>;

//a Map operation, this operation will fit Map<> and will insert OO... components into the chain
template<typename... OO> struct Ins {template<typename M>  using Map=typename M::template Ins<OO...>;};
template<typename... OO> struct App {template<typename M>  using Map=typename M::template App<OO...>;};

using ToggleDemo=ToggleFieldDef<
  ItemDef<BodyAction<action::subIdx>,Text,AsEditMode<>>,
  StaticBody<
    ItemDef<AsField<StaticText<text::no>>>,
    ItemDef<AsField<StaticText<text::yes>>>,
    ItemDef<AsField<Text>>
  >
>;

using Power=NumFieldDef<
  Chain<
    Id<ids::power>,
    AsLabel<StaticText<text::power>>//field label
  >,
  NumField<//use range to change the data
    StaticNumRange<int,0,100>,//valid range
    ItemNav<Wraps::no>,
    Watch<AsField<Default<int,55>,Int>>//use `int` and `change watch` as field (data)
  >,
  AsUnit<StaticText<text::percent>>//field unit
>;

auto mainMenu=menuDef(
  ItemDef<Text,ItemNav<Wraps::yes>>{"Main menu"},
  staticBody(
    ItemDef<Action<action::op1>,StaticText<text::op1>,Desc<StaticText<desc::op1>>>{},
    ItemDef<Action<action::op2>,StaticText<text::op2>,Desc<StaticText<desc::op2>>>{},
    ItemDef<Id<ids::op3>,Action<action::op3>,Watch<EnDis<false>>,StaticText<text::op3>,Desc<StaticText<desc::op3>>>{},
    menuDef(
      Title<ItemNav<Wraps::yes>,StaticText<text::fields_menu>,Desc<StaticText<desc::fields_menu>>>{},
      staticBody(
        Power{},
        ToggleDemo{"Toggle","Maybe"},
        SelectDemo{},
        ChooseDemo{},
        ItemDef<Text,Desc<Text>,Action<stay>>{"Stay","calls stay action function."},
        Back{}
      )
    ),
    MenuDef<//sub menu with C array body (all items of the type)
      Title<
        BodyAction<action::subIdx>,
        ItemNav<Wraps::no>,
        StaticText<text::array_sub_menu>,
        Desc<StaticText<desc::array_sub_menu>>
      >,
      CArrayBody<CItem,cBody,sizeof cBody/sizeof *cBody>
    >{},
    MenuDef<//sub menu with C array body of virtual `IItem` (not all of the same type)
      Title<BodyAction<action::subIdx>,ItemNav<Wraps::yes>,StaticText<text::sub_ibody>,Desc<StaticText<desc::sub_ibody>>>,
      CPtrArrayBody<IItem,iBody,sizeof(iBody)/sizeof(iBody[0])>
    >{},
    MenuDef<
      Title<Id<ids::container>,BodyAction<action::subIdx>,ItemNav<Wraps::yes>,StaticText<text::sub_sbody>,Desc<StaticText<desc::sub_sbody>>>,
      StdBody<vector<IItem*>>
    >{},
    Quit{}
  )
);

auto tinyMenu=menuDef(
  ItemDef<Text,ItemNav<Wraps::yes>>{"title"},
  staticBody(
    ItemDef<Text>{"op1"},
    ItemDef<Text,Action<action::quit>>{"exit"}
  )
);

INavDef<
  TreeNav,
  Root<decltype(mainMenu),mainMenu>
  // Root<decltype(tinyMenu),tinyMenu>
> nav;

bool action::op2(Sz) {
  // syslog.setColors(GREEN,BLACK);
  // syslog.clear();
  syslog<<"option #2 action called.\ntoggle option #3 enable/disable state"<<endl;
  mainMenu.withId<ids::op3>().enable(!mainMenu.withId<ids::op3>().enabled());
  return true;
}

//==========================================================================================--
bool run() {
  static TinyTimeUtils::FPS<60> fps;
  if(fps) {
    fps.reset();
    nav.in(in);
    bool w=false;
    bool o=false;
    bool s=false;
    // if(nav.changed(web)) {}
    if(nav.changed(out)) {

      w=true;
      dout<<xy<0,24><<colors<RED,WHITE><<flush;
      web.clear();
      web.resume();
      nav.navPrint(web);

      o=true;
      out.resume();
      nav.navPrint(out);
      
    }
    if(syslog.changed()) {s=true;syslog.print();}
    if(s) syslog.sync();
    if(o) nav.sync(out);
    // if(w) nav.sync(web);

  }
  return running;
}

void setup(){
  #ifdef ARDUINO
    Serial.begin(115200);
    while(!Serial);
  #endif
  cout<<"ArduinoMenu R&D"<<endl;

  //populate std container menu
  mainMenu.withId<container>().body().push_back(new IItemDef<Text>{"runtime"});
  mainMenu.withId<container>().body().push_back(new IItemDef<Text>{"populated"});
  mainMenu.withId<container>().body().push_back(new IItemDef<Text>{"items"});

  //initialize outputs
  web.mode(LockMode::None);
  web.setColors(BLUE,WHITE);
  web.clear();
  nav.navPrint(web);
  footer.mode(LockMode::None);
  footer.setColors(BLUE,BLACK);
  footer.clear();
  footer.put("footer");
  syslog.mode(LockMode::None);
  syslog.setColors(GREEN,BLACK);
  syslog.clear();
  syslog.put(".·•<::(log)::>•·.");
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