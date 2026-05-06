# AM5 development notes

## BUGS

_just like that..._

- entering a menu with idx out of range block the app!
- enum Toggle & Select are not calling the action when done
- clear info panel on focus change
- ~~XML formatting still sucks~~ XML with indent, attributes and cdata sections!
- allow pad exit without `Esc` (either side?)
- ~~Body is reprinting the menu for each pad item~~

## Design

### decisions

#### case table for ctx.path on a pad print process up to editing.

```text
ctx.pAt:0 ctx.path:path:{a}       => menu nav
ctx.pAt:1 ctx.path:path:{a,b}     => sub-menu nav
ctx.pAt:2 ctx.path:path:{a,b,c}   => sub-menu nav
ctx.pAt:1 ctx.path:path:{a,b,c}   => parent draw/pad nav
ctx.pAt:1 ctx.path:path:{a,b,c,d} => pad edit

path[pAt]==ctx.pIdx <=> parent is selected

path.len-pAt:
1-0=1 => NavMode::Nav       => root menu nav
2-1=1 => NavMode::Nav       => sub menu nav
3-2=1 => NavMode::Edit/Tune => sub menu nav
3-1=2 => NavMode::Nav       => pad menu nav
4-1=3 => NavMode::Edit/Tune => pad field edit
```

```c++
bool psel() {return path[pAt]==ctx.pIdx;}// <=> parent is selected
```


#### remove Gate from Raw? we can block calls on nav, however we need the output matched results!

#### new menu structure (menu not deriving from Title)

- brings the problem of not being able to add parts to the menu (composition is closed).
  This is a BIG problem, however this is also what allows us to have **menu generating functions** like `menuDef(...)`

### other design things

- ~~make a bitmask out of Fmt, too many switches on format (and they will still work!)~~ **done**
- ~~move Wraps setting out of ItemNav (rename?)~~,  
  ~~only usefulness is to open on enter, former **CanNav**~~
  reintroduced ItemNav with the new menu structure, it makes easier to build the menus  
- use prefix/suffix items/components to implement separators?
  and avoid the @⅜£⅜§ of navigating over them...
- allow things like `EnDis` to print state as a xml attribute... persistency API?
  we can cheat this one by grabbing `ctx.enabled`
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

