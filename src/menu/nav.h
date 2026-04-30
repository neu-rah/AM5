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

    Path focus(Sz i) {return m_path.focusAt(i);}
    Depth level() const {return m_level;}
    Sz sel() const {return m_path[m_level];}

    void navMode(NavMode m) {m_navMode.set(m);}
    const NavMode navMode() const {return m_navMode.get();}

    void sync() {
      m_level.sync();
      m_navMode.sync();
      m_prevSel=sel();
    }

    template<typename Out>
    void sync(Out& out) {
      sync();
      LockMode om=out.lockMode();
      out.lockMode(LockMode::Sync);
      printTo(out);
      out.lockMode(om);
    }

    bool changed() const {
      return m_level.changed()
        ||m_navMode.changed()
        ||m_prevSel!=sel();
    }

    template<typename Out>
    bool changed(Out& out) {
      if(changed()) return true;
      LockMode om=out.lockMode();
      out.lockMode(LockMode::Changed);
      bool r=printTo(out);
      out.lockMode(om);
      return r;
    }

    template<typename Out>
    bool printTo(Out& out) {
      ///track scroll top for each level, this is output device specific
      static Sz tops[root().depth()]{0};//TODO: check if ScrollBody is in output part, or store this there with an API call fallback.
      Ctx ctx{m_path.focusAt(m_level+1),m_navMode,m_print_level,true,tops};
      return root().printMenu(out,ctx);
    }
    bool doCmd(Cmd cmd,Key k=0, bool e=false) {
      if(cmd==Cmd::Esc) return close();//preemptive esc=>close
      dout<<xy<0,2><<colors<BLUE,WHITE><<focus(m_level+1)<<padWith<10><<flush;
      return root().nav(Base::obj(),{cmd,k,e},focus(m_level+1));
    }

    bool doNav(CKE cke,Sz len,Wraps w) {
      DataDef<NumRange<Sz>,Data<Sz&>> at(0,len-1,w,m_path.data[(int)level()]);
      switch(cke.cmd) {
        case Cmd::Up: at.up();break;
        case Cmd::Down: at.down();break;
        default:break;
      }
      return true;
    }

    template<typename In>  bool in(In& in) {return in.cmd(Base::obj());}

    void go(Sz i,Depth delta=0) {
      assert(m_level+delta<depth());
      m_path.data[m_level+delta]=i;
    }

    bool padOpen() {
      dout<<xy<0,4><<colors<RED,BLACK><<"Nav::padOpen ";
      if(m_level.get()<depth()) {
        dout<<(int)m_level<<"->"<<flush;
        m_level.set(m_level+1);
        m_path.data[m_level]=0;
        if(m_level.get()<m_print_level) m_print_level=m_level;
        dout<<(int)m_level<<"... "<<flush;
        dout<<"|"<<cnt<>++<<padWith<10><<flush;
        return true;
      } else {
        dout<<"|"<<cnt<>++<<padWith<10><<flush;
        return false;
      }
    }
    bool open() {
      dout<<xy<0,3><<colors<RED,BLACK><<"Nav::open |"<<cnt<>++<<padWith<10><<flush;
      if(padOpen()) {
        m_print_level=m_level;
        return true;
      } else return false;
    }
    
    bool close() {
      navMode(NavMode::Nav);
      if(m_level) {
        m_level.set(m_level-1);
        if(m_print_level>m_level) m_print_level=m_level;
        return true;
      } else return false;
    }

  protected: 
    Sz m_prevSel{};
    PathData<depth()> m_path{};
    DataDef<Watch<Data<Depth>>> m_level{0};
    Depth m_print_level{0};
    DataDef<Watch<Data<NavMode>>> m_navMode{NavMode::Nav};
  };
};