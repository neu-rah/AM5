/**
 * @file nav.h
 * @author Rui Azevedo (neu-rah) (ruihfazevedo@gmail.com)
 * @brief 
 * @version 5
 * @date 2026-04-18
 * 
 * @copyright Copyright (c) 2026
 * 
*/

#pragma once

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
    // inline constexpr const M& root() const {return m_menu;}
  };
};

struct TreeNav {
  template<typename N>
  struct Part:N {
    using Base=N;
    using Root=typename Base::Root;
    using Base::root;
    // using Base::obj;
    static constexpr const Depth depth() {return Root::depth();}
    constexpr Path focus(Sz i) const {return m_path.focus(i);}
    constexpr Path self() const {return m_path.self();}
    constexpr Path parent() const {return m_path.parent();}
    Depth level() {return m_level;}
    Sz sel() {return m_path.sel(m_level);}

    void sync() {
      m_level.sync();
      m_navMode.sync();
      m_prevSel=sel();
    }

    template<typename Out>
    void sync(Out& out) {
      sync();
      LockMode om=out.mode();
      out.mode(LockMode::Sync);
      navPrint(out);
      out.mode(om);
    }

    bool changed() {
      return m_level.changed()
        ||m_navMode.changed()
        ||m_prevSel!=sel();
    }

    template<typename Out>
    bool changed(Out& out) {
      if(changed()) return true;
      LockMode om=out.mode();
      out.mode(LockMode::Changed);
      bool r=navPrint(out);
      out.mode(om);
      return r;
    }

    template<typename Out>
    bool navPrint(Out& out) {
      // dout.xy(0,1);dout<<"level:"<<(int)m_level.get()<<" print:"<<(int)m_print_level<<flush;
      ///track scroll top for each level, this is output device specific
      static Sz tops[root().depth()]{0};//TODO: check if ScrollBody is in output part, or store this there with an API call fallback.
      Ctx ctx{m_path.focus(m_level+1),m_navMode,m_print_level ,m_prevSel,tops};
      return root().printMenu(out,ctx);
    }

    template<bool kbd> 
    bool doCmd(Cmd cmd,Key k=0, bool e=false) {
      if(cmd==Cmd::Esc) return close();//preemptive esc=>close
      return root().template nav<kbd>(Base::obj(),{cmd,k,e},focus(m_level+1));
    }

    bool doNav(CKE cke,Sz len,Wraps w) {
      DataDef<NumRange,Data<Sz&>> at{0l,len-1,w,m_path.m_pathData[level()]};
      switch(cke.cmd) {
        case Cmd::Up: at.up();break;
        case Cmd::Down: at.down();break;
        default:break;
      }
      return true;
    }

    template<typename In>  bool in(In& in) {return in.cmd(Base::obj());}

    void go(Sz i,Depth delta=0) {m_path.m_pathData[m_level+delta]=i;}

    void navMode(NavMode m) {m_navMode=m;}
    NavMode navMode() const {return m_navMode.get();}

    bool padOpen() {
      if(m_level.get()<=depth()) {
        m_level.set(m_level+1);
        m_path.m_pathData[m_level]=0;
        if(m_level.get()<m_print_level) m_print_level=m_level;
        return true;
      } else return false;
    }
    bool open() {
      if(padOpen()) {
        m_print_level=m_level;
        return true;
      } else return false;
    }
    
    bool close() {
      if(m_level) {
        navMode(NavMode::Nav);
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
