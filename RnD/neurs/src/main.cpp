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
  // ::Data//hold data internally or use
  ::Ref<power>//as a static reference to external data
  ::template StaticNumRange<0,100>//valid range info
  ::Step<Wrap::yes> //step the value (assuming valid)
  n;

void run() {
  n.valid();
  cout<<n<<endl;
  n.up(5);
  cout<<n<<endl;
  n.up(50);
  cout<<n<<endl;
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