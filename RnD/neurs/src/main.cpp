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
  #ifdef __AVR__
    SerialOut,
  #else
    ConsoleOut,
  #endif
  StaticPos<0,20>,
  StaticArea<100,20>
> out;

OutDef<
  TextFmt,
  #ifdef __AVR__
    SerialOut,
  #else
    ConsoleOut,
  #endif
  StaticPos<20,30>,
  StaticArea<30,4>
> footer;


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

namespace desc {
  static constexpr const CText back{"return to previous menu."};
  static constexpr const CText quit{"terminate the program."};
  static constexpr const CText dot{"."};
  static constexpr const CText op1{"simple option with action function.\n`Enter` to activate function."};
  static constexpr const CText op2{"toggles Option 3\nenabled/disabled state."};
  static constexpr const CText op3{"simple option\ncan be enabled/disabled at runtime."};
  static constexpr const CText fields_menu{"some field examples..."};
  static constexpr const CText array_sub_menu{"pure C-Array as menu body, no virtual functions, all items the same"};
  static constexpr const CText sub_ibody{"pure C-Array with virtual derived items"};
  static constexpr const CText sub_sbody{"std::container with virtual items"};
};

template<typename... OO> using Desc=OnFocus<typename Put<OO...>::template ToOut<decltype(footer),footer,Clear::yes>>;
using Back=ItemDef</*CloseOnSelect,*/StaticText<text::back>,Desc<StaticText<desc::back>>>;
using Quit=ItemDef<Action<action::quit>,AsLabel<StaticText<text::quit>,Desc<StaticText<desc::quit>>>>;

auto fieldsMenu() {
  return menuDef<Wraps::yes>(
    ItemDef<Text,ItemNav>{"Fields"},
    staticBody(
      Back{}
    )
  );
}

auto tinyMenu=menuDef<Wraps::yes>(
  ItemDef<Text,ItemNav>{"Title"},
  staticBody(
    ItemDef<Text>{"yawn!"},
    ItemDef<Text>{"wtf!"},
    fieldsMenu(),
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
  // nav.go(2);
  // nav.enter();
  while(run());
  cout<<"end."<<endl;
  return 0;
}