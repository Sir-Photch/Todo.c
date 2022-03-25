# todo-applet

Inspired by rexims todo-rs

`but in C!`

## build
```console
$ make release
```

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
