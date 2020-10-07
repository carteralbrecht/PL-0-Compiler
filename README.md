# PL/0 Compiler and Virtual Machine

This program reads in PL/0 source code, and analyzes it, parses it, generates assembly code, and executes the machine code on a virtual machine

## Compilation 

Use the `make` build automation tool to compile the program

```bash
make
```

This will produce the executable program `hw4compiler` 

In the event of an update to the source code, or if you wish to remove the executable from the directory, run

```bash
make clean
```

## Input
An input is a `.txt` file containing PL/0 source code.

## Usage/Output

To run the program type

```bash
./compiler <input_file.txt> <flags>
```

The program supports three optional flags (in any order):
* `-a` prints the generated assembly code
* `-l` prints the list of lexemes\tokens
* `-v` prints the execution stages of the vm (Note: This will output after any `write` instructions) 

In the event of an error, the program will halt and print a message containing the type of error 

To see examples of the types of errors you may encounter, see `error-tests\all-error-outputs.txt`

## Demo

In the root directory, there is a PL/0 source code file which can be used to demonstrate the program

To see the demo, from the root directory run

```bash
./hw4compiler directives-demo-input.txt -l -a -v
```

The expected output for this demo is located in `directives-demo-output.txt`  
