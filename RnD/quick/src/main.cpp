// includes --
  #include <menu.h>
  #include <menu/IO/ansiOut.h>
  #include <menu/fmt/textFmt.h>
  #include <menu/fmt/ansiFmt.h>
  #include <menu/fmt/xmlFmt.h>
  #include <menu/IO/pcKbdIn.h>
  #include <menu/body/cArrayBody.h>
  #include <menu/body/stdBody.h>

  #include <tinyTimeUtils.h>
  using namespace TinyTimeUtils;
  
  #ifdef __AVR__
    #include <menu/IO/arduino/serialOut.h>
    #include <menu/IO/arduino/serialIn.h>
    #include <streamFlow.h>
    using namespace StreamFlow;
    #define cout Serial
  #else
    #include <vector>
    #include <menu/IO/streamOut.h>
    #include <menu/IO/linuxKeyIn.h>
  #endif
  using namespace std;

bool running=true;

InDef<
  #ifdef ARDUINO
    SerialIn,
  #else
    LinuxKeyIn,
  #endif
  PCKbd
> in;

OutDef<
  ScrollPrinter,//menu parts to use
  ANSIFmt,//add some ANSI colors and format to the output
  // TextFmt,
  ClearFreeFmt,//this can take a lot of burden away from user format
  DataParser<>,//put all data into characters
  CtrlChars,
  UTF8,//bypass UTF8 surrogate codes
  TextWrap,//long texts continue next line
  Clip,//keep content inside area
  ColorTrack<int>,//track color setting for device resume...
  Cursor,//track cursor position for resume...
  Gate,
  ANSIOut,//inject ansi codes into the next output device
  #ifdef __AVR__
    SerialOut,
  #else
    ConsoleOut,
  #endif
  StaticPos<20,10>,
  StaticArea<30,8>
> out;

namespace text {
  static constexpr const CText main_menu{"Main menu"};
  static constexpr const CText back{"<Back"};
  static constexpr const CText quit{"Exit!"};
  static constexpr const CText op1{"Option 1"};
  static constexpr const CText op2{"Option 2"};
  static constexpr const CText op3{"Option 3"};
  static constexpr const CText fields_menu{"Fields..."};
  static constexpr const CText array_sub_menu{"Same type array>"};
  static constexpr const CText sub_ibody{"IItem* array"};
  static constexpr const CText sub_sbody{"std::container"};
  static constexpr const CText sub1{"10"};
  static constexpr const CText sub2{"40"};
  static constexpr const CText sub3{"60"};
  static constexpr const CText sub4{"80"};
  static constexpr const CText sub5{"100"};
  static constexpr const CText power{"Power"};
  static constexpr const CText percent{"%"};
  static constexpr const CText off{"Off"};
  static constexpr const CText on{"On"};
  static constexpr const CText yes{"Yes"};
  static constexpr const CText no{"No"};
  static constexpr const CText toggle_demo{"Toggle"};
  static constexpr const CText select_demo{"Select"};
  static constexpr const CText choose_demo{"Choose"};
  static constexpr const CText dateSep{"."};
};

enum ids {op3,power,container,date_fld};

namespace action {
  bool op1(Sz) {
    dout<<xy<0,1><<colors<BLUE,BLACK><<padWith<80><<xy<0,1>;
    dout<<"option #1 action called."<<flush;
    out.resume();
    return true;
  }
  bool op2(Sz);
  bool op3(Sz) {
    dout<<xy<0,1><<colors<BLUE,BLACK><<padWith<80><<xy<0,1>;
    dout<<"option #3 action called."<<flush;
    out.resume();
    return true;
  }
  bool quit(Sz) {
    dout<<xy<0,1><<colors<RED,BLACK><<padWith<80><<xy<0,1>;
    dout<<"Bye!"<<endl;
    running=false;
    return true;
  }
  bool subIdx(Sz i) {
    dout<<xy<0,1><<colors<CYAN,BLACK><<padWith<80><<xy<0,1>;
    dout<<"sub option #"<<i<<" selected."<<flush;
    out.resume();
    return false;
  }
}

using Back=ItemDef<StaticText<text::back>>;
using Quit=ItemDef<Action<action::quit>,AsLabel<StaticText<text::quit>>>;

using CItem=ItemDef<Text>;

CItem cBody[]{
  "..",
  ".pio/",
  "src/",
  "all of the same type.txt",
  "platformio.ini",
  "README.md"
};

bool stay(int i) {
  dout<<xy<0,1><<colors<RED,BLACK><<padWith<80><<xy<0,1>;
  dout<<"stay function called. The menu remains, not closing as default."<<flush;
  out.resume();
  return true;
}

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
  WrapNav,
  BodyAction<action::subIdx>
>;

using SelectDemo=SelectFieldDef<
  Title<
    AsLabel<StaticText<text::select_demo>>,
    AsEditMode<>,//edit mode indicator
    BodyAction<action::subIdx>
  >,
  StaticBody<//sub menu static body
    ItemDef<AsField<StaticText<text::sub1>>,AsUnit<StaticText<text::percent>>>,
    ItemDef<AsField<StaticText<text::sub2>>,AsUnit<StaticText<text::percent>>>,
    ItemDef<AsField<StaticText<text::sub3>>,AsUnit<StaticText<text::percent>>>,
    ItemDef<AsField<StaticText<text::sub4>>,AsUnit<StaticText<text::percent>>>,
    ItemDef<AsField<StaticText<text::sub5>>,AsUnit<StaticText<text::percent>>>
  >,
  WrapNav,
  BodyAction<action::subIdx>
>;

