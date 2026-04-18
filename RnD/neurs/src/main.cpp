// includes --
  #include <menu.h>
  #include <tinyTimeUtils.h>

  #ifdef ARDUINO
    #include <Arduino.h>
  #endif

  #ifdef __AVR__
    #include <avr_std.h>
    #include <streamFlow.h>
    using namespace StreamFlow;
    #define cout Serial
  #else
    #include <iostream>
    #include <type_traits>
    #include <cstring>
    using namespace std;
  #endif
  using namespace std;

OutDef<ConsoleOut> out;

int power=55;

TypeDef<int>
  // ::Value//hold data internally (functions get/set) or use
  ::Ref<power>//as a static reference to external data, functions get/set
  ::template StaticNumRange<0,100>//valid range info
  ::Step<Wrap::yes> //step the value (assuming valid), functions up/down
  n;

ItemDef<Data<TypeDef<int>::Value>> item1{1967};

void run() {
  n.valid();
  cout<<n<<endl;
  n.up(5);
  cout<<n<<endl;
  n.up(50);
  cout<<n<<endl;
  item1.print(out);
  cout<<endl;
}

void setup(){
  #ifdef ARDUINO
    Serial.begin(115200);
    while(!Serial);
  #endif
}

#ifdef ARDUINO
  void loop() {run();}
#else
  int main() {setup();run();}
#endif