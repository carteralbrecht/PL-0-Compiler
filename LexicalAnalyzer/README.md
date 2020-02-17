# PL/0 Lexical Analyzer

This lexical analyzer reads in PL/0 source code and identifies certain lexical errors, or produces the lexemes and their token-types

## Compilation 

Use the `make` build automation tool to compile the LA

```bash
make
```

This will produce the executable LA program `lexi` 

In the event of an update to the source code, or if you wish to remove the executable from the directory, run

```bash
make clean
```

## Input
An input is a `.txt` file containing PL/0 source code. The input may or may not be valid PL/0.

## Usage/Output

To run the LA, you must specify an input file which contains the PL/0 source to analyze

```bash
./lexi <input_file.txt>
```

The LA will first output the source code it is analyzing.  

Next, the LA will do one of the following:  
* output the lexeme table and the lexeme list  
* produce an 'invalid symbol' error  
* produce an 'identifier exceeds max length' error
* produce a 'number exceeds max length' error  
* produce a 'comment opened and not closed' error
* produce an 'invalid start to identifier' error

(Any error message will also include the line and character where the error exists in the source file)

## Demo

In the `tests` directory, there are several tests demonstrating different input situations  

To execute test0, from the root directory run

```bash
./lexi ./tests/test0.txt
```

The expected outputs for each test are also in `tests` ending in `-output.txt`  
More of the lexical analyzer can be explored by running the other tests in a similar fashion

## Author
Written by Carter Albrecht (ca845545) for Montagne's COP3402 class of Spring 2020.