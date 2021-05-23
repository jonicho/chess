# chess

A chess engine written in C

## Getting Started

First, clone the repository with
```
$ git clone https://github.com/jonicho/chess
```

### Compile with optimizations for release

For compilation with optimizations for release compile with:
```
make MAKE_RELEASE=1
```
This compiles the whole project every time it is run.
The compiled binary and object files will be in `./build_release`.

To clean all generated build files run
```
make clean MAKE_RELEASE=1
```

### Compile for development

To compile with debug symbols for development compile with:
```
make
```
This only compiles files that changed.
The compiled binary and object files will be in `./build`.

To clean all generated build files run
```
make clean
```
