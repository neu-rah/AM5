#include <menu.h>

#include <iostream>
using namespace std;

namespace id {enum {
  cancel,ok
};};

namespace act {
  bool ok(int i) {cout<<i<<" proceeding..."<<endl;return true;}
  bool cancel(int i) {cout<<i<<" cancel = default nav (possibly close)"<<endl;return false;}
};

ItemDef<Id<id::ok>,AsLabel<Text>,EnDis<false>,Action<act::ok>> ok("ok");
ItemDef<Id<id::cancel>,AsLabel<Text>,Action<act::cancel>> cancel{"cancel"};

int main(){
  cout<<ok.get()<<endl;
  cout<<cancel<<endl;
  ok.act(1);
  cancel.act(0);
  ok.enable(false);
  ok.act(1);
  cout<<"end."<<endl;
  return 0;
}