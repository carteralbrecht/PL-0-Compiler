// Carter Albrecht, ca845545
// COP3402 Montagne, Spring 2020

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/token.h"

// returns a lexeme string for a given (non keyword/ident) token type
char *getLexeme(token_type t)
{
    switch (t)
    {
        case semicolonsym:
            return ";";
        case becomessym:
            return ":=";
        case plussym:
            return "+";
        case minussym:
            return "-";
        case multsym:
            return "*";
        case slashsym:
            return "/";
        case eqsym:
            return "=";
        case lparentsym:
            return "(";
        case rparentsym:
            return ")";
        case commasym:
            return ",";
        case periodsym:
            return ".";
        case geqsym:
            return ">=";
        case gtrsym:
            return ">";
        case neqsym:
            return "<>";
        case leqsym:
            return "<=";
        case lessym:
            return "<";
        default:
            return NULL;
    }
}

char *gettokenString(token_type type)
{
    switch (type)
    {
        case nulsym:
            return "nulsym";
        case leqsym:
            return "leqsym";
        case identsym:
            return "identsym";
        case numbersym:
            return "numbersym";
        case plussym:
            return "plussym";
        case minussym:
            return "minussym";
        case multsym:
            return "multsym";
        case slashsym:
            return "slashsym";
        case oddsym:
            return "oddsym";
        case eqsym:
            return "eqsym";
        case neqsym:
            return "neqsym";
        case lessym:
            return "lessym";
        case gtrsym:
            return "gtrsym";
        case geqsym:
            return "geqsym";
        case lparentsym:
            return "lparentsym";
        case rparentsym:
            return "rparentsym";
        case commasym:
            return "commasym";
        case semicolonsym:
            return "semicolonsym";
        case periodsym:
            return "periodsym";
        case becomessym:
            return "becomessym";
        case beginsym:
            return "beginsym";
        case endsym:
            return "endsym";
        case ifsym:
            return "ifsym";
        case thensym:
            return "thensym";
        case whilesym:
            return "whilesym";
        case dosym:
            return "dosym";
        case callsym:
            return "callsym";
        case constsym:
            return "constsym";
        case varsym:
            return "varsym";
        case procsym:
            return "procsym";
        case writesym:
            return "writesym";
        case readsym:
            return "readsym";
        case elsesym:
            return "elsesym";
        default:
            return "";
    }
}

// used to check ident strings against reserved words
// if it is a reserved word, return its token_type, otherwise return identsym
token_type getNonSpecialType(char *lexeme)
{
    if (strcmp(lexeme, "odd") == 0)
        return oddsym;
    else if (strcmp(lexeme, "begin") == 0)
        return beginsym;
    else if (strcmp(lexeme, "end") == 0)
        return endsym;
    else if (strcmp(lexeme, "if") == 0)
        return ifsym;
    else if (strcmp(lexeme, "then") == 0)
        return thensym;
    else if (strcmp(lexeme, "while") == 0)
        return whilesym;
    else if (strcmp(lexeme, "do") == 0)
        return dosym;
    else if (strcmp(lexeme, "call") == 0)
        return callsym;
    else if (strcmp(lexeme, "const") == 0)
        return constsym;
    else if (strcmp(lexeme, "var") == 0)
        return varsym;
    else if (strcmp(lexeme, "procedure") == 0)
        return procsym;
    else if (strcmp(lexeme, "write") == 0)
        return writesym;
    else if (strcmp(lexeme, "read") == 0)
        return readsym;
    else if (strcmp(lexeme, "else") == 0)
        return elsesym;
    else
        return identsym;
}

// creates an empty linked list of tokens
token_list *createList(void)
{
    token_list *t = malloc(sizeof(token_list));
    if (t == NULL)
        return NULL;
    t->head = NULL;
    t->size = 0;
    return t;
}

token *createToken(token_type type, char *lexeme)
{
    token *new = malloc(sizeof(token));
    new->type = type;
    if (lexeme == NULL)
    {
        new->lexeme = NULL;
    }
    else
    {
        new->lexeme = malloc(sizeof(char) * (strlen(lexeme) + 1));
        strcpy(new->lexeme, lexeme);
    }
    return new;
}

node *createNode(token *data)
{
    node *new = malloc(sizeof(node));
    if (new == NULL)
        return NULL;
    new->data = data;
    new->next = NULL;
    return new;
}

// adds a token to the token list
token_list *addNode(token_list *tokens, token_type type, char *lexeme)
{
    if (tokens == NULL)
        return NULL;

    node *newNode = createNode(createToken(type, lexeme));

    node *temp = tokens->head;

    if (temp == NULL)
    {
        tokens->head = newNode;
    } else
    {
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = newNode;
    }

    tokens->size++;
    return tokens;
}

// used when adding a token that is just a single character to the linked list
token_list *addSingleCharToken(token_list *tokens, token_type type)
{
    char *lexeme = getLexeme(type);
    return addNode(tokens, type, lexeme);
}

token *destroyToken(token *tok)
{
    if (tok == NULL)
        return NULL;
    free(tok->lexeme);
    free(tok);
    return NULL;
}

// frees all dynamically allocated memory associated with a linked list
token_list *destroyTokenList(token_list *tokens)
{
    if (tokens == NULL)
        return NULL;

    node *current = tokens->head;
    node *next;

    while (current != NULL)
    {
        next = current->next;
        destroyToken(current->data);
        free(current);
        current = next;
    }
    tokens->head = NULL;
    free(tokens);
    return NULL;
}
