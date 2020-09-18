// Carter Albrecht, ca845545
// COP3402 Montagne, Spring 2020

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/parse.h"
#include "../include/token.h"
#include "../include/symbol.h"
#include "../include/pm0.h"
#include "../include/err.h"

symbol_table *symTable;
FILE *lexemeList;
token *currentToken;

instruction code[MAX_CODE_LENGTH];
int codePosition = 0; // holds position in code array
int regPosition = 0; // holds position of next available register
int currentLevel = 0;

// write instruction to code array
void emit(int op, int R, int L, int M)
{
    if (codePosition > MAX_CODE_LENGTH)
    {
        err_msg_exit(CODE_LENGTH_ERR);
    }
    else
    {
        code[codePosition].op = op; // opcode
        code[codePosition].r = R; //register
        code[codePosition].l = L; // lexicographical level
        code[codePosition].m = M; // modifier
        codePosition++;
    }
}

// get the next token from the input file
void getToken()
{
    currentToken = destroyToken(currentToken);
    int type;
    char lexeme[12];
    int wasIdentOrNumber = 0;

    if (fscanf(lexemeList, "%d", &type) == EOF)
        return;

    if (type == identsym || type == numbersym)
    {
        fscanf(lexemeList, "%s", lexeme);
        wasIdentOrNumber = 1;
    }

    currentToken = createToken(type, wasIdentOrNumber ? lexeme : NULL);
}

void factor()
{
    if (currentToken->type == identsym)
    {
        symbol *curr = tableLookUp(symTable, currentToken->lexeme);
        if (!curr)
        {
            err_msg_exit(UNDCLRD_IDENT_ERR);
        }
        if (curr->kind == VAR_SYMBOL_KIND)
        {
            emit(LOD, regPosition, currentLevel - curr->level, curr->addr);
        }
        else
        {
            emit(LIT, regPosition, 0, curr->val);
        }
        getToken();
    }
    else if (currentToken->type == numbersym)
    {
        emit(LIT, regPosition, 0, atoi(currentToken->lexeme));
        getToken();
    }
    else if (currentToken->type == lparentsym)
    {
        getToken();
        expression();
        if (currentToken->type != rparentsym)
            err_msg_exit(RPAREN_ERR);
        getToken();
    }
    else
        err_msg_exit(FACTR_BGN_ERR);
}

void term()
{
    factor(); // store factor in current register
    while (currentToken->type == multsym || currentToken->type == slashsym)
    {
        regPosition++; // increment register
        token_type multOrDiv = currentToken->type;
        getToken();
        factor(); // put next factor in the incrementeed register
        emit(multOrDiv == multsym ? MUL : DIV, regPosition - 1, regPosition - 1, regPosition);
        regPosition--;
    }
}

void expression()
{
    // this handles the optional + or - before term
    if (currentToken->type == plussym || currentToken->type == minussym)
    {
        token_type addop = currentToken->type;
        getToken();
        term();
        if (addop == minussym)
        {
            emit(NEG, regPosition, 0, 0);
        }
    }
    else
    {
        term(); // get the value of term, stores it in current register position
    }

    // after the first term, an expression could have 0 or more additional terms being added/subbed
    while (currentToken->type == plussym || currentToken->type == minussym)
    {
        regPosition++; // increment register position
        token_type addop = currentToken->type;
        getToken();
        term(); // get the value of next term, store it in current register position
        emit(addop == plussym ? ADD : SUB, regPosition - 1, regPosition - 1, regPosition);
        regPosition--;
    }
}

void condition()
{
    if (currentToken->type == oddsym)
    {
        getToken();
        expression();
    } else
    {
        expression(); // store result in current reg position

        if (!(currentToken->type == eqsym
              || currentToken->type == neqsym
              || currentToken->type == lessym
              || currentToken->type == gtrsym
              || currentToken->type == leqsym
              || currentToken->type == geqsym))
        {
            err_msg_exit(RELOP_ERR);
        }

        token_type relop = currentToken->type;
        int op = 0;
        getToken();

        regPosition++; // increment reg position

        expression(); // store result in incremented reg position

        if (relop == eqsym)
            op = EQL;
        if (relop == neqsym)
            op = NEQ;
        if (relop == lessym)
            op = LSS;
        if (relop == gtrsym)
            op = GTR;
        if (relop == leqsym)
            op = LEQ;
        if (relop == geqsym)
            op = GEQ;

        emit(op, regPosition - 1, regPosition - 1, regPosition);
        regPosition--;
    }
}

