# RISC-V Assembler

The project contains a program that assembles a **RISC-V code to machine code**. It is a part of the CS204 - Computer Architecture course being taught under Prof. T.V. Kalyan and Prof. Neeraj Goyal at IIT Ropar. The language used in the program is **C++ version 20**

## Authors

- [@shivambhagat79](https://www.github.com/shivambhagat79) Shivam Bhagat (2022CSB1123)
- [@Pratibha0934](https://github.com/Pratibha0934) Pratibha Garg (2022EEB1204)
- [@saharsh18](https://github.com/saharsh18) Saharsh Saxena (2022CSB1116)

## Usage Instructions

- Input is taken from "input.asm" file only.
- Output is obtained in "output.mc" file only.
- The code must be basic RISC-V code.
- The program does not support all pseudocodes but supports the use of labels in .text segment.
- While using labels, it must be followed by a command.
- Full-line comments are allowed, but inline comments are not supported.
- **Immediate** values must only be given in **Decimal format** and no other format like.

## Compiling and running the program

The program can be run by using the following commands.

For Compiling the program, use the following command in the terminal in the folder where the files are stored.

```bash
g++ assembler.cpp
```

To run the program, run the following command.

```bash
<!-- for Linux/Mac -->
./a.out

<!-- For windows -->
./a.exe
```
