#include <menu.h>

#include <iostream>
using namespace std;

using Sp=StaticData<char,' '>;
using NL=StaticData<char,'\n'>;

bool op1(int i) {return true;}

auto menu=menuDef(
  ItemDef<Text,NL>{"Title"},
  staticBody(
    ItemDef<Text,NL,Action<op1>>{"Option 1"},
    ItemDef<Text,NL>{"Option 2"},
    ItemDef<Text,NL>{"Option 3"},
    ItemDef<Text,NL>{"Quit"}
  )
);

int main() {
  menu.printMenu(cout);
  cout<<endl;
  return 0;
}