// Carter Albrecht, ca845545
// COP3402 Montagne, Spring 2020

#ifndef PL0_LEXI_H
#define PL0_LEXI_H

#include "token.h"

#define IDENTIFIER_MAX_LENGTH 11
#define NUMBER_MAX_LENGTH 5
#define LEXEME_LIST_TEMP_FILENAME "lexeme_list.txt"

void printErrPrefix(FILE *err_out, char *fileName, int line_number, int char_number);
void printLexemeTable(token_list *tokens);
void fprintLexemeList(token_list *tokens, char *filename);
void printSourceProgram(FILE *f);
void start_lexer(char *filename, int lexeme_flag);


#endif //PL0_LEXI_H
