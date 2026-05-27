#include <menu.h>
#include <menu/IO/streamOut.h>
#include <menu/fmt/textFmt.h>

#include <iostream>
using namespace std;

namespace id {enum {
  cancel,ok
};};

namespace act {
  bool ok(int i) {cout<<i<<" proceeding..."<<endl;return true;}
  bool cancel(int i) {cout<<i<<" cancel = default nav (possibly close)"<<endl;return false;}
};

// ItemDef<Id<id::ok>,AsLabel<Text>,EnDis<false>,Action<act::ok>> ok("ok");
// ItemDef<Id<id::cancel>,AsLabel<Text>,Action<act::cancel>> cancel{"cancel"};
// int val=0;
// DataDef<NumRange<int&>,Data<int&>> tmp{0,3,true,val};

OutDef<TextFmt,ConsoleOut> out;

int main(){
  // auto tmp=menuDef(
  //   Title<Text>{"title"},
  //   staticBody(
  //     ItemDef<AsLabel<Text>>{"text"}
  //   )
  // );
  ItemDef<Text> i{"text"};
  operator<<(cout,i);
  cout<<endl;
  i.print(out);
  cout<<endl;
  // tmp.body().m_item.print(out);
  // cout<<tmp.body().m_item<<endl;
  cout<<"end."<<endl;
  return 0;
}