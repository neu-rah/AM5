struct ItemDef<
  Menu<
    ItemDef<Chain<DataPrint, Data<const char*> > >, 
    StaticBody<
      ItemDef<Action<action::op1>, Chain<DataPrint, DataRef<const char* const, text::op1> >, OnFocus<Put<Chain<DataPrint, DataRef<const char* const, desc::op1> > >::ToOut<...> > >, 
      ItemDef<Action<action::op2>, Chain<DataPrint, DataRef<const char* const, text::op2> >, OnFocus<Put<Chain<DataPrint, DataRef<const char* const, desc::op2> > >::ToOut<...> > >, 
      ItemDef<Id<0>, Action<action::op3>, Watch<EnDis<false> >, Chain<DataPrint, DataRef<const char* const, text::op3> >, OnFocus<Put<Chain<DataPrint, DataRef<const char* const, desc::op3> > >::ToOut<...> > >, 
      ItemDef<
        Menu<
          ItemDef<Chain<DataPrint, DataRef<const char* const, text::fields_menu> >, OnFocus<Put<Chain<DataPrint, DataRef<const char* const, desc::fields_menu> > >::ToOut<...> > >, 
          StaticBody<
            ItemDef<AsFmt<Fmt::Label, Chain<DataPrint, Data<const char*> > >, AsFmt<Fmt::EditMode>, EditField, ParentDraw, AsFmt<Fmt::Field, TextField<15, CharMask::Range<unsigned char, 32, 126> > > >, 
            ItemDef<Chain<Id<1>, AsFmt<Fmt::Label, Chain<DataPrint, DataRef<const char* const, text::power> > > >, AsFmt<Fmt::EditMode>, EditField, ParentDraw, NumField<StaticNumRange<int, 0, 100, false>, ItemNav, Watch<AsFmt<Fmt::Field, Default<int, 55>, Chain<DataPrint, Data<int> > > > >, AsFmt<Fmt::Unit, Chain<DataPrint, DataRef<const char* const, text::percent> > > >, 
            ItemDef<
              ToggleBehave, 
              Menu<
                ItemDef<Chain<DataPrint, Data<const char*> >, AsFmt<Fmt::EditMode> >, 
                StaticBody<
                  ItemDef<AsFmt<Fmt::Field, Chain<DataPrint, DataRef<const char* const, text::no> > > >, 
                  ItemDef<AsFmt<Fmt::Field, Chain<DataPrint, DataRef<const char* const, text::yes> > > >, 
                  ItemDef<AsFmt<Fmt::Field, Chain<DataPrint, Data<const char*> > > > 
                >, ParentDraw, WrapNav
              > 
            >, 
            ItemDef<
              RecallNavPos, 
              Menu<
                ItemDef<AsFmt<Fmt::Label, Chain<DataPrint, DataRef<const char* const, text::select_demo> > >, AsFmt<Fmt::EditMode>, BodyAction<action::subIdx> >, 
                StaticBody<
                  ItemDef<AsFmt<Fmt::Field, Chain<DataPrint, DataRef<const char* const, text::sub1> > >, AsFmt<Fmt::Unit, Chain<DataPrint, DataRef<const char* const, text::percent> > > >, 
                  ItemDef<AsFmt<Fmt::Field, Chain<DataPrint, DataRef<const char* const, text::sub2> > >, AsFmt<Fmt::Unit, Chain<DataPrint, DataRef<const char* const, text::percent> > > >, 
                  ItemDef<AsFmt<Fmt::Field, Chain<DataPrint, DataRef<const char* const, text::sub3> > >, AsFmt<Fmt::Unit, Chain<DataPrint, DataRef<const char* const, text::percent> > > >, 
                  ItemDef<AsFmt<Fmt::Field, Chain<DataPrint, DataRef<const char* const, text::sub4> > >, AsFmt<Fmt::Unit, Chain<DataPrint, DataRef<const char* const, text::percent> > > >, 
                  ItemDef<AsFmt<Fmt::Field, Chain<DataPrint, DataRef<const char* const, text::sub5> > >, AsFmt<Fmt::Unit, Chain<DataPrint, DataRef<const char* const, text::percent> > > > 
                >, EditField, ParentDraw, WrapNav, BodyAction<action::subIdx> 
              > 
            >, 
            ItemDef<
              RecallNavPos, 
              Menu<
                ItemDef<Chain<DataPrint, DataRef<const char* const, text::choose_demo> >, AsFmt<Fmt::EditMode>, BodyAction<action::subIdx> >, 
                StaticBody<
                  ItemDef<AsFmt<Fmt::Field, Chain<DataPrint, DataRef<const char* const, text::sub1> >, AsFmt<Fmt::Unit, Chain<DataPrint, DataRef<const char* const, text::percent> > > > >, 
                  ItemDef<AsFmt<Fmt::Field, Chain<DataPrint, DataRef<const char* const, text::sub2> >, AsFmt<Fmt::Unit, Chain<DataPrint, DataRef<const char* const, text::percent> > > > >, 
                  ItemDef<AsFmt<Fmt::Field, Chain<DataPrint, DataRef<const char* const, text::sub3> >, AsFmt<Fmt::Unit, Chain<DataPrint, DataRef<const char* const, text::percent> > > > >, 
                  ItemDef<AsFmt<Fmt::Field, Chain<DataPrint, DataRef<const char* const, text::sub4> >, AsFmt<Fmt::Unit, Chain<DataPrint, DataRef<const char* const, text::percent> > > > >, 
                  ItemDef<AsFmt<Fmt::Field, Chain<DataPrint, DataRef<const char* const, text::sub5> >, AsFmt<Fmt::Unit, Chain<DataPrint, DataRef<const char* const, text::percent> > > > > 
                >, WrapNav, BodyAction<action::subIdx> 
              > 
            >, 
            ItemDef<
              Menu<
                ItemDef<AsFmt<Fmt::Label, Chain<DataPrint, Data<const char*> > >, AsFmt<Fmt::EditMode> >, 
                StaticBody<
                  ItemDef<EditField, ParentDraw, ItemNav, NumField<StaticNumRange<int, 1900, 2150, true>, Watch<AsFmt<Fmt::Field, Default<int, 2026>, Chain<DataPrint, Data<int> > > > > >, 
                  ItemDef<Chain<DataPrint, DataRef<const char* const, text::dateSep> >, EditField, ParentDraw, AsFmt<Fmt::EditMode>, ItemNav, NumField<StaticNumRange<int, 1, 12, true>, Watch<AsFmt<Fmt::Field, Chain<DataPrint, Data<int> > > > > >, 
                  ItemDef<Chain<DataPrint, DataRef<const char* const, text::dateSep> >, EditField, ParentDraw, AsFmt<Fmt::EditMode>, ItemNav, NumField<StaticNumRange<int, 1, 31, true>, Watch<AsFmt<Fmt::Field, Chain<DataPrint, Data<int> > > > > > 
                >, PadDraw, PadDraw
              > 
            >, 
            ItemDef<
              Chain<DataPrint, DataRef<const char* const, text::back> >, 
              OnFocus<Put<Chain<DataPrint, DataRef<const char* const, desc::back> > >::ToOut<...> > 
            > 
          >, 
          WrapNav
        > 
      >, 
      ItemDef<
        Menu<
          ItemDef<BodyAction<action::subIdx>, Chain<DataPrint, DataRef<const char* const, text::array_sub_menu> >, OnFocus<Put<Chain<DataPrint, DataRef<const char* const, desc::array_sub_menu> > >::ToOut<...> > >, 
          CArrayBody<ItemDef<Chain<DataPrint, Data<const char*> > >, (& cBody), 6> 
        > 
      >, 
      ItemDef<
        Menu<
          ItemDef<BodyAction<action::subIdx>, Chain<DataPrint, DataRef<const char* const, text::sub_ibody> >, OnFocus<Put<Chain<DataPrint, DataRef<const char* const, desc::sub_ibody> > >::ToOut<...> > >, 
          CPtrArrayBody<IItem, (& iBody), 6>, 
          WrapNav
        > 
      >, 
      ItemDef<
        Menu<
          ItemDef<BodyAction<action::subIdx>, Chain<DataPrint, DataRef<const char* const, text::sub_sbody> >, OnFocus<Put<Chain<DataPrint, DataRef<const char* const, desc::sub_sbody> > >::ToOut<...> > >, 
          StdBody<std::vector<IItem*, std::allocator<IItem*> > >, 
          WrapNav, Id<2> 
        > 
      >, 
      ItemDef<Action<action::quit>, AsFmt<Fmt::Label, Chain<DataPrint, DataRef<const char* const, text::quit> >, OnFocus<Put<Chain<DataPrint, DataRef<const char* const, desc::quit> > >::ToOut<...> > > > 
    >, WrapNav
  > 
>