void statement()
{
    if (currentToken->type == identsym)
    {
        symbol *curr = tableLookUp(symTable, currentToken->lexeme);
        if (!curr)
        {
            err_msg_exit(UNDCLRD_IDENT_ERR);
        }

        if (curr->kind != VAR_SYMBOL_KIND)
        {
            err_msg_exit(CONST_ASSN_ERR);
        }

        getToken();
        if (currentToken->type != becomessym)
        {
            err_msg_exit(BCM_AFTR_IDENT_ERR); // error
        }

        getToken();
        expression(); // put the value of the expression into the register
        emit(STO, regPosition, currentLevel - curr->level, curr->addr); // store value from register position on the stack
    }
    else if (currentToken->type == callsym)
    {
        getToken();
        if (currentToken->type != identsym)
        {
            err_msg_exit(IDENT_EXPECTED_ERR);
        }
        symbol *curr = tableLookUp(symTable, currentToken->lexeme);
        if (!curr)
        {
            err_msg_exit(UNDCLRD_IDENT_ERR);
        }
        if (curr->kind != PROC_SYMBOL_KIND)
        {
            err_msg_exit(CALL_KIND_ERR);
        }
        emit(CAL, 0, currentLevel - curr->level, curr->addr);
        getToken();
    }
    else if (currentToken->type == beginsym)
    {
        getToken();
        statement();

        while (currentToken->type == semicolonsym)
        {
            getToken();
            statement();
        }

        if (currentToken->type != endsym)
        {
            printf("%d\n", currentToken->type);
            err_msg_exit(END_ERR);
        }

        getToken();
    }
    else if (currentToken->type == ifsym)
    {
        getToken();
        condition();
        if (currentToken->type != thensym)
        {
            err_msg_exit(THEN_ERR);
        }
        getToken();
        int ctemp1 = codePosition;
        emit(JPC, regPosition, 0, 0);
        statement();
        code[ctemp1].m = codePosition;

        // how to handle this
        if (currentToken->type == elsesym)
        {
            code[ctemp1].m = codePosition + 1;
            getToken();
            int ctemp2 = codePosition;
            emit(JMP, 0, 0, 0);
            statement();
            code[ctemp2].m = codePosition;
        }
    }
    else if (currentToken->type == whilesym)
    {
        int ctemp1 = codePosition;
        getToken();
        condition();
        int ctemp2 = codePosition;
        emit(JPC, regPosition, 0, 0);
        if (currentToken->type != dosym)
        {
            err_msg_exit(DO_ERR);
        }
        getToken();
        statement();
        emit(JMP, 0, 0, ctemp1);
        code[ctemp2].m = codePosition;
    }
    else if (currentToken->type == readsym) // read from user and put it in the variable
    {
        getToken();
        if (currentToken->type != identsym)
        {
            err_msg_exit(IDENT_EXPECTED_ERR);
        }

        symbol *curr = tableLookUp(symTable, currentToken->lexeme);

        if (!curr)
        {
            err_msg_exit(UNDCLRD_IDENT_ERR);
        }

        if (curr->kind == CONST_SYMBOL_KIND) // we cant change a constant
        {
            err_msg_exit(CONST_ASSN_ERR);
        }

        emit(SIO10, regPosition, 0, 2);
        emit(STO, regPosition, currentLevel - curr->level, curr->addr);

        getToken();

    }
    else if (currentToken->type == writesym) // print the identifier
    {
        getToken();
        expression();

//        getToken(); // get the identifier token
//
//        // check if token is really an ident
//        if (currentToken->type != identsym)
//        {
//            err_msg_exit(IDENT_EXPECTED_ERR);
//        }
//
//        // find it in the symbol table
//        symbol *curr = tableLookUp(symTable, currentToken->lexeme);
//
//        // if it is not in symbol table throw an error
//        if (!curr)
//        {
//            err_msg_exit(UNDCLRD_IDENT_ERR);
//        }
//
//        // if it is a variable grab it from the stack and put it in register
//        if (curr->kind == VAR_SYMBOL_KIND)
//        {
//            emit(LOD, regPosition, 0, curr->addr);
//        }
//
//        // if it is a constant, its value is in the symbol table and put it in register
//        else
//        {
//            emit(LIT, regPosition, 0, curr->val);
//        }

        // now write that register to the screen
        emit(SIO9, regPosition, 0, 1);

        //getToken();
    }
}

void program()
{
    getToken();
    block();
    if (currentToken == NULL || currentToken->type != periodsym)
    {
        err_msg_exit(PERIOD_ERR);
    }
    emit(SIO11, 0, 0, 3);
}

