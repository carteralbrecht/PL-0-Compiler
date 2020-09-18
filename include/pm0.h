// Carter Albrecht, ca845545
// COP3402 Montagne, Spring 2020

#ifndef PL0_PM0_H
#define PL0_PM0_H

#define NUM_REGISTERS 8
#define MAX_STACK_HEIGHT 40
#define MAX_CODE_LENGTH 200

#define VM_EXECUTION_TEMP_FILENAME "vm.txt"

typedef struct instruction
{
    int op;
    int r;
    int l;
    int m;
} instruction;

enum op_code
{
    LIT = 1,
    RTN,
    LOD,
    STO,
    CAL,
    INC,
    JMP,
    JPC,
    SIO9,
    SIO10,
    SIO11, // 9 10 11 are all SIO
    NEG,
    ADD,
    SUB,
    MUL,
    DIV,
    ODD,
    MOD,
    EQL,
    NEQ,
    LSS,
    LEQ,
    GTR,
    GEQ
};

char *get_op_str(int op);
void start_vm(char *filename, int vm_flag);

#endif //PL0_PM0_H
