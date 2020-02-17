# PM/0

PM/0 is a virtual machine that that accepts 24 types of instructions in the format of "OP R L M".

(four integers per instruction)

OP: corresponds to the operation code  
R: refers to a register  
L: indicates the lexicographical level or a register  
M: may indicate a literal value, a program/data or a register

## Compilation 

Use the `make` build automation tool to compile PM/0 

```bash
make pm0
```
In the event of an update to the source code, or if you wish to remove the executable from the directory, run

```bash
make clean
```

## Input Files
An input file will consist of instructions seperated by a new line.

The ISA is as follows:

01   – LIT R, 0, M  
Loads a constant value (literal) M into Register R  

02   – RTN 0, 0, 0  
Returns from a subroutine and restore the caller environment 
 
03   – LOD R, L, M  
Load value into a selected register from the stack location at  offset M from L lexicographical levels down  

04   – STO R, L, M  
Store value from a selected register in the stack location at  offset M from L lexicographical levels down  

05   – CAL 0, L, M  
Call procedure at code index M (generates new Activation Record and pc <- M)  

06   – INC 0, 0, M  
Allocate M locals (increment sp by M). First four are Functional Value,Static Link (SL), Dynamic Link (DL),and Return Address (RA)  

07   – JMP 0, 0, M  
Jump to instruction M  

08   – JPC R, 0, M  
Jump to instruction M if R = 0  

09   – SI0 R, 0, 1  
Write a register to the screen  

10   – SIO R, 0, 2  
Read in input from the user and store it in a register  

11   – SIO 0, 0, 3  
End of program (program stops running)  

12   - NEG R, 0, 0  
Negate register R

13   - ADD R, L, M  
Store the sum of registers L and M in register R  

14   - SUB R, L, M  
Store the result of register L - register M in register R

15   - MUL R, L, M  
Store the result of register L * register M in register R  

16   - DIV R, L, M  
Store the result of register L / register M in register R

17   - ODD R, 0, 0  
Store register R % 2 in register R

18   - MOD R, L, M  
Store register L % register M in register R

19   - EQL R, L, M  
Store result of register L == register M in register R

20   - NEQ R, L, M  
Store result of register L != register M in register R

21   - LSS R, L, M  
Store result of register L < register M in register R

22   - LEQ R, L, M  
Store result of register L <= register M in register R

23   - GTR R, L, M  
Store result of register L > register M in register R

24   - GEQ R, L, M  
Store result of register L >= register M in register R


## Usage

To run PM/0, you must specify an input file which contains the instructions to execute. 

```bash
./pmo <input_file>
```

The VM will first print the instructions it will be executing, the initial state of the VM, and then the state of the VM after each instruction is executed.

## Demo

In this directory you will find two test inputs, to run the first test, run

```bash
./pm0 input1.txt
```

and similarly to run the second test, run

```bash
./pm0 input2.txt
```

If you wish to view what the output of these tests should look like, see `output1.txt` and `output2.txt` in the root directory.

## Author
Written by Carter Albrecht (ca845545) for Montagne's COP3402 class of Spring 2020.