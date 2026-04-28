#pragma once

/**
 * @file pcKbdIn.h
 * @author Rui Azevedo (ruihfazevedo@gamil.com)
 * @brief PC keyboard extended codes parser
 * @version 5
 * @date 2024-11-13
 * 
 */

#include "menu/in.h"
#include "tinyTimeUtils.h"

/// @brief handles PC keyboard extended codes (arrow keys) and translates them to menu commands
struct PCKbd {
  template<typename O>
  struct Part:O {
    template<typename Nav>//needed to cleanup delayed values
    static inline bool cmd(Nav& nav) {return parseKey(nav,0);}
    template<typename Nav>
    static bool parseKey(Nav& nav,Key k) {
      static Key esc=0;
      static Key term=0;
      static TinyTimeUtils::Timeout<30> escTimer;
      if (esc==1 && escTimer&&!k) {
        esc = 0;
        return nav.doCmd(Cmd::Esc,0,false);
      }
      if(k==term||!k) return false;
      if (esc==1) {
        if (k == '[') {
          esc = 2;
          return false;
        } else {
          esc=0;
          escTimer.reset();
          return nav.doCmd(Cmd::Esc)&&
                 nav.doCmd(Cmd::Key,k);
        }
      } else if(esc==2) {
        term=0x7E;
        esc=0;
        switch(k) {
          case 'A':return nav.doCmd(Cmd::Down);
          case 'B':return nav.doCmd(Cmd::Up);
          case 'C':return nav.doCmd(Cmd::Left);
          case 'D':return nav.doCmd(Cmd::Right);
          default: 
            esc=0;
            return O::parseKey(nav,k)||nav.doCmd(Cmd::Key,k,true);
        }
      } else {
        esc=0;
        if(k==0x1B) {// \e
          esc=1;
          escTimer.reset();
          return false;
        } else {
          esc=0;
          switch (k) {
          case 0x1B:
          case 0xA:
            term=0xD;
            return nav.doCmd(Cmd::Enter);
          case 0xD:
            term=0xA;
            return nav.doCmd(Cmd::Enter);
          default:
            return nav.doCmd(Cmd::Key,k)||O::parseKey(nav,k);
          }
        }
      }
    }
  };
};

