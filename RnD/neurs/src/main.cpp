#include <menu.h>
#include <menu/IO/streamOut.h>

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

OutDef<
  ConsoleOut
> out;



int main(){
  out.put("AM5 R&D");
  out.nl();
  return 0;
}