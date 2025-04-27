# machincod
this repository belongs to [altay.cx](http://altay.cx/)

## Installation
```
$ git clone https://github.com/dimalmfao/machincod
$ cd machincod
$ make
$ make install
```

Compiling :
```
$ machincod -s source.mac -o out.s
```

First step, compile :
```
machincod -s source.mac -o out.s
```
Second, assembling and linking : 
```
as out.s -o out.o -mnaked-reg -msyntax=intel
ld -o out out.o -lmachincod 
```

Hello World:
```c
fn main() : void
{
    print "hello world";
}
```
