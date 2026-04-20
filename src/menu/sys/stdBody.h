#pragma once

template<typename T>
struct StdBody:T {
  // T data;
  static constexpr Depth depth() {return 1;}
  constexpr Sz len() const {return T::size();}
  constexpr Sz len(Sz i) const {return T::operator[](i).len();}

  template<bool kbd,typename Nav> 
  bool nav(Nav& n,CKE cke,Path path,Sz i) 
    {return T::operator[](i)->template nav<kbd>(n,cke,path);}

  template<typename Out> bool printBody(Out& out,Ctx& ctx) {
    for(auto i=T::begin();i<T::end()&&out.freeY();i++) out.printItem(**i,ctx);
    return false;
  }

  template<typename Out> bool printMenu(Out& out,Ctx& ctx,Sz i)
    {return T::operator[](i)->printMenu(out,ctx);}

//Id, this is compile-time search/reference, but NOT here, all items are virtual here --
  template<int> using HasId=std::integral_constant<bool,false>;
  template<int> using WithId=std::integral_constant<bool,false>;
};