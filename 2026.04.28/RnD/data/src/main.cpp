#include <iostream>
using namespace std;

#include <hapi.h>

struct Nil {};

//==================================================================--
const char* nome{"Rui"};
int ano{1967};
char notas[]{"Notes..."};

using A=DataDef<StaticData<int,11>>;
using B=DataDef<StaticRef<const char*,nome>>;
using C=DataDef<Watch,StaticRef<int,ano>>;
using D=DataDef<StaticRef<decltype(notas),notas>>;

DataDef<Watch,Data<int>> month{11};

int main() {
  month.sync();
  cout<<"month changed:"<<month.changed()<<endl;
  cout<<A::get()<<endl;
  cout<<B::get()<<endl;
  C::get()=12;
  cout<<C::get()<<endl;
  D::get()[0]='V';
  cout<<D::get()<<endl;
  month.get()=21;
  cout<<month.get()<<endl;
  cout<<"month changed:"<<month.changed()<<endl;
  return 0;
}