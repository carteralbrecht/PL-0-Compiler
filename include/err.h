// Carter Albrecht, ca845545
// COP3402 Montagne, Spring 2020

#ifndef PL0_ERR_H
#define PL0_ERR_H

#define USE_EQ_ERR "Use = instead of :="
#define INVALID_CONST_DEC_ERR "constant must be given value at declaration"
#define NUM_AFTR_EQ_ERR "= must be followed by a number"
#define BCM_AFTR_IDENT_ERR ":= expected after identifier"
#define END_ERR "begin without an end"
#define CALL_KIND_ERR "cannot call a const or var"
#define IDENT_EXPECTED_ERR "Identifier expected"
#define IDENT_AFTR_CONSTVARPROC_ERR "const/var/proc must be followed by identifier"
#define SEMICOLON_COMMA_ERR "Semicolon or comma expected"
#define PERIOD_ERR "Period expected"
#define UNDCLRD_IDENT_ERR "Undeclared identifier"
#define CONST_ASSN_ERR "Assignment to constant is not allowed"
#define THEN_ERR "then expected"
#define DO_ERR "do expected"
#define RELOP_ERR "Relational operator expected"
#define RPAREN_ERR "Right parenthesis missing"
#define FACTR_BGN_ERR "A factor cannot begin with this symbol"
#define FILE_OPEN_ERR "Could not open file"
#define MEM_ALLOC_ERR "Could not allocate memory"

#define CMNT_NOT_CLOSED_ERR "Comment opened and not closed"
#define LONG_NUM_ERR "Number exceeds max length"
#define INVALID_IDNT_ERR "Invalid start to identifier"
#define LONG_IDENT_ERR "Identifier exceeds max length"
#define UNKNWN_SYM_ERR "Unknown symbol"

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_RESET "\x1b[0m"

#define CODE_LENGTH_ERR "Code exceeds max length for VM"

void err_msg_exit(char *msg);


#endif //PL0_ERR_H
