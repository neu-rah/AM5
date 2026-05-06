# AM5 development notes

## BUGS

_just like that..._

- ~~entering a menu with idx out of range block the app!~~ -> clamping focus value within body range.
- enum Toggle & Select are not calling the action when done
- clear info panel on focus change
- XML with indent, attributes and cdata sections! **still sucks**
- allow **pad exit** without `Esc` (either side?)
- ~~Body is reprinting the menu for each pad item~~

## Design

### current footer print by output redirection makes big recursive types... mitigate!

```c++
template<typename... OO>
using Desc=
  OnFocus<
  typename Put<OO...>::template ToOut<
    decltype(footer),footer,Clear::yes//<here we add all the output type into this item part type
  >
>;
```

can we use Ids? => join multiple outputs

### ~~pad printing navigation and color~~ 

> **done on ansiFmt... replicate**  
  _This makes evident the need of color tables!_

### ~~remove Gate from Raw? ~~

we can block calls on nav, **however we need the output matched results!**

- need draw to sync and check for changes on visible
- need output only on LockMode::Update or LockMode::None.
- can provide char measure if Cursor present.

### new menu structure 

_**menu not deriving from Title**_

- ~~brings the problem of not being able to add parts to the menu (composition is closed).~~

>solved, even factory functions can, ex: `menuDef<WrapNav>(...)`.

**can you believe this?**
```c++
template<typename... OO,typename T,typename B,typename... PP> 
constexpr MenuDef<T,B,OO...> menuDef(T&& t,B&& b,PP&&... pp)
  {return {std::forward<T>(t),std::forward<B>(b),std::forward<PP>(pp)...};}
```

### other design things

*  ~~only usefulness is to open on enter, former **CanNav**~~  
*  ~~reintroduced ItemNav with the new menu structure, it makes easier to build the menus~~  
- use prefix/suffix items/components to implement separators?
  and avoid the @⅜£⅜§ of navigating over them...
- allow things like `EnDis` to print state as a xml attribute... persistency API?
  we can cheat this one by grabbing `ctx.enabled`... can it be generalized?
- make buffers independent of output, we want to print to many

>`Wraps` setting now on `Menu` and on numeric ranges

## ToDo

_features still not implemented_

- ✓ sizes
- ✓ `Menu`
- ✓ `StaticBody`
- ✓ xml output _(minimal)_
- ✓ inputs
- ✓ navigation
- ✓ en/dis
- ✓ `Id` API
- ✓ scroll
- ✓ Data format, use only 1 character print for cursor
- ✓ `Utf8` - skip utf8 encoding and count only 1 char
- ✓ `Cursor` - calc cursor position for measuring
- ✓ gate/lock - prevent update on measure
- ✓ std containers => virtual API
- ✓ partial update / minimal draw
- ✓ translated values
- ✓ colors _using formats_
- ✓ ~~attributes & styles~~ will use code (less checking!)
- ✓ fields _(numeric and indexed enums)_
- ✓ color space, provide color report for devices that do not support it (kind of cursor)
- ✓ device resume, we need this for the following output panels to work, and color report too
- ✓ `PadFree` output part -> `ClearFreeFmt`
- ✓ `ClearFree` output part -> `ClearFreeFmt`
- pad menu/fields
- text only non-selectable fake items, useful for Pad separators
- real enum fields that retrieve and store data (not nav position)
- text edit fields
- password fields
- body union
- fonts
- dialogs
- multilingual text
- text align Left|Center|Right -> see old versions
- panel align
- animation
- view with panels?
- inactivity timeout
- events
- input burst => PERSIST ON IT IF INPUT AVAILABLE AND NOTHING CHANGED
- easing
- borders/margins
- persistency
- EEPROM recycle management
- file systems
- SDCards
- shells
- RPC's

