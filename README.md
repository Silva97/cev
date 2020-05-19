![GitHub release (latest by date)](https://img.shields.io/github/v/release/Silva97/cev)
[![Build Status](https://travis-ci.com/Silva97/cev.svg?token=xR3sEwSzqmn3DPAajsWE&branch=master)](https://travis-ci.com/Silva97/cev)

# cev
This project is an expression evaluator developed in C language. This uses the [shunting-yard] algorithm to evaluate the expression respecting the precedence of the operators.

The functions `cev()` and `cev_script()` (inside [cev.c] file) is the main of the evaluator that be used to evaluate an string or a file. The expressions support definition of variables and use int64_t signed integers to numbers. All the following C operators can be used:

|      |      |      |      |
| :--: | :--: | :--: | :--: |
| `+`  | `-`  | `*`  | `/`  |
| `%`  | `!`  | `&&` | `\|\|`|
| `>`  | `<`  | `^`  | `~`  |
| `&`  | `\|` | `>>` | `<<` |
| `==` | `!=` | `>=` | `<=` |
| `=`  | `+=` | `-=` | `/=` |
| `%=` | `*=` | `**` |      |

> The `**` is the same as in the JavaScript, calculating the power of the operators.

Simplest example usage:
```c
int64_t result = cev(NULL, "234 + 0xffffffffff / (0732 ** 3)");
```

To use in your project, import `cev.h` and `tree.h` to your include file. Use the command below to compile a static library:
```
$ make lib
```

The file `libcev.a` will be created. Just include `cev.h`, link this static library and done!

# Command-line tool
This project have an [bc]-like command-line calculator that can run scripts or evaluate expressions
direct from input.

![cev example](https://i.imgur.com/D2UECtx.png)

To install it, just run:
```
$ make
$ sudo make install
```

To see help about usage, run `cev -h` in your terminal.

[shunting-yard]: https://en.wikipedia.org/wiki/Shunting-yard_algorithm
[cev.c]: https://github.com/Silva97/cev/blob/master/src/cev/cev.c
[bc]: http://man7.org/linux/man-pages/man1/bc.1p.html