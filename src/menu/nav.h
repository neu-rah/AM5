/**
 * @file nav.h
 * @author Rui Azevedo (neu-rah) (ruihfazevedo@gmail.com)
 * @brief menu navigation API
 * @version 5
 * @date 2026-04-28
 * 
 * @copyright Copyright (c) 2026
 * 
*/

#pragma once

#include "menu/sys/data.h"

template<typename N> struct NavAPI:N {};

template <typename API,typename... NN>
struct DefinedNav:APIOf<API, NN...> {
  using Base = APIOf<API, NN...>;
  bool up() {return Base::doCmd(Cmd::Up);}
  bool down() {return Base::doCmd(Cmd::Down);}
  bool enter() {return Base::doCmd(Cmd::Enter);}
  bool esc() {return Base::doCmd(Cmd::Esc);}
};

template<typename... II>
struct NavDef:DefinedNav<NavAPI<CRTP<NavDef<II...>>>,II...> {};

struct INav {};

template<typename... II>
struct INavDef:INav,DefinedNav<NavAPI<CRTP<INavDef<II...>>>,II...> {
  using Base=DefinedNav<NavAPI<CRTP<INavDef<II...>>>,II...>;
};

template<typename T,T& menu>
struct Root {
  template<typename N>
  struct Part:N {
    using Base=N;
    using Root=std::remove_reference_t<decltype(menu)>;
    static constexpr Root& root() {return menu;}
    static constexpr const Depth depth() {return Root::depth();}
  };
};

template<typename M>
struct StaticRoot {
  template<typename N>
  struct Part:N {
    using Base=N;
    M m_menu;
    inline constexpr M& root() {return m_menu;}
    static constexpr const Depth depth() {return M::depth();}
  };
};

struct TreeNav {
  template<typename N>
  struct Part:N {
    using Base=N;
    using Root=typename Base::Root;
    using Base::root;
    using Base::depth;
    template<typename Out>
    bool printTo(Out& out) {
      ///track scroll top for each level, this is output device specific
      static Sz tops[root().depth()]{0};//TODO: check if ScrollBody is in output part, or store this there with an API call fallback.
      Ctx ctx{m_path.focusAt(m_level+1),m_navMode,m_print_level,true,tops};
      return root().printMenu(out,ctx);
    }
  protected: 
    Sz m_prevSel{};
    PathData<depth()> m_path{};
    DataDef<Watch<Data<Depth>>> m_level{0};
    Depth m_print_level{0};
    DataDef<Watch<Data<NavMode>>> m_navMode{NavMode::Nav};
  };
};