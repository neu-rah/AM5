# AM5 development notes

## BUGS

_just like that..._

- entering a menu with idx out of range block the app!
- enum Toggle & Select are not calling the action when done
- clear info panel on focus change
- XML formatting still sucks
- allow pad exit without `Esc` (either side?)
- Body is reprinting the menu for each pad item

## Design

### decisions

#### new menu structure (menu not deriving from Title)

- brings the problem of not being able to add parts to the menu (composition is closed).
  This is a BIG problem, however this is also what allows us to have **menu generating functions** like `menuDef(...)`

### other design things

- ~~make a bitmask out of Fmt, too many switches on format (and they will still work!)~~ **done**
- ~~move Wraps setting out of ItemNav (rename?)~~,  
  only usefulness is to open on enter, former **CanNav**,  
- use prefix/suffix items/components to implement separators?
  and avoid the @⅜£⅜§ of navigating over them...

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
- text only non-selectable fake items, useful for Pad separators
- real enum fields that retrieve and store data (not nav position)
- pad menu/fields
- text edit fields
- password fields
- body union
- fonts
- `PadFree` output part _-> padWith(freeX())_ **in format now**
- `ClearFree` output part _-> while(freeY()) PadFree(freeX())_ **in format now**
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

