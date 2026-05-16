#include <menu.h>

#include <iostream>
using namespace std;

Data<const char*>::template Part<Nil> t("ok");

int main(){
  cout<<t<<endl;
  cout<<"end."<<endl;
  return 0;
}