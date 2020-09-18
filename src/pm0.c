// Carter Albrecht, ca845545
// COP3402 Montagne, Spring 2020

#include <stdio.h>
#include "../include/pm0.h"
#include "../include/err.h"

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
        case LIT:
            return "LIT";
        case RTN:
            return "RTN";
        case LOD:
            return "LOD";
        case STO:
            return "STO";
        case CAL:
            return "CAL";
        case INC:
            return "INC";
        case JMP:
            return "JMP";
        case JPC:
            return "JPC";
        case SIO9:
        case SIO10:
        case SIO11:
            return "SIO";
        case NEG:
            return "NEG";
        case ADD:
            return "ADD";
        case SUB:
            return "SUB";
        case MUL:
            return "MUL";
        case DIV:
            return "DIV";
        case ODD:
            return "ODD";
        case MOD:
            return "MOD";
        case EQL:
            return "EQL";
        case NEQ:
            return "NEQ";
        case LSS:
            return "LSS";
        case LEQ:
            return "LEQ";
        case GTR:
            return "GTR";
        case GEQ:
            return "GEQ";
        default:
            return NULL;
    }
}

void start_vm(char *filename, int vm_flag)
{
    FILE *input = fopen(filename, "r");

    if (!input)
    {
        err_msg_exit(FILE_OPEN_ERR);
    }

    FILE *execution_out = NULL;

    if (vm_flag)
    {
        execution_out = fopen(VM_EXECUTION_TEMP_FILENAME, "w");
        if (!execution_out)
        {
            err_msg_exit(FILE_OPEN_ERR);
        }
    }

    char buffer[256];
    int count = 0;

    // scan in the input file and store each instruction in code array
    while (fgets(buffer, 256, input))
    {
        if (count >= MAX_CODE_LENGTH)
        {
            err_msg_exit(CODE_LENGTH_ERR);
        }

        sscanf(buffer, "%d %d %d %d", &code[count].op, &code[count].r, &code[count].l, &code[count].m);
        count++;
    }

    fclose(input);
    remove(filename);

    if (vm_flag)
    {
        fprintf(execution_out, "============ VM Execution ============\n");
        // print out the instructions to execute
        fprintf(execution_out, "Line\t\t\tOP\tR\tL\tM\t\n");
        for (int i = 0; i < count; i++)
        {
            fprintf(execution_out, "%d\t\t\t", i);
            fprintf(execution_out, "%s\t", get_op_str(code[i].op));
            fprintf(execution_out, "%d\t%d\t%d\t\n", code[i].r, code[i].l, code[i].m);
        }

        // print out initial values of PC, BP, SP, Registers, and Stack
        fprintf(execution_out, "\nInitial Values\n");
        fprintf(execution_out, "\tPC: %d\n", PC);
        fprintf(execution_out, "\tBP: %d\n", BP);
        fprintf(execution_out, "\tSP: %d\n", SP);

        fprintf(execution_out, "\tRegisters: ");
        for (int i = 0; i < NUM_REGISTERS; i++)
            fprintf(execution_out, "%d%s", R[i], (i == NUM_REGISTERS - 1) ? "\n" : " ");

        fprintf(execution_out, "\tStack: ");
        for (int i = 0; i < MAX_STACK_HEIGHT; i++)
            fprintf(execution_out, "%d%s", stack[i], (i == MAX_STACK_HEIGHT - 1) ? "\n" : " ");
    }

    // Run the program
    while (!Halt)
    {
        IR = code[PC]; // store current instruction in instruction register

        if (vm_flag)
        {
            //mprintf("\n%d %s %d %d %d:\n", PC , get_op_str(IR.op), IR.r, IR.l, IR.m);
            fprintf(execution_out, "\n%d %s %d %d %d:\n", PC , get_op_str(IR.op), IR.r, IR.l, IR.m); // print out current instruction
        }

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
        if (BP != 0 && vm_flag)
        {
            fprintf(execution_out, "\tPC: %d\n", PC);
            fprintf(execution_out, "\tBP: %d\n", BP);
            fprintf(execution_out, "\tSP: %d\n", SP);

            fprintf(execution_out, "\tRegisters: ");
            for (int i = 0; i < NUM_REGISTERS; i++)
                fprintf(execution_out, "%d%s", R[i], (i == NUM_REGISTERS - 1) ? "\n" : " ");

            fprintf(execution_out, "\tStack: ");
            for (int i = 1; i <= SP; i++)
            {
                if (activation_records[i])
                    fprintf(execution_out, "| ");
                fprintf(execution_out, "%d%s", stack[i], (i == SP) ? "" : " ");
            }

            fprintf(execution_out, "\n");
        }
    }

    if (vm_flag)
    {
        fprintf(execution_out, "======================================\n");
        fclose(execution_out);
        execution_out = fopen(VM_EXECUTION_TEMP_FILENAME, "r");
        if (!execution_out)
        {
            err_msg_exit(FILE_OPEN_ERR);
        }
        char c;
        while (fscanf(execution_out, "%c", &c) != EOF)
            printf("%c", c);
        printf("\n");
        fclose(execution_out);
        remove(VM_EXECUTION_TEMP_FILENAME);
    }
}
