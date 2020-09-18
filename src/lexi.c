// Carter Albrecht, ca845545
// COP3402 Montagne, Spring 2020

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../include/lexi.h"
#include "../include/err.h"

// prints the lexeme table given a linked list of tokens
void printLexemeTable(token_list *tokens)
{
    if (tokens == NULL)
        return;

    printf("Lexeme Table:\n");
    printf("%-20s %5s\n", "lexeme", "token-type");
    node *temp = tokens->head;
    while (temp != NULL)
    {
        printf("%-20s %5d\n", temp->data->lexeme, temp->data->type);
        temp = temp->next;
    }
    printf("\n\n");
}

void printSymbolicRepresentation(token_list *tokens)
{
    if (tokens == NULL)
        return;

    node *temp = tokens->head;
    while (temp != NULL)
    {
        printf("%s ", gettokenString(temp->data->type));
        if (temp->data->type == identsym || temp->data->type == numbersym)
            printf("%s ", temp->data->lexeme);
        temp = temp->next;
    }
    printf("\n");
}

// prints the lexeme list given a linked list of tokens
void fprintLexemeList(token_list *tokens, char *filename)
{
    if (tokens == NULL)
        return;

    FILE *out;

    if (filename == NULL)
    {
        out = stdout;
    }
    else
    {
        out = fopen(filename, "w");
        if (!out)
            err_msg_exit(FILE_OPEN_ERR);
    }

    node *temp = tokens->head;
    while (temp != NULL)
    {
        fprintf(out, "%d ", temp->data->type);
        if (temp->data->type == identsym || temp->data->type == numbersym)
            fprintf(out, "%s ", temp->data->lexeme);
        temp = temp->next;
    }
    fprintf(out, "\n");

    if (filename != NULL)
    {
        fclose(out);
    }

}

// prints the source program to be analyzed
void printSourceProgram(FILE *f)
{
    if (f == NULL)
        return;

    // print out input char by char
    printf("Source Program:\n");

    char c;
    while (fscanf(f, "%c", &c) != EOF)
        printf("%c", c);

    printf("\n\n");

    rewind(f); // reset internal read position
}

