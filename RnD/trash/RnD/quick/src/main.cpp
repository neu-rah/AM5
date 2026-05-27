// includes --
  #include <menu.h>
  #include <menu/IO/ansiOut.h>
  #include <menu/fmt/textFmt.h>
  #include <menu/fmt/ansiFmt.h>
  #include <menu/fmt/xmlFmt.h>
  #include <menu/IO/pcKbdIn.h>
  #include <menu/body/cArrayBody.h>
  #include <menu/body/stdBody.h>

  #include <tinyTimeUtils.h>
  using namespace TinyTimeUtils;
  
  #ifdef __AVR__
    #include <menu/IO/arduino/serialOut.h>
    #include <menu/IO/arduino/serialIn.h>
    #include <streamFlow.h>
    using namespace StreamFlow;
    #define cout Serial
  #else
    #include <vector>
    #include <menu/IO/streamOut.h>
    #include <menu/IO/linuxKeyIn.h>
  #endif
  using namespace std;

bool running=true;

ItemDef<
  AsEditMode<>,
  Int
> tmp{1};

//================================================================================================--
bool run() {
  // static TinyTimeUtils::FPS<120> fps;
  // if(fps) {
  //   fps.reset();
  //   nav.in(in);
  //   if(nav.changed(out)) {
  //     out.resume();
  //     nav.printTo(out);
  //     nav.sync(out);
  //   }
  // }
  // if(!fps) ms_delay(fps.when()-TinyTimeUtils::timeSrc());

  return running;
}

void setup() {
  cout<<"AM5 R&D"<<endl;

  // out.lockMode(LockMode::None);
  // out.setColors(WHITE,BLACK);
  // out.clear();
  // nav.printTo(out);
}

int main(){
  setup();
  while(run());
  // dout<<xy<0,50><<"end."<<endl;
  return 0;
}