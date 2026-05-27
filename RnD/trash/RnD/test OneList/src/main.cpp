#include <menu.h>
#include <menu/IO/streamOut.h>
#include <menu/fmt/textFmt.h>

#include <oneList.h>
using namespace hapi::one_list;

#include <iostream>
using namespace std;

OutDef<FullPrinter,TextFmt,ConsoleOut> out;

namespace text{
  static constexpr const CText quit{"Exit!"};
  static constexpr const CText yes{"Yes"};
  static constexpr const CText no{"No"};
};

namespace action {
  bool quit(Sz) {return true;}
};

using Quit=ItemDef<Action<action::quit>,AsLabel<StaticText<text::quit>>>;

using ToggleDemo=ToggleFieldDef<
  ItemDef<Text,AsEditMode<>>,
  StaticBody<
    // ItemDef<AsField<StaticText<text::no>>>,
    // ItemDef<AsField<StaticText<text::yes>>>,
    ItemDef<AsField<Text>>
  >//,BodyAction<action::subIdx>
>;

MenuDef<
  ItemDef<Text>,
  StaticBody<
    ItemDef<Id<1>,Text>,
    ItemDef<Id<2>,Text>,
    ItemDef<Id<3>,Text>
  >
> tmp("title",{"op1","op2","op3"});

auto menu=menuDef(
  ItemDef<Text>{"title"},
  staticBody(
    ItemDef<Id<1>,Text>{"Option 1"},
    ItemDef<Id<2>,Text>("Option 2"),
    ItemDef<Id<3>,Text>("Option 3"),
    ToggleDemo{"Toggle",{"Maybe"}},
    Quit{}
  )
);

int main(){
  // cout<< Chain<Id<1>,Text,ItemAPI<>>::template has<Id<1>> <<endl;
  // cout<< Chain<Id<1>,Text>::template Part<ItemAPI<>>::template has<Id<1>> <<endl;
  // cout<< ItemDef<Id<1>,Text>::template has<Id<1>> <<endl;

  // cout<<tmp.template withId<Id<2>>()<<endl;
  // cout<<menu.template withId<Id<2>>()<<endl;

  PathData<1> path{0};
  Ctx ctx{path};
  menu.printMenu(out,ctx);

  cout<<"end."<<endl;
  return 0;
}