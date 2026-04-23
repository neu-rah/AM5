# AM5

## APIs

### Output API

#### Get remaining free space

```c++
Sz freeX()
Sz freeY()
Area free()
```

#### Get cursor position

```c++
Sz posX()
Sz posY()
Pos pos()
```

#### Set cursor position

```c++
void setPos(Sz,Sz)
void setPos(Pos p)
```

#### Get origin position

```c++
Sz orgX()
Sz orgY()
Pos org()
```

#### Output lock mode

```c++
LockMode mode()
void mode(LockMode)
```

#### Set colors

```c++
template<typename Cor> void setColors(Cor,Cor)
```

#### Clear output device

```c++
void clear()
```

#### Insert a newline

```c++
void nl()
```

#### Send last data to output device, empty buffers if any

```c++
void flush()
```

#### Restore device state

Usually cursor position and colors, depending on the composition...

```c++
void resume()
```

### Data API

A precursor to Item API for internal consume, using data without the full API weight. 

#### Check state changed or synchronize it.

```c++
bool changed() {return false;}
void sync() {}
```

### Item API

#### Get menu tree depth at compile time

```c++
Depth depth()
```

#### Get/Set enabled state

```c++
bool enabled()
void enable(bool=true)
bool changed()
```

#### Check changed state or synchronize

```c++
static constexpr bool changed()
static constexpr void sync()
```

#### Internal up/down function, some Data items might implement this.

```c++
bool up() {return false;}
bool down() {return false;}
```

#### Printing the menu at the current state

```c++
template<typename Out> bool printMenu(Out&,Ctx&)
template<typename Out> bool printBody(Out&,Ctx&)
template<typename Out> bool printItem(Out&,Ctx&)
template<typename Out> void print(Out&,Ctx&)
```

#### Get the compile time ID, this support a compile time reference to an item by its ID

```c++
static constexpr int getId()
```

#### Get item by id from a menu tree structure

The reference assignment is compile time, runtime will see it as just a reference to an item.

```c++
using WithId<int id>
```

#### Process navigation commands

```c++
template<typename Nav> bool nav(Nav& n,const CKE& cke,Path)
```
  
### Input API

#### Check if input is available

```c++
bool available() {return false;}
```

#### Parse keys

when available, this is an internal commodity call

```c++
template<typename Nav> bool parseKey(Nav& nav,Key k)
```

#### Process input checking for navigation commands

Called by the navigation process

```c++
template<typename Nav> bool cmd(Nav& nav) {return parseKey(nav,0);}
```

### Nav API

_**still under design...**_