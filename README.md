# pe_tool

A command-line tool written in C that parses Windows PE (Portable Executable) files and reports their security features.

**Status:** Under development. Right now it reads a file into memory and prints its size and first two bytes. PE header parsing comes next.

## Build

Requires GCC and GNU Make. Tested on Windows with GCC (WinLibs, UCRT) and mingw32-make:

```sh
mingw32-make
```

This builds `build/pe_tool.exe`. `mingw32-make clean` removes the `build` folder.

## Usage

```sh
./build/pe_tool.exe <file>
```

Example:

```text
$ ./build/pe_tool.exe /c/Windows/System32/notepad.exe
read: 360448 bytes
first bytes: 4d 5a
```

`4d 5a` is "MZ", the signature every PE file starts with. On error, the program prints the reason to stderr and exits with status 1.

## License

Copyright (c) 2026 defensivearchitect. All rights reserved. See [LICENSE](LICENSE).
