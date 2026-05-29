#include <menu.h>
#include <oneOut.h>

#include <iostream>
using namespace std;

/// static space character
using Sp=StaticData<char,' '>;
/// static newline character
using NL=StaticData<char,'\n'>;

/// a title = text + newline
template<typename... OO>
using Title=ItemDef<Text,NL,OO...>;

/// a prompt = Text + newline + action function
template<ActionFunc func,typename... OO>
using Prompt=ItemDef<Text,NL,Action<func>,OO...>;

/// some action functions
bool op1(int i)  {cout<<"op1";return true;}
bool op2(int i)  {cout<<"op2";return true;}
bool op3(int i)  {cout<<"op3";return true;}
bool quit(int i) {cout<<"quit";return false;}

struct FlatNav {
  Sz at;
  template<typename O>
  struct Part:O {
    using Base=O;
    template<typename Out,typename... PP>
    void print(Out& out,int i,const PP&&... pp) const 
      {out.put(i==at?'>':' ');Base::print(out,i,std::forward<PP>(pp)...);}
  };
};

auto menu=menuDef<Id<1>>(
  Title<Id<2>>{"Main menu"},
  staticBody(
    Prompt<op1,Id<11>>{"Option 1"},
    Prompt<op2,Id<12>>{"Option 2"},
    Prompt<op3,Id<13>>{"Option 3"},
    Prompt<quit,Id<10>>{"Quit"}
  )
);

OutDef<ConsoleOut> out;

int main() {
  cout<<std::boolalpha;
  menu.printMenu(out);
  cout<<"has id 1:"<<query<SameAs<Id<1>>,decltype(menu)><<endl;
  cout<<"has id 2:"<<query<SameAs<Id<2>>,decltype(menu)><<endl;
  cout<<"has id 3:"<<query<SameAs<Id<3>>,decltype(menu)><<endl;
  cout<<"has id 13:"<<query<SameAs<Id<13>>,decltype(menu)><<endl;
  return 0;
}