void start_lexer(char *filename, int lexeme_flag)
{
    FILE *in_file = fopen(filename, "r");

    if (!in_file)
    {
        err_msg_exit(FILE_OPEN_ERR);
    }

    token_list *tokens = createList(); // initialize an empty linked list for the tokens
    if (tokens == NULL) // malloc fail check and cleanup
    {
        fclose(in_file);
        err_msg_exit(MEM_ALLOC_ERR);
    }

    char c; // current char being read

    // main analysis loop
    while ((c = getc(in_file)) != EOF)
    {
        if (isspace(c))
            continue;

        // comment check
        if (c == '/')
        {
            char next;
            if ((next = getc(in_file)) == '*') // if a comment is started
            {
                while (1) // comment loop
                {
                    next = getc(in_file); // read in next char
                    if (next == EOF) // throw err, cleanup, and abort if comment not closed
                    {
                        destroyTokenList(tokens);
                        fclose(in_file);
                        err_msg_exit(CMNT_NOT_CLOSED_ERR);
                    }
                    if (next == '*') // possible comment end
                    {
                        next = getc(in_file);
                        if (next == '/') // if comment is really ended
                        {
                            break;
                        }
                    }
                }
                continue; // once we are out of the comment we can continue
            } else
            {
                ungetc(next, in_file); // a comment was not started so put char back
            }
        }

        if (c == '<') // three possibilities for <
        {
            c = getc(in_file); // get char after
            if (c == '=') // check for leq
            {
                addNode(tokens, leqsym, "<=");
                continue;
            } else if (c == '>') // check for neq
            {
                addNode(tokens, neqsym, "<>");
                continue;
            } else // otherwise it was just a <
            {
                ungetc(c, in_file); // put the char after back
                addNode(tokens, lessym, "<");
                continue;
            }
        }

        if (c == '>')
        {
            c = getc(in_file);
            if (c == '=')
            {
                addNode(tokens, geqsym, ">=");
                continue;
            } else
            {
                ungetc(c, in_file);
                addNode(tokens, gtrsym, ">");
                continue;
            }
        }

        if (c == ':')
        {
            char next = getc(in_file);
            if (next == '=')
            {
                addNode(tokens, becomessym, ":=");
                continue;
            }
            else
            {
                ungetc(next, in_file);
            }
        }

        // check for these single character tokens
        if (c == ';')
        {
            addSingleCharToken(tokens, semicolonsym);
            continue;
        }
        if (c == '+')
        {
            addSingleCharToken(tokens, plussym);
            continue;
        }
        if (c == '-')
        {
            addSingleCharToken(tokens, minussym);
            continue;
        }
        if (c == '*')
        {
            addSingleCharToken(tokens, multsym);
            continue;
        }
        if (c == '/')
        {
            addSingleCharToken(tokens, slashsym);
            continue;
        }
        if (c == '=')
        {
            addSingleCharToken(tokens, eqsym);
            continue;
        }
        if (c == '(')
        {
            addSingleCharToken(tokens, lparentsym);
            continue;
        }
        if (c == ')')
        {
            addSingleCharToken(tokens, rparentsym);
            continue;
        }
        if (c == ',')
        {
            addSingleCharToken(tokens, commasym);
            continue;
        }
        if (c == '.')
        {
            addSingleCharToken(tokens, periodsym);
            continue;
        }

        // number handling
        if (isdigit(c))
        {
            char number_buf[NUMBER_MAX_LENGTH + 1]; // store digits
            int buf_index = 0;
            number_buf[buf_index] = c; // put first digit at first index in buffer
            // keep reading digits
            while (isdigit((c = getc(in_file))))
            {
                buf_index++;
                if (buf_index == NUMBER_MAX_LENGTH) // if the number is too long, err msg, cleanup, exit
                {
                    fclose(in_file);
                    destroyTokenList(tokens);
                    err_msg_exit(LONG_NUM_ERR);
                }
                number_buf[buf_index] = c; // throw next digit in buffer
            }
            number_buf[buf_index + 1] = '\0'; // terminate the digit string
            // if the first character read that wasn't a digit, was a letter, then it was really an identifier with invalid start
            if (isalpha(c))
            {
                fclose(in_file);
                destroyTokenList(tokens);
                err_msg_exit(INVALID_IDNT_ERR);
            }
            ungetc(c, in_file); // we reached non-valid char so put it back
            tokens = addNode(tokens, numbersym, number_buf);
            continue;
        }

        // same logic as number handling
        if (isalpha(c))
        {
            char lexeme_buf[IDENTIFIER_MAX_LENGTH + 1];
            int buf_index = 0;
            lexeme_buf[buf_index] = c;
            while (isalnum(c = getc(in_file)))
            {
                buf_index++;
                if (buf_index == IDENTIFIER_MAX_LENGTH)
                {
                    fclose(in_file);
                    destroyTokenList(tokens);
                    err_msg_exit(LONG_IDENT_ERR);
                }
                lexeme_buf[buf_index] = c;
            }
            lexeme_buf[buf_index + 1] = '\0'; // terminate our lexeme string
            ungetc(c, in_file); // we reached non-valid char so put it back
            addNode(tokens, getNonSpecialType(lexeme_buf), lexeme_buf);
            continue;
        }

        // If we made it this far, the character was unknown
        else
        {
            fclose(in_file);
            destroyTokenList(tokens);
            err_msg_exit(UNKNWN_SYM_ERR);
        }
    }

    if (lexeme_flag)
    {
        printf("\n===== Lexemes =====\n");
        fprintLexemeList(tokens, NULL);
        printSymbolicRepresentation(tokens);
        printf("===================\n\n");
    }

    fprintLexemeList(tokens, LEXEME_LIST_TEMP_FILENAME);

    destroyTokenList(tokens);
    fclose(in_file);
}
