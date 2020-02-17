// Carter Albrecht, ca845545
// COP3402 Montagne, Spring 2020
// Assignment #2: Lexical Analyzer

#ifndef PL0_TOKEN_H
#define PL0_TOKEN_H

// all possible token_types
typedef enum
{
    nulsym = 1,
    identsym,
    numbersym,
    plussym,
    minussym,
    multsym,
    slashsym,
    oddsym,
    eqsym,
    neqsym,
    lessym,
    leqsym,
    gtrsym,
    geqsym,
    lparentsym,
    rparentsym,
    commasym,
    semicolonsym,
    periodsym,
    becomessym,
    beginsym,
    endsym,
    ifsym,
    thensym,
    whilesym,
    dosym,
    callsym,
    constsym,
    varsym,
    procsym,
    writesym,
    readsym,
    elsesym
} token_type;

// token node
typedef struct token
{
    token_type type;
    char *lexeme;
    struct token *next;
} token;

// holds the head of a linked list of tokens and the size of the list
typedef struct token_list
{
    token *head;
    int size;
} token_list;

char *getLexeme(token_type t);
token_type getNonSpecialType(char *lexeme);
token_list *createList(void);
token_list *addToken(token_list *tokens, token_type type, char *lexeme);
token_list *addSingleCharToken(token_list *tokens, token_type type);
token_list *destroyTokenList(token_list *tokens);

#endif //PL0_TOKEN_H
