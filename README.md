# c6502

c6502 is a simple MOS 6502 emulator made for educational purposes.

## Requirements

* [Raylib](https://github.com/raysan5/raylib)
* [C64 Truetype font](https://style64.org/c64-truetype)

## Prerequisites

Make sure you have Raylib installed, preferably via a package manager.

### eg. Arch Linux

```shell
$ pacman -S raylib
```

### macOS
```shell
$ brew install raylib
```

Download [C64_Pro_Mono-STYLE.ttf](https://style64.org/c64-truetype) font file and place it in the project root folder.

## Compiling

Makefile script has support for compiling under Linux or macOS. Windows not supported.

```shell
$ make
```

## Usage

```shell
c6502 [FILE] [OPTIONS]
```

### Example: Spawning a gui debugger window

```shell
$ ./c6502 assembly6502_program.bin --gui
```