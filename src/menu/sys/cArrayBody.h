#pragma once

#include "menu/sys/base.h"

template<typename T,T data[],Sz _sz>
struct CArrayBody {
  static constexpr Depth depth() {
    // Sz o{0};
    // for(Sz i=0;i<_sz;i++) if(data[i].depth()>o) o=data[i].depth();
    return 1;
  }
  static constexpr const Sz len() {return _sz;}
  static constexpr const Sz len(Sz i) {assert(i<_sz);return data[i];}

  template<bool kbd,typename Nav> 
  bool nav(Nav& n,CKE cke,Path path,Sz i) 
    {return data[i].template nav<kbd>(n,cke,path);}

  template<typename Out> bool printBody(Out& out,Ctx& ctx) {
    for(Sz i=0;i<_sz&&out.freeY();i++) out.printItem(data[ctx.idx],ctx);
    return false;
  }

  template<typename Out> bool printMenu(Out& out,Ctx& ctx,Sz i)
    {return data[i].printMenu(out,ctx);}


//Id, this is compile-time search/reference, but NOT here, all items are equal here --
  template<int> using HasId=std::integral_constant<bool,false>;
  template<int> using WithId=std::integral_constant<bool,false>;
};

template<typename T,T* data[],Sz _sz>
struct CPtrArrayBody {
  static constexpr Depth depth() {
    // Sz o{0};
    // for(Sz i=0;i<_sz;i++) if(data[i].depth()>o) o=data[i].depth();
    return 1;
  }
  static constexpr const Sz len() {return _sz;}
  static constexpr const Sz len(Sz i) {assert(i<_sz);return data[i]->len();}

  template<bool kbd,typename Nav> 
  bool nav(Nav& n,CKE cke,Path path,Sz i) 
    {return data[i]->template nav<kbd>(n,cke,path);}

  template<typename Out> bool printBody(Out& out,Ctx& ctx) {
    for(Sz i=0;i<_sz&&out.freeY();i++) out.printItem(*data[ctx.idx],ctx);
    return false;
  }

  template<typename Out> bool printMenu(Out& out,Ctx& ctx,Sz i)
    {return data[i]->printMenu(out,ctx);}


//Id, this is compile-time search/reference, but NOT here, all items are equal here --
  template<int> using HasId=std::integral_constant<bool,false>;
  template<int> using WithId=std::integral_constant<bool,false>;
};