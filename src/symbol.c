// Carter Albrecht, ca845545
// COP3402 Montagne, Spring 2020

#include "../include/symbol.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/err.h"

symbol_table *createSymbolTable()
{
    symbol_table *table = malloc(sizeof(symbol_table));
    if (!table)
    {
        exit(1); // TODO: Report error
    }
    table->symbols = calloc(MAX_SYMBOL_TABLE_SIZE, sizeof(symbol));
    if (!table->symbols)
    {
        free(table);
        exit(1); // TODO REPORT ERROR;
    }
    table->tp = 1;
    return table;
}

symbol *createProcSymbol(char *name, int level, int addr)
{
    symbol *sym = malloc(sizeof(symbol));
    if (!sym)
    {
        return NULL; // TODO REPORT ERROR
    }
    sym->kind = PROC_SYMBOL_KIND;
    sym->name = malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(sym->name, name);
    sym->level = level;
    sym->addr = addr;
    return sym;
}

symbol *createVarSymbol(char *name, int level, int addr)
{
    symbol *sym = malloc(sizeof(symbol));
    if (!sym)
    {
        return NULL; // TODO REPORT ERROR
    }
    sym->kind = VAR_SYMBOL_KIND;
    sym->name = malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(sym->name, name);
    sym->level = level;
    sym->addr = addr;
    return sym;
}

symbol *createConstSymbol(char *name, int val)
{
    symbol *sym = malloc(sizeof(symbol));
    if (!sym)
    {
        return NULL; // TODO REPORT ERROR
    }
    sym->kind = CONST_SYMBOL_KIND;
    sym->name = malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(sym->name, name);
    sym->val = val;
    return sym;
}

symbol *createTempSymbol(char *name)
{
    symbol *sym = malloc(sizeof(symbol));
    if (!sym)
    {
        return NULL; // TODO REPORT ERROR
    }
    sym->name = malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(sym->name, name);
    sym->kind = -1;
    return sym;
}

symbol_table *destroySymbolTable(symbol_table *table)
{
    if (table == NULL)
        return NULL;

    if (table->symbols == NULL)
    {
        free(table);
        return NULL;
    }

    for (int i = 0; i < table->tp; i++)
    {
        destroySymbol(table->symbols[i]);
    }

    free(table->symbols);
    free(table);
    return NULL;
}

symbol *tableLookUp(symbol_table *table, char *name)
{
    table->symbols[0] = createTempSymbol(name);
    for (int i = table->tp - 1; i > 0; i--)
        if (strcmp(name, table->symbols[i]->name) == 0)
            return table->symbols[i];
    return NULL;
}

symbol_table *tableInsert(symbol_table *table, symbol *sym)
{
    if (table == NULL)
        return NULL;

    if (sym == NULL)
        return table;

    if (table->tp == MAX_SYMBOL_TABLE_SIZE - 1)
    {
        return NULL; // TODO ERRS
    }
    table->symbols[table->tp] = sym;
    table->tp++;
    return table;
}

symbol *destroySymbol(symbol *sym)
{
    if (sym == NULL)
        return NULL;
    free(sym->name);
    free(sym);
    return NULL;
}

symbol_table *deleteSymbol(symbol_table *table, char *name)
{
    if (table == NULL)
    {
        return NULL;
    }

    if (name == NULL)
    {
        return table;
    }

    int prev = table->tp;
    for (int i = table->tp - 1; i > 0; i--)
    {
        symbol *curr = table->symbols[i];
        if (strcmp(name, curr->name) == 0)
        {
            curr = destroySymbol(curr);
            while (prev < table->tp)
            {
                symbol *temp = table->symbols[prev];
                table->symbols[prev] = table->symbols[prev - 1];
                table->symbols[prev - 1] = temp;
                prev++;
            }
            table->tp--;
            break;
        }
        prev = i;
    }
    return table;
}

void printSymbol(symbol *sym)
{
    if (sym == NULL)
        return;

    if (sym->kind != CONST_SYMBOL_KIND)
        printf("[name: \"%s\" add: \"%d\" level: \"%d\"]", sym->name, sym->addr, sym->level);
    else
        printf("[name: \"%s\" val: \"%d\"]", sym->name, sym->val);
}

void printSymbolTable(symbol_table *table)
{
    if (table == NULL || table->symbols == NULL)
        return;

    for (int i = 1; i < table->tp; i++)
    {
        printf("index %d ", i);
        printSymbol(table->symbols[i]);
        printf("\n");
    }
    printf("\n");
}
