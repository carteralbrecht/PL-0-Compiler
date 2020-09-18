// Carter Albrecht, ca845545
// COP3402 Montagne, Spring 2020

#ifndef PL0_TOKEN_H
#define PL0_TOKEN_H

// all possible token_types
typedef enum
{
    nulsym = 1,
    identsym = 2,
    numbersym = 3,
    plussym = 4,
    minussym = 5,
    multsym = 6,
    slashsym = 7,
    oddsym = 8,
    eqsym = 9,
    neqsym = 10,
    lessym = 11,
    leqsym = 12,
    gtrsym = 13,
    geqsym = 14,
    lparentsym = 15,
    rparentsym = 16,
    commasym = 17,
    semicolonsym = 18,
    periodsym = 19,
    becomessym = 20,
    beginsym = 21,
    endsym = 22,
    ifsym = 23,
    thensym = 24,
    whilesym = 25,
    dosym = 26,
    callsym = 27,
    constsym = 28,
    varsym = 29,
    procsym = 30,
    writesym = 31,
    readsym = 32,
    elsesym = 33
} token_type;

typedef struct token
{
    token_type type;
    char *lexeme;
} token;

// token node
typedef struct node
{
    token *data;
    struct node *next;
} node;

// holds the head of a linked list of tokens and the size of the list
typedef struct token_list
{
    node *head;
    int size;
} token_list;

char *getLexeme(token_type t);
token_type getNonSpecialType(char *lexeme);
token_list *createList(void);
token_list *addNode(token_list *tokens, token_type type, char *lexeme);
token_list *addSingleCharToken(token_list *tokens, token_type type);
token_list *destroyTokenList(token_list *tokens);
token *destroyToken(token *tok);
token *createToken(token_type type, char *lexeme);
char *gettokenString(token_type type);

#endif //PL0_TOKEN_H
