// Carter Albrecht, ca845545
// COP3402 Montagne, Spring 2020
// Assignment #2: Lexical Analyzer

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"

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

// adds a token to the token list
token_list *addToken(token_list *tokens, token_type type, char *lexeme)
{
    if (tokens == NULL)
        return NULL;

    token *new = malloc(sizeof(token));
    new->type = type;
    new->lexeme = malloc(sizeof(char) * (strlen(lexeme) + 1));
    strcpy(new->lexeme, lexeme);
    new->next = NULL;

    token *temp = tokens->head;

    if (temp == NULL)
    {
        tokens->head = new;
    } else
    {
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = new;
    }

    tokens->size++;
    return tokens;
}

// used when adding a token that is just a single character to the linked list
token_list *addSingleCharToken(token_list *tokens, token_type type)
{
    char *lexeme = getLexeme(type);
    return addToken(tokens, type, lexeme);
}

// frees all dynamically allocated memory associated with a linked list
token_list *destroyTokenList(token_list *tokens)
{
    if (tokens == NULL)
        return NULL;

    token *current = tokens->head;
    token *next;

    while (current != NULL)
    {
        next = current->next;
        free(current->lexeme);
        free(current);
        current = next;
    }
    tokens->head = NULL;
    free(tokens);
    return NULL;
}
