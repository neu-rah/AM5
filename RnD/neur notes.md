# AM5 development notes

## BUGS

_just like that..._

- enum Toggle & Select are not calling the action when done
- clear info panel on focus change
- ~~web/xml printing window?~~ works, formatting still sucks
- XML format still messed up
- ~~fix Pad update, check changed/sync system for pads~~
- allow pad exit without `Esc` (either side?)
- ~~device cursor is signaling cursor position with pad close...~~
- disable main selection highlight whe pad is active

## Design

_design decisions_

- ~~make a bitmask out of Fmt, too many switches on format (and they will still work!)~~ **done**
- ~~move Wraps setting out of ItemNav (rename?)~~,  
  only usefulness is to open on enter, former **CanNav**,  
  ~~still have to rename or put the functionality somewhere else~~

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

