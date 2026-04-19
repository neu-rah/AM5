#pragma once

enum {no=false,yes=true};
enum class Edge {start,stop};
enum class Wraps:bool {no=false,yes=true};

enum class NavMode {Nav,Edit,Tune};

enum class Cmd {Enter,Esc,Up,Down,Left,Right,Key,Go};

enum class Fmt {
  View,Title,Menu,Body,Item,
  Index,Accel,NavCursor,
  Field,Label,EditMode,EditCursor,Data,Unit
};

/// @brief lock/unlock print output
enum class LockMode {
  None,//normal output
  Update,//draw only changed
  Sync,//just sync the visible values
  Measure,//normal lock, no output, just cursor movement calculation (no real cursor moved)
  Changed//check if any visible items changed
};

#ifdef MENU_DEBUG

  template<typename Out>
  Out& operator<<(Out& out,const Wraps& w) {
    switch(w) {
      case Wraps::no:return out<<"Wraps::no";break;
      case Wraps::yes:return out<<"Wraps::yes";break;
    }
  }

  template<typename Out>
  Out& operator<<(Out& out,const NavMode& mode) {
    switch(mode) {
      case NavMode::Nav: return out<<"NavMode::Nav";break;
      case NavMode::Edit: return out<<"NavMode::Edit";break;
      case NavMode::Tune: return out<<"NavMode::Tune";break;
      default: return out<<"NavMode::?";break;
    }
  };

  template<typename Out>
  Out& operator<<(Out& out,const Cmd& cmd) {
    switch(cmd) {
      case Cmd::Enter:return out<<"Cmd::Enter";
      case Cmd::Esc:return out<<"Cmd::Esc";
      case Cmd::Up:return out<<"Cmd::Up";
      case Cmd::Down:return out<<"Cmd::Down";
      case Cmd::Left:return out<<"Cmd::Left";
      case Cmd::Right:return out<<"Cmd::Right";
      case Cmd::Key:return out<<"Cmd::Key";
      case Cmd::Go:return out<<"Cmd::Go";
      default: return out<<"Cmd?";
    }
  };

  template<typename Out>
  Out& operator<<(Out& out,const Fmt& tag) {
    switch(tag) {
      case Fmt::View: return out<<"View";
      case Fmt::Title: return out<<"Title";
      case Fmt::Menu: return out<<"Menu";
      case Fmt::Body: return out<<"Body";
      case Fmt::Item: return out<<"Item";
      case Fmt::Index: return out<<"Index";
      case Fmt::Accel: return out<<"Accel";
      case Fmt::NavCursor: return out<<"NavCursor";
      case Fmt::Field: return out<<"Field";
      case Fmt::Label: return out<<"Label";
      case Fmt::EditMode: return out<<"EditMode";
      case Fmt::EditCursor: return out<<"EditCursor";
      case Fmt::Data: return out<<"Data";
      case Fmt::Unit: return out<<"Unit";
      default: return out<<"Fmt::?";
    }
  }

  template<typename Out>
  Out& operator<<(Out& out, LockMode m) {
    switch(m){
      case LockMode::None:return out<<"None";
      case LockMode::Update:return out<<"Update";
      case LockMode::Sync:return out<<"Sync";
      case LockMode::Measure:return out<<"Measure";
      case LockMode::Changed:return out<<"Changed";
      default: return out<<"LockMode::?";
    }
  }

  #endif