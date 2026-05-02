#pragma once

#include "menu/sys/base.h"

struct Formats {
  template<typename O>
  struct Part:UseEditCursorFmt::template Part<O> {
    using IsFormat=std::true_type;
    static_assert(O::template Excludes<IsPrinter>::value,"Printers must preseed Formats");//TODO:: then chop OutDef into Printers|Fmt|Dev
  };
};

struct ClearFreeFmt {
  template<typename O>
  struct Part:O {
    static_assert(O::template Requires<IsCursor>::value,"ClearFree needs a valid Cursor (IsCursor class) not the API default fallback");
    using Base=O;
    using Base::fmtStart;
    using Base::fmtStop;
    void clearLine() {Base::padWith(Base::freeX());Base::nl();}
    void clearFree() {do clearLine(); while(Base::freeY());}
    template<Fmt tag> std::enable_if_t<tag&Fmt::View> fmtStop(const Ctx& ctx) {clearFree();}
    template<Fmt tag> std::enable_if_t<tag&(Fmt::Title|Fmt::Item)> fmtStop(const Ctx& ctx) {clearLine();}
  };
};

struct UseFmtFuncs {
  template<typename O>
  struct Part:Formats::template Part<O> {

    using Base=typename Formats::template Part<O>;

    template<Fmt tag> std::enable_if_t<tag==Fmt::None> fmtStart(const Ctx& ctx) {Base::noneStart(ctx);/*Base::template fmtStart<tag>(ctx);*/}
    template<Fmt tag> std::enable_if_t<tag==Fmt::View> fmtStart(const Ctx& ctx) {Base::viewStart(ctx);/*Base::template fmtStart<tag>(ctx);*/}
    template<Fmt tag> std::enable_if_t<tag==Fmt::Title> fmtStart(const Ctx& ctx) {Base::titleStart(ctx);/*Base::template fmtStart<tag>(ctx);*/}
    template<Fmt tag> std::enable_if_t<tag==Fmt::Menu> fmtStart(const Ctx& ctx) {Base::menuStart(ctx);/*Base::template fmtStart<tag>(ctx);*/}
    template<Fmt tag> std::enable_if_t<tag==Fmt::Body> fmtStart(const Ctx& ctx) {Base::bodyStart(ctx);/*Base::template fmtStart<tag>(ctx);*/}
    template<Fmt tag> std::enable_if_t<tag==Fmt::Item> fmtStart(const Ctx& ctx) {Base::itemStart(ctx);/*Base::template fmtStart<tag>(ctx);*/}
    template<Fmt tag> std::enable_if_t<tag==Fmt::Index> fmtStart(const Ctx& ctx) {Base::indexStart(ctx);/*Base::template fmtStart<tag>(ctx);*/}
    template<Fmt tag> std::enable_if_t<tag==Fmt::Accel> fmtStart(const Ctx& ctx) {Base::accelStart(ctx);/*Base::template fmtStart<tag>(ctx);*/}
    template<Fmt tag> std::enable_if_t<tag==Fmt::NavCursor> fmtStart(const Ctx& ctx) {Base::navCursorStart(ctx);/*Base::template fmtStart<tag>(ctx);*/}
    template<Fmt tag> std::enable_if_t<tag==Fmt::Field> fmtStart(const Ctx& ctx) {Base::fieldStart(ctx);/*Base::template fmtStart<tag>(ctx);*/}
    template<Fmt tag> std::enable_if_t<tag==Fmt::Label> fmtStart(const Ctx& ctx) {Base::labelStart(ctx);/*Base::template fmtStart<tag>(ctx);*/}
    template<Fmt tag> std::enable_if_t<tag==Fmt::EditMode> fmtStart(const Ctx& ctx) {Base::editModeStart(ctx);/*Base::template fmtStart<tag>(ctx);*/}
    template<Fmt tag> std::enable_if_t<tag==Fmt::EditCursor> fmtStart(const Ctx& ctx) {Base::editCursorStart(ctx);/*Base::template fmtStart<tag>(ctx);*/}
    template<Fmt tag> std::enable_if_t<tag==Fmt::Data> fmtStart(const Ctx& ctx) {Base::dataStart(ctx);/*Base::template fmtStart<tag>(ctx);*/}
    template<Fmt tag> std::enable_if_t<tag==Fmt::Unit> fmtStart(const Ctx& ctx) {Base::unitStart(ctx);/*Base::template fmtStart<tag>(ctx);*/}

