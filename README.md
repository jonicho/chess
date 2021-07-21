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
$ make MAKE_RELEASE=1
```
This compiles the whole project every time it is run.
The compiled binary and object files will be in `./build_release`.

To clean all generated build files run
```
$ make clean MAKE_RELEASE=1
```

### Compilation for development

To compile with debug symbols for development compile with:
```
$ make
```
This only compiles files that changed.
The compiled binary and object files will be in `./build`.

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
$ ./build/chess test
```
or, to run the tests with optimizations, run

```
$ ./build_release/chess test
```
