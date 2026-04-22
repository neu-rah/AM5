# AM5 Components

## Item components

`Action<f>` associate an `f` function of type `bool(Sz)` to be called on enter.

`Alias<Title,Value>` allow alternative `Title` print for a hidden `Value`

`BodyAction<f>` associate an `f` function of type `bool(Sz)` to be called as default for any body item member.

`EditField` item will accept navigation, enter/exit navigation level by `Cmd::Enter`

`EnDis<i>` provides enable/disable ability to the item, internally stores the state

`Hidden<...>` hides data from printing

`Id<i>` provide a compile-time Id, allows static compile-time reference link

`ItemNav<Wraps>` _deprecated_ (just opens the item on `Cmd::Enter`)

`ItemRef<R,R& ref>` complete redirection to other Item, _not tested yet_

`Menu<T,B>` define a menu with title `T` and body `B`

`NumField` translates nav commands into up/down API calls

`OnFocus<...>` conditional print of <...> when the item is on focus.

`ParentDraw` parent will print even if this item is focused and editing

`struct Put<...>` alternative output for content <...>

`RecallNavPos` stores and restores navigation selection, also prints selected value when not editing.

## Output components

### generic

*`PartialDraw` signals (not provides) this device can update pert of the content

`Gate` suspend printing while measuring or get/set info

*`DeviceCursor` receives a signal/position for the cursor to be positioned after printing

`StaticArea` output rectangle (information only)

`StaticPos` output rectangle position (information only)

*`Raw` provides raw access to the raw device bypassing filters

`UseEditCursorFmt` works with `DeviceCursor` to effectively put the cursor as stored

`DataParser` decomposes all printing data to character level, ready to be feed to other components that require a per character print.

`UTF8` supports UTF8

`TextWrap` wraps long texts with a newline

`Clip` keep content inside defined `Area`

`ColorTrack` tracks used color for `resume()`

`CtrlChars` picks special characters

`Cursor` tracks cursor position for `resume()`

`Buffer<Scroll,char>` adds a buffer to the output, content can now be reprinted

#### from included files:

`StreamOut<Out,Out&>` wire some C++ stream output.

`ConsoleOut` an alias for the standard C++ console output (cout)

`ArduinoSerialOut<Dev,Dev&>` associate an Arduino Serial output device

`SerialOut` an alias for the standard arduino `Serial`

`ANSIOut` an overlay to provide ANSI command codes over an existing output device component.

### printers

this components will emits fmtStart/fmtStop function calls and also answer the menu printing functions and call those function down the composed chain.

Changing the composition order will also change the printing order, omitting them will remove the printing of the omitted part.

`MenuPrinter<...>` a list of printer parts needed to print a menu

`TitlePrinter` calls formats and chain print for the title

`BodyPrinter` prints body item members

`ScrollBodyPrinter` prints body item members, ensure focus is visible inside the output area.

`ItemPrinter<...>` group the parts needed to print an Item, components inside can be omitted or reordered

`ItemBodyPrinter` prints the item body (not the menu body)

`NavCursorPrinter` prints a navigation cursor with the focused item.

`StaticDataPrinter<Data>` prints static data, move to reposition

### formats

`TextFmt` attends to format functions and puts some text format to the output. Cursors, indicators and other format cosmetics

`ANSIFmt` attends to format functions and puts some text/ANSI format codes to the output.

`XmlFmt` writes xml to the output

## Navigation components

`Root<T,T& menu>` holds a reference to an existing menu.

`StaticRoot<M>` holds internal menu data

`TreeNav` controls the navigation over a menu tree.

## Input components

We do not provide any components, however we include some files.

>note that one can issue navigation commands directly to the navigation object.

#### from included files:

`ArduinoSerialIn<Dev,Dev&>` associate am Arduino serial input

`SerialIn` an alias for the standard Arduino `Serial` input

`LinuxKeyIn` when on a linux machine we can read from the keyboard

`PCKbd` key parser for PC keyboard codes, we can use them over a serial device



