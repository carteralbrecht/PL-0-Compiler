// Carter Albrecht, ca845545
// COP3402 Montagne, Spring 2020

#ifndef PL0_PARSE_H
#define PL0_PARSE_H

#define CODEGEN_TEMP_FILENAME "code.txt"

void program();
void block();
void statement();
void condition();
void expression();
void term();
void factor();
void getToken();
void stop(int type);
int isNumber(char *str);
void parse(char *filename, int code_flag);

#endif //PL0_PARSE_H
