#ifdef __AVR__
  #include <Arduino.h>
#else
  #include <menu.h>
  #include <iostream>
  using namespace std;
#endif

template<int n> using CLit=char[n];

template<int n> 
struct Lit{
  const CLit<n>& text;
  constexpr Lit(const CLit<n>& o):text{o}{}
};

template<Lit name> struct A {};

#ifdef ARDUINO
  void setup() {
    Serial.begin(115200);
    while(!Serial);
  }
  void loop() {
    Serial.println(literal("ok").value);
    delay(3000);
  }
#else
  int main() {
    cout<<Lit{"ok"}.text<<endl;
    return 0;
  }
#endif