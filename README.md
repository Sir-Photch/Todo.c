# Todo.c

Todo-applet inspired by rexims todo-rs

`but in C!`

## build
### Linux:
```console
$ make release
```
### Windows (Experimental??):
Assuming `mingw` is installed.
```
$ make windows
```
All dependencies are included. Notice that for windows, `todo.exe` depends on `pdcurses.dll`.

## usage

```console
$ ./todo <todofile>
```
Opens up `todofile` and reads its contents. If it does not exist, it is created after exiting. If no file is specified, it defaults to `./todofile`.

| key | description |
| --- | --- |
| <kbd>j</kbd> | move downwards |
| <kbd>k</kbd> | move upwards |
| <kbd>Enter</kbd> | toggle done-status |
| <kbd>a</kbd> | add item |
| <kbd>d</kbd> | remove selected item |
| <kbd>i</kbd> | enter insert mode, exit with <kbd>Enter</kbd> |
| <kbd>q</kbd> | quit |

## thankfully depends on these public-domain libs:
- [pdcurses](https://github.com/wmcbrine/PDCurses)
- [DG_dynarr.h](https://github.com/nothings/single_file_libs)
