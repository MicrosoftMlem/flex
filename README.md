# Flex

A C++ program that tallies multiple elements of C++, C and header files, alongside markdown and similar files to generate a rating of your project.

Currently has a limited range of supported file types, and doesn't fully analyse the files on all the intended aspects yet.

## Requirements

A C++23 compiler with <print> support: GCC 14+, Clang 18+ (17 partial)



## Building

### Arch

Build with CMake:

```bash
cmake -S . -B build
cmake --build build
```



## Running

To analyse current working directory:

```bash
./build/flex
```

Or you can specify the directory or file:

```bash
./buid/flex [PATH]
```