void block()
{
    currentLevel++;
    int prevTp = symTable->tp;
    int arPosition = 4;
    int jmpaddr = codePosition;
    emit(JMP, 0, 0, 0);
    if (currentToken->type == constsym) // const
    {
        do
        {
            char constantSymbolName[256]; // buffer for name
            int constantSymbolValue; // to hold value

            getToken();
            if (currentToken->type != identsym) // identifier
                err_msg_exit(IDENT_AFTR_CONSTVARPROC_ERR); // error

            strcpy(constantSymbolName, currentToken->lexeme); // identsym skips ahead and holds the identifier in the lexeme

            getToken();

            if (currentToken->type == becomessym)
                err_msg_exit(USE_EQ_ERR);

            if (currentToken->type != eqsym) // equals
                err_msg_exit(INVALID_CONST_DEC_ERR); // error

            getToken();
            if (currentToken->type != numbersym)
            {
                err_msg_exit(NUM_AFTR_EQ_ERR); // error
            }

            constantSymbolValue = atoi(currentToken->lexeme); // numbersym skips ahead and holds the number as a string in lexeme

//            if (tableLookUp(symTable, constantSymbolName) != NULL)
//                err_msg_exit(IDENT_USDAGN_ERR);

            tableInsert(symTable, createConstSymbol(constantSymbolName, constantSymbolValue));

            getToken();
        } while (currentToken->type == commasym);

        if (currentToken->type != semicolonsym)
            err_msg_exit(SEMICOLON_COMMA_ERR);

        getToken();
    }

    if (currentToken->type == varsym)
    {
        do
        {
            getToken();
            if (currentToken->type != identsym)
                err_msg_exit(IDENT_AFTR_CONSTVARPROC_ERR);

//            if (tableLookUp(symTable, currentToken->lexeme) != NULL)
//                err_msg_exit(IDENT_USDAGN_ERR);

            tableInsert(symTable, createVarSymbol(currentToken->lexeme, currentLevel, arPosition++));

            getToken();
        } while (currentToken->type == commasym);

        if (currentToken->type != semicolonsym)
            err_msg_exit(SEMICOLON_COMMA_ERR);

        getToken();
    }

    while(currentToken->type == procsym)
    {
        getToken();
        if (currentToken->type != identsym)
        {
            err_msg_exit(IDENT_AFTR_CONSTVARPROC_ERR);
        }

//        if (tableLookUp(symTable, currentToken->lexeme) != NULL)
//            err_msg_exit(IDENT_USDAGN_ERR);

        tableInsert(symTable, createProcSymbol(currentToken->lexeme, currentLevel, codePosition));
        getToken();
        if (currentToken->type != semicolonsym)
        {
            err_msg_exit(SEMICOLON_COMMA_ERR);
        }
        getToken();
        block();
        if (currentToken->type != semicolonsym)
        {
            err_msg_exit(SEMICOLON_COMMA_ERR);
        }
        getToken();
    }

    code[jmpaddr].m = codePosition;
    emit(INC, 0, 0, arPosition);
    statement();
    if (currentLevel != 1)
        emit(RTN, 0, 0, 0);
    symTable->tp = prevTp;
    currentLevel--;

}

// "main" function to start the parser, takes an input file name and a flag for printing the generated code
void parse(char *filename, int code_flag)
{
    FILE *err_out = stdout; // possibly add optional err redirection in future
    lexemeList = fopen(filename, "r");

    if (!lexemeList)
        err_msg_exit(FILE_OPEN_ERR);

    symTable = createSymbolTable();
    if (!symTable)
    {
        err_msg_exit(MEM_ALLOC_ERR);
    }

    currentToken = NULL;

    program();

    remove(filename);

    printf("Program is syntactically correct\n");

    FILE *code_out = fopen(CODEGEN_TEMP_FILENAME, "w");

    if (!code_out)
    {
        err_msg_exit(FILE_OPEN_ERR);
    }

    for (int i = 0; i < codePosition; i++)
    {
        fprintf(code_out, "%d %d %d %d%s", code[i].op, code[i].r, code[i].l, code[i].m, (i == codePosition - 1) ? "" : "\n");
    }

    if (code_flag)
    {
        printf("\n===== Generated Code =====\n");
        fclose(code_out);
        fopen(CODEGEN_TEMP_FILENAME, "r");
        char c;
        while (fscanf(code_out, "%c", &c) != EOF)
            printf("%c", c);
        printf("\n");
        printf("==========================\n\n");

    }

    fclose(code_out);
}