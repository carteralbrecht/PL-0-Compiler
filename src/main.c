// Carter Albrecht, ca845545
// COP3402 Montagne, Spring 2020

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/lexi.h"
#include "../include/err.h"
#include "../include/parse.h"
#include "../include/pm0.h"

#define LEXEME_LIST_FLAG 'l'
#define GENERATED_CODE_FLAG 'a'
#define VM_EXECUTION_FLAG 'v'

int main(int argc, char **argv)
{
    int lexeme_flag = 0, code_flag = 0, vm_flag = 0;

    if (argc < 2) // arg check
    {
        printf("Proper Syntax: ./compile <input-txt-file> <flags>\n");
        printf("======================================\n");
        printf("accepted flags:\n");
        printf("-%-20c %5s\n", LEXEME_LIST_FLAG, "print the lexeme list");
        printf("-%-20c %5s\n", GENERATED_CODE_FLAG, "print the generated assembly code");
        printf("-%-20c %5s\n", VM_EXECUTION_FLAG, "print the virtual machine execution");
        return EXIT_FAILURE;
    }
    else if (argc != 2)
    {
        for (int i = 2; i < argc; i++)
        {
            if (argv[i][1] == LEXEME_LIST_FLAG)
                lexeme_flag = 1;
            else if (argv[i][1] == GENERATED_CODE_FLAG)
                code_flag = 1;
            else if (argv[i][1] == VM_EXECUTION_FLAG)
                vm_flag = 1;
        }
    }

    start_lexer(argv[1], lexeme_flag);
    parse(LEXEME_LIST_TEMP_FILENAME, code_flag);
    start_vm(CODEGEN_TEMP_FILENAME, vm_flag);
}

