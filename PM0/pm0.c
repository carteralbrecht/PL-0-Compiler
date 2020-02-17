// Carter Albrecht, ca845545
// COP3402 Montagne, Spring 2020
// Assignment #1: PM/0

#include <stdio.h>

#define NUM_REGISTERS 8
#define MAX_STACK_HEIGHT 40
#define MAX_CODE_LENGTH 200
#define MAX_LEXI_LEVELS 3

typedef struct instruction
{
    int op;
    int r;
    int l;
    int m;
} instruction;

instruction IR;
instruction code[MAX_CODE_LENGTH];

int stack[MAX_STACK_HEIGHT] = {0};
int activation_records[MAX_STACK_HEIGHT] = {0}; // Helper for printing stack with AR separation
int R[NUM_REGISTERS] = {0};

int BP = 1, SP = 0, PC = 0, Halt = 0;

int base(int l, int base) // l stand for L in the instruction format
{
    int b1 = base; // find base L levels down b1 = base;

    while (l > 0)
    {
        b1 = stack[b1 + 1];
        l--;
    }

    return b1;
}

// returns op code string corresponding to the integer value of op
char *get_op_str(int op)
{
    switch (op)
    {
        case 1:
            return "LIT";
        case 2:
            return "RTN";
        case 3:
            return "LOD";
        case 4:
            return "STO";
        case 5:
            return "CAL";
        case 6:
            return "INC";
        case 7:
            return "JMP";
        case 8:
            return "JPC";
        case 9:
        case 10:
        case 11:
            return "SIO";
        case 12:
            return "NEG";
        case 13:
            return "ADD";
        case 14:
            return "SUB";
        case 15:
            return "MUL";
        case 16:
            return "DIV";
        case 17:
            return "ODD";
        case 18:
            return "MOD";
        case 19:
            return "EQL";
        case 20:
            return "NEQ";
        case 21:
            return "LSS";
        default:
            return NULL;
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Proper Syntax: ./pm0 <input-file>\n");
        return 1;
    }

    FILE *input = fopen(argv[1], "r");

    if (input == NULL)
    {
        printf("Error Opening File \"%s\"", argv[1]);
        return 1;
    }

    char buffer[256];
    int count = 0;

    // scan in the input file and store each instruction in code array
    while (fgets(buffer, 256, input))
    {
        if (count >= MAX_CODE_LENGTH)
        {
            printf("Error: %s exceeds %d instructions\n", argv[1], MAX_CODE_LENGTH);
            return 1;
        }

        sscanf(buffer, "%d %d %d %d", &code[count].op, &code[count].r, &code[count].l, &code[count].m);
        count++;
    }

    // print out the instructions to execute
    printf("Line\t\t\tOP\tR\tL\tM\t\n");
    for (int i = 0; i < count; i++)
    {
        printf("%d\t\t\t", i);
        printf("%s\t", get_op_str(code[i].op));
        printf("%d\t%d\t%d\t\n", code[i].r, code[i].l, code[i].m);
    }

    // print out initial values of PC, BP, SP, Registers, and Stack
    printf("\nInitial Values\n");
    printf("\tPC: %d\n", PC);
    printf("\tBP: %d\n", BP);
    printf("\tSP: %d\n", SP);

    printf("\tRegisters: ");
    for (int i = 0; i < NUM_REGISTERS; i++)
        printf("%d%s", R[i], (i == NUM_REGISTERS - 1) ? "\n" : " ");

    printf("\tStack: ");
    for (int i = 0; i < MAX_STACK_HEIGHT; i++)
        printf("%d%s", stack[i], (i == MAX_STACK_HEIGHT - 1) ? "\n" : " ");

    // Run the program
    while (!Halt)
    {
        IR = code[PC]; // store current instruction in instruction register

        printf("\n%d %s %d %d %d:\n", PC , get_op_str(IR.op), IR.r, IR.l, IR.m); // print out current instruction

        PC++;


        switch (IR.op)
        {
            case 1: // LIT
                R[IR.r] = IR.m;
                break;

            case 2: // RTN
                activation_records[BP] = 0;
                SP = BP - 1;
                BP = stack[SP + 3];
                PC = stack[SP + 4];
                break;

            case 3: // LOD
                R[IR.r] = stack[base(IR.l, BP) + IR.m];
                break;

            case 4: // STO
                stack[base(IR.l, BP) + IR.m] = R[IR.r];
                break;

            case 5: // CAL
                stack[SP + 1] = 0;
                stack[SP + 2] = base(IR.l, BP);
                stack[SP + 3] = BP;
                stack[SP + 4] = PC;
                BP = SP + 1;
                PC = IR.m;
                activation_records[BP] = 1;
                break;

            case 6: // INC
                SP = SP + IR.m;
                break;

            case 7: // JMP
                PC = IR.m;
                break;

            case 8: // JPC
                if (R[IR.r] == 0)
                    PC = IR.m;
                break;

            case 9: // SIO Write
                printf("%d\n", R[IR.r]);
                break;

            case 10: // SIO Read
                printf("Type the number to store in the register: ");
                scanf("%d", &R[IR.r]);
                break;

            case 11: // SIO Halt
                Halt = 1;
                break;

            case 12: // NEG
                R[IR.r] = -R[IR.r];
                break;

            case 13: // ADD
                R[IR.r] = R[IR.l] + R[IR.m];
                break;

            case 14: // SUB
                R[IR.r] = R[IR.l] - R[IR.m];
                break;

            case 15: // MUL
                R[IR.r] = R[IR.l] * R[IR.m];
                break;

            case 16: // DIV
                R[IR.r] = R[IR.l] / R[IR.m];
                break;

            case 17: // ODD
                R[IR.r] = R[IR.r] % 2;
                break;

            case 18: // MOD
                R[IR.r] = R[IR.l] % R[IR.m];
                break;

            case 19: // EQL
                R[IR.r] = R[IR.l] == R[IR.m];
                break;

            case 20: // NEQ
                R[IR.r] = R[IR.l] != R[IR.m];
                break;

            case 21: // LSS
                R[IR.r] = R[IR.l] < R[IR.m];
                break;

            case 22: // LEQ
                R[IR.r] = R[IR.l] <= R[IR.m];
                break;

            case 23: // GTR
                R[IR.r] = R[IR.l] > R[IR.m];
                break;

            case 24: // GEQ
                R[IR.r] = R[IR.l] >= R[IR.m];
                break;
        }

        // print out the execution of the program after each instruction
        if (BP != 0)
        {
            printf("\tPC: %d\n", PC);
            printf("\tBP: %d\n", BP);
            printf("\tSP: %d\n", SP);

            printf("\tRegisters: ");
            for (int i = 0; i < NUM_REGISTERS; i++)
                printf("%d%s", R[i], (i == NUM_REGISTERS - 1) ? "\n" : " ");

            printf("\tStack: ");
            for (int i = 1; i <= SP; i++)
            {
                if (activation_records[i])
                    printf("| ");
                printf("%d%s", stack[i], (i == SP) ? "" : " ");
            }

            printf("\n");
        }
    }

    fclose(input);
}