//a Map operation, this operation will fit Map<> and will insert OO... components into the chain
template<typename... OO> struct Ins {template<typename M>  using Map=typename M::template Ins<OO...>;};
template<typename... OO> struct App {template<typename M>  using Map=typename M::template App<OO...>;};

using ToggleDemo=ToggleFieldDef<
  ItemDef<Text,AsEditMode<>>,
  StaticBody<
    ItemDef<AsField<StaticText<text::no>>>,
    ItemDef<AsField<StaticText<text::yes>>>,
    ItemDef<AsField<Text>>
  >,
  BodyAction<action::subIdx>
>;

using Power=NumFieldDef<
  Chain<
    Id<ids::power>,
    AsLabel<StaticText<text::power>>//field label
  >,
  NumField<//use range to change the data
    StaticNumRange<int,0,100,false>,//valid range
    ItemNav,
    Watch<AsField<Default<int,55>,Int>>//use `int` and `change watch` as field (data)
  >,
  AsUnit<StaticText<text::percent>>//field unit
>;

  // date field generating function
auto dateField(const char*lbl)
#ifdef __AVR__
->decltype(
  padDef(
    ItemDef<AsLabel<Text>,AsEditMode<>>{lbl},
    staticBody(
      ItemDef<//lets define a numeric field:
        EditField,//use nav keys up/down to change numeric value within range
        ParentDraw,//draw inplace
        EnDis<false>,
        AsEditMode<>,//edit mode indicator (format)
        ItemNav,//open nav level for this item on Cmd::Enter
        NumField<StaticNumRange<int,1900,2150,true>,//static numeric range
        Watch<AsField<Default<int,2026>,Int>>>//watch for changes, format an Int (Data<int>) as field with default value 2026
      >{2026},
      ItemDef<
        StaticText<text::dateSep>,
        EditField,
        ParentDraw,
        AsEditMode<>,
        ItemNav,
        NumField<StaticNumRange<int,1,12,true>,
        Watch<AsField<Int>>>
      >{1},
      ItemDef<
        StaticText<text::dateSep>,
        EditField,
        ParentDraw,
        AsEditMode<>,
        ItemNav,
        NumField<StaticNumRange<int,1,31,true>,
        Watch<AsField<Int>>>
      >{1}
    )
  )
)
#endif
{
  return padDef(
    ItemDef<AsLabel<Text>,AsEditMode<>>{lbl},
    staticBody(
      ItemDef<//lets define a numeric field:
        EditField,//use nav keys up/down to change numeric value within range
        ParentDraw,//draw inplace
        EnDis<false>,
        AsEditMode<>,//edit mode indicator (format)
        ItemNav,//open nav level for this item on Cmd::Enter
        NumField<StaticNumRange<int,1900,2150,true>,//static numeric range
        Watch<AsField<Default<int,2026>,Int>>>//watch for changes, format an Int (Data<int>) as field with default value 2026
      >{2026},
      ItemDef<
        StaticText<text::dateSep>,
        EditField,
        ParentDraw,
        AsEditMode<>,
        ItemNav,
        NumField<StaticNumRange<int,1,12,true>,
        Watch<AsField<Int>>>
      >{1},
      ItemDef<
        StaticText<text::dateSep>,
        EditField,
        ParentDraw,
        AsEditMode<>,
        ItemNav,
        NumField<StaticNumRange<int,1,31,true>,
        Watch<AsField<Int>>>
      >{1}
    )
  );
}

auto mainMenu=menuDef<WrapNav>(
  ItemDef<Text>{"Main menu"},
  staticBody(
    ItemDef<Action<action::op1>,StaticText<text::op1>>{},
    ItemDef<Action<action::op2>,StaticText<text::op2>>{},
    ItemDef<Id<ids::op3>,Action<action::op3>,Watch<EnDis<false>>,StaticText<text::op3>>{},
    menuDef<WrapNav>(
      Title<StaticText<text::fields_menu>>{},
      staticBody(
        ItemDef<//text edit field demo
          AsLabel<Text>,
          AsEditMode<>,
          EditField,
          ParentDraw,
          // ItemNav,
          AsField<TextField<15>>
        >{"Name"},
        Power{},
        ToggleDemo{"Toggle","Maybe"},
        SelectDemo{},
        ChooseDemo{},
        dateField("date"),
        Back{}
      )
    ),
    MenuDef<//sub menu with C array body (all items of the same type)
      Title<
        BodyAction<action::subIdx>,
        StaticText<text::array_sub_menu>
      >,
      CArrayBody<CItem,cBody,sizeof cBody/sizeof *cBody>
    >{},
    Quit{}
  )
);

NavDef<
  TreeNav,
  Root<decltype(mainMenu),mainMenu>
> nav;

bool action::op2(Sz) {
  dout<<xy<0,1><<colors<GREEN,BLACK><<padWith<80><<xy<0,1>;
  dout<<"option #2 action called. Toggle option #3 enable/disable state"<<endl;
  mainMenu.withId<ids::op3>().enable(!mainMenu.withId<ids::op3>().enabled());
  return true;
}

//================================================================================================--
bool run() {
  static TinyTimeUtils::FPS<120> fps;
  if(fps) {
    fps.reset();
    nav.in(in);
    if(nav.changed(out)) {
      out.resume();
      nav.printTo(out);
      nav.sync(out);
    }
  }
  if(!fps) ms_delay(fps.when()-TinyTimeUtils::timeSrc());

  return running;
}

void setup() {
  cout<<"AM5 R&D"<<endl;

  out.lockMode(LockMode::None);
  out.setColors(WHITE,BLACK);
  out.clear();
  nav.printTo(out);
}

int main(){
  setup();
  while(run());
  // dout<<xy<0,50><<"end."<<endl;
  return 0;
}