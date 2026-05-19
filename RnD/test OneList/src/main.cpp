#include <menu.h>
#include <menu/IO/streamOut.h>
#include <menu/fmt/textFmt.h>

#include <oneList.h>
using namespace hapi::one_list;

#include <iostream>
using namespace std;

OutDef<TextFmt,ConsoleOut> out;

MenuDef<
  ItemDef<Text>,
  StaticBody<
    ItemDef<Id<1>,Text>,
    ItemDef<Id<2>,Text>,
    ItemDef<Id<3>,Text>
  >
> tmp("title",{"op1","op2","op3"});

auto body=menuDef(
  ItemDef<Text>{"title"},
  list(
    ItemDef<Id<1>,Text>{"Option 1"},
    ItemDef<Id<2>,Text>("Option 2"),
    ItemDef<Id<3>,Text>("Option 3")
  )
);

int main(){
  cout<< Chain<Id<1>,Text,ItemAPI<>>::template has<Id<1>> <<endl;
  cout<< Chain<Id<1>,Text>::template Part<ItemAPI<>>::template has<Id<1>> <<endl;
  cout<< ItemDef<Id<1>,Text>::template has<Id<1>> <<endl;

  cout<<tmp.template withId<Id<2>>()<<endl;
  cout<<body.template withId<Id<2>>()<<endl;
  cout<<"end."<<endl;
  return 0;
}