    template<Fmt tag> std::enable_if_t<tag==Fmt::None> fmtStop(const Ctx& ctx) {Base::noneStop(ctx);/*Base::template fmtStop<tag>(ctx);*/}
    template<Fmt tag> std::enable_if_t<tag==Fmt::View> fmtStop(const Ctx& ctx) {Base::viewStop(ctx);/*Base::template fmtStop<tag>(ctx);*/}
    template<Fmt tag> std::enable_if_t<tag==Fmt::Title> fmtStop(const Ctx& ctx) {Base::titleStop(ctx);/*Base::template fmtStop<tag>(ctx);*/}
    template<Fmt tag> std::enable_if_t<tag==Fmt::Menu> fmtStop(const Ctx& ctx) {Base::menuStop(ctx);/*Base::template fmtStop<tag>(ctx);*/}
    template<Fmt tag> std::enable_if_t<tag==Fmt::Body> fmtStop(const Ctx& ctx) {Base::bodyStop(ctx);/*Base::template fmtStop<tag>(ctx);*/}
    template<Fmt tag> std::enable_if_t<tag==Fmt::Item> fmtStop(const Ctx& ctx) {Base::itemStop(ctx);/*Base::template fmtStop<tag>(ctx);*/}
    template<Fmt tag> std::enable_if_t<tag==Fmt::Index> fmtStop(const Ctx& ctx) {Base::indexStop(ctx);/*Base::template fmtStop<tag>(ctx);*/}
    template<Fmt tag> std::enable_if_t<tag==Fmt::Accel> fmtStop(const Ctx& ctx) {Base::accelStop(ctx);/*Base::template fmtStop<tag>(ctx);*/}
    template<Fmt tag> std::enable_if_t<tag==Fmt::NavCursor> fmtStop(const Ctx& ctx) {Base::navCursorStop(ctx);/*Base::template fmtStop<tag>(ctx);*/}
    template<Fmt tag> std::enable_if_t<tag==Fmt::Field> fmtStop(const Ctx& ctx) {Base::fieldStop(ctx);/*Base::template fmtStop<tag>(ctx);*/}
    template<Fmt tag> std::enable_if_t<tag==Fmt::Label> fmtStop(const Ctx& ctx) {Base::labelStop(ctx);/*Base::template fmtStop<tag>(ctx);*/}
    template<Fmt tag> std::enable_if_t<tag==Fmt::EditMode> fmtStop(const Ctx& ctx) {Base::editModeStop(ctx);/*Base::template fmtStop<tag>(ctx);*/}
    template<Fmt tag> std::enable_if_t<tag==Fmt::EditCursor> fmtStop(const Ctx& ctx) {Base::editCursorStop(ctx);/*Base::template fmtStop<tag>(ctx);*/}
    template<Fmt tag> std::enable_if_t<tag==Fmt::Data> fmtStop(const Ctx& ctx) {Base::dataStop(ctx);/*Base::template fmtStop<tag>(ctx);*/}
    template<Fmt tag> std::enable_if_t<tag==Fmt::Unit> fmtStop(const Ctx& ctx) {Base::unitStop(ctx);/*Base::template fmtStop<tag>(ctx);*/}

  };
};

struct FmtFuncsAPI {
  template<typename O>
  struct Part:O {
    static constexpr void noneStart(const Ctx& ctx) {}
    static constexpr void viewStart(const Ctx& ctx) {}
    static constexpr void titleStart(const Ctx& ctx) {}
    static constexpr void menuStart(const Ctx& ctx) {}
    static constexpr void bodyStart(const Ctx& ctx) {}
    static constexpr void itemStart(const Ctx& ctx) {}
    static constexpr void indexStart(const Ctx& ctx) {}
    static constexpr void accelStart(const Ctx& ctx) {}
    static constexpr void navCursorStart(const Ctx& ctx) {}
    static constexpr void fieldStart(const Ctx& ctx) {}
    static constexpr void labelStart(const Ctx& ctx) {}
    static constexpr void editModeStart(const Ctx& ctx) {}
    static constexpr void editCursorStart(const Ctx& ctx) {}
    static constexpr void dataStart(const Ctx& ctx) {}
    static constexpr void unitStart(const Ctx& ctx) {}

    static constexpr void noneStop(const Ctx& ctx) {}
    static constexpr void viewStop(const Ctx& ctx) {}
    static constexpr void titleStop(const Ctx& ctx) {}
    static constexpr void menuStop(const Ctx& ctx) {}
    static constexpr void bodyStop(const Ctx& ctx) {}
    static constexpr void itemStop(const Ctx& ctx) {}
    static constexpr void indexStop(const Ctx& ctx) {}
    static constexpr void accelStop(const Ctx& ctx) {}
    static constexpr void navCursorStop(const Ctx& ctx) {}
    static constexpr void fieldStop(const Ctx& ctx) {}
    static constexpr void labelStop(const Ctx& ctx) {}
    static constexpr void editModeStop(const Ctx& ctx) {}
    static constexpr void editCursorStop(const Ctx& ctx) {}
    static constexpr void dataStop(const Ctx& ctx) {}
    static constexpr void unitStop(const Ctx& ctx) {}
  };
};