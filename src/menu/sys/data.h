/**
 * @file data.h
 * @author Rui Azevedo (ruihfazevedo@gmail.com)
 * @brief 
 * @version 5
 * @date 2026-04-17
 * 
 * @copyright Copyright (c) 2026
 * 
*/

#include <typedef.h>
#include "menu/sys/base.h"

struct DataPrint {
  template<typename O>
  struct Part:O {
    using Base=O;
    template<typename Out>
    void print(Out& out) {
      out.template fmt<Edge::start,Fmt::Data>();
      out.put(O::get());
      out.template fmt<Edge::stop,Fmt::Data>();
    }
  };
};
