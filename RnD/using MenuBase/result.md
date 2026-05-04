# comparing menu definition methods

## STATIC

**type-level definition**

```c++
  using MyMenu=MenuDef<
    ItemDef<StaticText<text::title>>,
    StaticBody<
      ItemDef<StaticText<text::op1>>,
      ItemDef<StaticText<text::op2>>,
      ItemDef<StaticText<text::op3>>
    >,
    WrapNav,
    PadDraw
  >;
  
  MyMenu myMenu;
```

```text
RAM:   [=         ]  12.6% (used 258 bytes from 2048 bytes)
Flash: [=         ]   9.3% (used 2996 bytes from 32256 bytes)
```

## TEXT

```c++
  using MyMenu=MenuDef<
    ItemDef<Text>,
    StaticBody<
      ItemDef<Text>,
      ItemDef<Text>,
      ItemDef<Text>
    >,
    WrapNav,
    PadDraw
  >;
  
  MyMenu myMenu(text::title,{text::op1,text::op2,text::op3});
```

```text
RAM:   [=         ]  13.0% (used 267 bytes from 2048 bytes)
Flash: [=         ]   9.0% (used 2894 bytes from 32256 bytes)
```

## ALT

```c++
  //alternative construction
  auto myMenu=menuDef(
    ItemDef<StaticText<text::title>>{},
    staticBody(
      ItemDef<Text>{text::op1},
      ItemDef<Text>{text::op2},
      ItemDef<Text>{text::op3}
    )
  );
```

```text
RAM:   [=         ]  13.0% (used 266 bytes from 2048 bytes)
Flash: [=         ]   9.3% (used 3014 bytes from 32256 bytes)
```
