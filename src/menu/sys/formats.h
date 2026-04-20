#pragma once

#include "menu/sys/base.h"

template<typename O>
struct Formats {
  using IsFormat=std::true_type;
  static_assert(O::template Excludes<IsPrinter>::value,"Printers must preseed Formats");
};