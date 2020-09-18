// Carter Albrecht, ca845545
// COP3402 Montagne, Spring 2020

#ifndef PL0_SYMBOL_H
#define PL0_SYMBOL_H

#define MAX_SYMBOL_TABLE_SIZE 20000 // ???
#define CONST_SYMBOL_KIND 1
#define VAR_SYMBOL_KIND 2
#define PROC_SYMBOL_KIND 3

typedef struct
{
    int kind; 		// const = 1, var = 2
    char *name;	// name up to 11 chars
    int val; 		// number (ASCII value)
    int level; 		// L level
    int addr; 		// M address
} symbol;

typedef struct
{
    symbol **symbols;
    int tp; // table pointer
} symbol_table;

symbol *createProcSymbol(char *name, int level, int addr);

symbol *createVarSymbol(char *name, int level, int addr);

symbol *destroySymbol(symbol *sym);

symbol *createConstSymbol(char *name, int val);

symbol_table *createSymbolTable();

symbol_table *tableInsert(symbol_table *table, symbol *sym);

symbol *tableLookUp(symbol_table *table, char *name);

symbol_table *destroySymbolTable(symbol_table *table);

symbol_table *deleteSymbol(symbol_table *table, char *name);

symbol *createTempSymbol(char *name);

void printSymbol(symbol *sym);

void printSymbolTable(symbol_table *table);

#endif //PL0_SYMBOL_H
