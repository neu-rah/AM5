# TODO from the 2022 version

## Bugs and fixes
- [x] Cmd::Up should always increase the numeric value (compose commands swap if needed other direction)
- [x] pass printing functions with template parameter `measure` instead of `if(!measure)`  
    => then add a measure class to disable printing right on top of output driver
      cursors are past drivers and they need the info, so pass it along over the driver
- [x] make color tables a `Part` so that we do not need to pass the device
- [ ] Nav should use reference to external created menu instead of the data member, data member versions can be composed after

## Development

## Features
- [x][x] NL part to compose multiline items while preserving vertical space account <= no checking for `\n` inside strings
- [x][ ] Flash text support
- [x][x] Id's
- [x][-] Styles = noStyle | pad | hNav | enumFld | wrap
- [x][x] Body Footer
- [x][x] use C array as body
- [x][x] Pads
- [x][x] Add extra params to `act` call, added index
- [x][-] add Act command so that we can generalize more
- [x][-] make item to call actions by id, allowing action tables and quick input to action translation
- [x][?] allow pre-made commands to mimic nav commands (sending commands to ID), this allows linking of buttons and fields
- [x][x] Multiline options
- [x][ ] RPC tables, however they nust be part of the root to be referenceable
- [x][-] make color tables serve function with Ctx cursor modes
- [x][ ] Exit pad by left nav, if not given a wrap style
- [x][?] Exit enum on cascade exit (but only left-)
- [x][x] use std::vector as body (or other collection?)
- [x][x] TextClip, (as items or as printer parts... or both?)-> **now as item parts**
- [x][-] split formats into composable parts
- [x][ ] roll long texts on focus => clipping ctrl
- [x][ ] add margin to `Align`
- [x][ ] Multilingual
- [x][x] split input into drivers and parsers
- [x][ ] Id to Path (is this useful?). yes, or support runtime id
- [~][x] safeguards to ensure proper mount of parts (ex: EnDis should not be on top of any action)
- [ ][ ] Cmd::Exit at root should suspend the menu if Activity monitor is present
- [ ][ ] open or select on focus (for enums and 2d nav)
- [ ][ ] Full panel options
- [ ][x] Panel footer => panel and measure
- [ ][ ] Part to invert key direction... can it relate to input type?
- [ ][ ] shortcuts and accel
- [ ][x] Check for `\n` inside strings to account for vertical measure? optional?
- [ ][x] UTF8 printing
- [ ][x] Help text
- [ ][ ] Dialogs => h/v nav coordination
- [ ][?] input/nav/output delegation
- [ ][ ] use Accel. change/edit for num. field edit
- [ ][ ] Functional data source, do not access data members, use functions (make data members private)
- [ ][?] Cached data source
- [ ][ ] Filesystems
- [ ][ ] MQTT interface
- [ ][x] allow Up/Down to use k as an alternative delta for numeric change?
- [ ][ ] allow hidden option, now that we print by viewport its just easy (a style), but not so easy for nav and scroll
- [ ][ ] support separators (non-focus items), essential for hidden options
      best way is to use a composed options, that counts to Nav as 1 option but prints the separator
- [ ][x] add fields color
- [-][x] disconnect nav and input, input should command by return value (nah)
- [-][-] Flash data support (for enums?), nah static is pretty good
- [-][-] Allow Id as template parameter on act functions, so that users can customize action by Id at zero cost.
      unless the action holds the id we can not do this, because the function must be parameterized with the id

## Fields
- [x] Select
- [x] Toggle
- [x] Choose
- [x] Translate fields, ie: hold a value but display as text
- [x] Text edit
- [ ] field data format
- [ ] Radio? this is possible by tieing a field with an enum value

## Input
- [ ] a command parser is something that can be overridden by the item
    use this for a num. keypad handler when entering the num.pad dialog
- [ ] RF driver => id nav

## Output
- [x] Text measure
- [x] Text vertical scroll
- [x] Account for multiline title space
- [x] V. scroll
- [x] margins and alignments
- [x] Viewport
- [ ] panels composition and liquid layout
- [ ] borders and margins
- [ ] Full and minimal draw
- [ ] Change check | draw | clean
- [ ] Formats: ansi, json, xml
- [ ] Liquid layout
- [ ] clipping && scroll
- [ ] serial to web browser, see pepper driver for node

## Navigation
- [x] 2D nav
- [x] Id nav for tiny mcu's
- [ ] matrix nav
- [ ] center nav, keep selected item on the same position
- [ ] enter on focus, this goes well for some items (like enums) but not so well on action prompts
- [ ] runtime id walk
- [ ] Inactivity monitor
- [ ] shell parser => shell commands as item parts


### Persistency
- [ ] version manage
- [ ] flash cycle

## Shell
- [ ] shell commands and item id(ish) hooks

  `led 1 30` led->menu 1->idx 30->fallback field values  
  `led 2 off` field hooked to on/off, set value  
  `red on` red->led 1, on -> field hooked to on/off, set value  
  `red` enumerate all data  
  `red -v` enumerate all data recursive  
  `red -v«n»` enumerate all data recursive, up to level «n»   
  `open /2/3` path navigation, internal command  
  `close` internal command  

## Notes

**Evaluate the cost of CRTP beside the horrible error messages.**
measuring and composition and extension would go easier.

specialization might not go well, because it would specialize the whole thing combination, I think.

**measuring**

1) signal with template parameter
  - some priting code might be heavy to be duplicated by specialization
  - all code must be passsing it

2) signal with output system flag
  + no specialization
  - no code need to be passing it

the idea is that the requester would then watch the cursor change and restore it before real print=> cursor and viewport must be present

> multiple measuring can drag the system this way.

**StaticPos** => just hold a coordinate

**Cursor** => present if device can position the cursor but does not report back, and also MUST be present if measuring.

**Panel**
Holds the available output dimension/area

**Viewport** 
inform available space with a panel and cursor or a device that reports cursor position.
Can reposition the content and move coordinates (scroll)

make measuring and align a module of the output!
**can not, once activated the printing has to be suspended**


## translation

**fields can:**
- be stored in hw registers provided that we have a read function
- be read-only hw register
- have a hw set along with a stored record (~~preferably non translated between them~~ MUST be between them in order to not display rounding errors)
- have transformation functions, as many as required (group them?)
	ex: gamma, map
- have format transformations
	ex: map
- have a format mask (like printf)


