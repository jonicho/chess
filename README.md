# chess

A chess engine written in C

## Getting Started

First, clone the repository with
```
$ git clone https://github.com/jonicho/chess
```

### Compilation with optimizations for release

For compilation with optimizations for release compile with:
```
$ make
```
This compiles the whole project every time it is run.
The compiled binary and object files will be in `./build/release/`.

To clean all generated build files run
```
$ make clean
```

### Compilation for development

To compile without optimizations and with debug symbols for development compile with:
```
$ make DEBUG=1
```
This only compiles files that changed.
The compiled binary and object files will be in `./build/debug/`.

To clean all generated build files run
```
$ make clean
```

### Usage
To play a game against the computer, simply run:
```
$ ./build/chess play
```
or 
```
$ ./build_release/chess play
```
depending on whether you compiled for release or development.

Enter your moves in pure coordinate notation.

### Testing

To execute tests that test some parts of the implementation run
```
$ ./build/debug/chess test
```
or
```
$ ./build/release/chess test
```
depending on whether you compiled for release or development.
