# AM5 development repo

_focusing on C++ composition_

- compile time composition 
- replacing config check by presence/absence of code
> 1) no vars  
> 2) no config check code  
> 3) only used features are present  

## purpose

Still be able to add complex features for mighty devices while supporting tiny ones.

## example

**new definition syntax**

plain C++ (no macros)

**using types**
```c++
using MainMenu=MenuDef<
  Title<ItemNav,
  StaticBody<//main menu static body
    ItemDef<Action<action::op1>,StaticText<&text::op1>>,
    ItemDef<Action<action::op2>,StaticText<&text::op2>>,
    ItemDef<
      Id<ids::op3>,
      Action<action::op3>,
      Watch<EnDis<false>>,
      StaticText<&text::op3>
    >,
    Quit
  >
>;
```

**using mixed functions and types**
```c++
auto tinyMenu=menuDef(
  ItemDef<Text,ItemNav{"title"},
  staticBody(
    ItemDef<Text>{"op1"},
    ItemDef<Text,Action<action::quit>>{"exit"}
  )
);
```