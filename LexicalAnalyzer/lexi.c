// Carter Albrecht, ca845545
// COP3402 Montagne, Spring 2020
// Assignment #2: Lexical Analyzer

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "token.h"

#define IDENTIFIER_MAX_LENGTH 11
#define NUMBER_MAX_LENGTH 5
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_RESET "\x1b[0m"

// prints the beginning of an error message to err_out
void printErrPrefix(FILE *err_out, char *fileName, int line_number, int char_number)
{
    fprintf(err_out, "%s:%d:%d: " ANSI_COLOR_RED "ERROR: " ANSI_COLOR_RESET, fileName, line_number, char_number);
}

// prints the lexeme table given a linked list of tokens
void printLexemeTable(token_list *tokens)
{
    if (tokens == NULL)
        return;

    printf("Lexeme Table:\n");
    printf("%-20s %5s\n", "lexeme", "token-type");
    token *temp = tokens->head;
    while (temp != NULL)
    {
        printf("%-20s %5d\n", temp->lexeme, temp->type);
        temp = temp->next;
    }
    printf("\n\n");
}

// prints the lexeme list given a linked list of tokens
void printLexemeList(token_list *tokens)
{
    if (tokens == NULL)
        return;

    printf("Lexeme List: ");
    token *temp = tokens->head;
    while (temp != NULL)
    {
        printf("%d ", temp->type);
        if (temp->type == identsym || temp->type == numbersym)
            printf("%s ", temp->lexeme);
        temp = temp->next;
    }
    printf("\n");
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

int main(int argc, char **argv)
{
    if (argc < 2) // arg check
    {
        printf("Proper Syntax: ./lexi <input-file>\n");
        return EXIT_FAILURE;
    }

    FILE *err_out = stdout; // possibly add optional err redirection in future
    FILE *in_file = fopen(argv[1], "r");
    if (!in_file)
    {
        printErrPrefix(err_out, argv[1], 0, 0);
        fprintf(err_out, "could not open file\n");
        return EXIT_FAILURE;
    }

    printSourceProgram(in_file); // always print source first

    token_list *tokens = createList(); // initialize an empty linked list for the tokens
    if (tokens == NULL) // malloc fail check and cleanup
    {
        printErrPrefix(err_out, argv[1], 0, 0);
        fprintf(err_out, "could not allocate memory");
        fclose(in_file);
        return EXIT_FAILURE;
    }

    char c; // current char being read

    // for error tracking, start on first line with 0 characters being read so far
    int line_number = 1;
    int char_number = 0;

    // main analysis loop
    while ((c = getc(in_file)) != EOF)
    {
        char_number++;

        // ignore whitespace, if its a newline then increment current line and reset char count
        if (isspace(c))
        {
            if (c == '\n')
            {
                line_number++;
                char_number = 0;
            }
            continue;
        }

        // comment check
        if (c == '/')
        {
            char next;
            int chars_read = 0; // use a seperate counter so we can save where the error started in source
            if (++chars_read && (next = getc(in_file)) == '*') // if a comment is started
            {
                while (1) // comment loop
                {
                    next = getc(in_file); // read in next char
                    chars_read++;
                    if (next == EOF) // throw err, cleanup, and abort if comment not closed
                    {
                        printErrPrefix(err_out,argv[1], line_number, char_number);
                        fprintf(err_out, "comment opened and not closed\n");
                        fclose(in_file);
                        destroyTokenList(tokens);
                        return EXIT_FAILURE;
                    }
                    if (next == '*') // possible comment end
                    {
                        next = getc(in_file);
                        chars_read++;
                        if (next == '/') // if comment is really ended
                        {
                            break;
                        }
                    }
                }
                char_number += chars_read; // since there was no err, update our char number
                continue; // once we are out of the comment we can continue
            } else
            {
                ungetc(next, in_file); // a comment was not started so put char back
            }
        }

        if (c == '<') // three possibilities for <
        {
            c = getc(in_file); // get char after
            char_number++;
            if (c == '=') // check for leq
            {
                addToken(tokens, leqsym, "<=");
                continue;
            } else if (c == '>') // check for neq
            {
                addToken(tokens, neqsym, "<>");
                continue;
            } else // otherwise it was just a <
            {
                ungetc(c, in_file); // put the char after back
                char_number--;
                addToken(tokens, lessym, "<");
                continue;
            }
        }

        if (c == '>')
        {
            c = getc(in_file);
            char_number++;
            if (c == '=')
            {
                addToken(tokens, geqsym, ">=");
                continue;
            } else
            {
                ungetc(c, in_file);
                char_number--;
                addToken(tokens, gtrsym, ">");
                continue;
            }
        }

        if (c == ':')
        {
            char next = getc(in_file);
            char_number++;
            if (next == '=')
            {
                addToken(tokens, becomessym, ":=");
                continue;
            }
            else
            {
                ungetc(next, in_file);
                char_number--;
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
            int chars_read = 0; // separate char tracker to save digit start position in case of err msg
            char number_buf[NUMBER_MAX_LENGTH + 1]; // store digits
            int buf_index = 0;
            number_buf[buf_index] = c; // put first digit at first index in buffer
            // keep reading digits
            while (++chars_read && isdigit((c = getc(in_file))))
            {
                buf_index++;
                if (buf_index == NUMBER_MAX_LENGTH) // if the number is too long, err msg, cleanup, exit
                {
                    number_buf[NUMBER_MAX_LENGTH] = '\0';
                    printErrPrefix(err_out, argv[1], line_number, char_number);
                    fprintf(err_out, "number starting with '%s' exceeds length %d\n", number_buf, NUMBER_MAX_LENGTH);
                    fclose(in_file);
                    destroyTokenList(tokens);
                    return EXIT_FAILURE;
                }
                number_buf[buf_index] = c; // throw next digit in buffer
            }
            number_buf[buf_index + 1] = '\0'; // terminate the digit string
            // if the first character read that wasn't a digit, was a letter, then it was really an identifier with invalid start
            if (isalpha(c))
            {
                printErrPrefix(err_out, argv[1], line_number, char_number);
                fprintf(err_out, "invalid start to identifier: %s\n", number_buf);
                fclose(in_file);
                destroyTokenList(tokens);
                return EXIT_FAILURE;
            }
            ungetc(c, in_file); // we reached non-valid char so put it back
            chars_read--;
            tokens = addToken(tokens, numbersym, number_buf);
            char_number += chars_read; // update our char count
            continue;
        }

        // same logic as number handling
        if (isalpha(c))
        {
            int chars_read = 0;
            char lexeme_buf[IDENTIFIER_MAX_LENGTH + 1];
            int buf_index = 0;
            lexeme_buf[buf_index] = c;
            while (++chars_read && isalnum(c = getc(in_file)))
            {
                buf_index++;
                if (buf_index == IDENTIFIER_MAX_LENGTH)
                {
                    lexeme_buf[IDENTIFIER_MAX_LENGTH] = '\0';
                    printErrPrefix(err_out, argv[1], line_number, char_number);
                    fprintf(err_out, "identifier starting with '%s' exceeds length %d\n", lexeme_buf,
                            IDENTIFIER_MAX_LENGTH);
                    fclose(in_file);
                    destroyTokenList(tokens);
                    return EXIT_FAILURE;
                }
                lexeme_buf[buf_index] = c;
            }
            lexeme_buf[buf_index + 1] = '\0'; // terminate our lexeme string
            ungetc(c, in_file); // we reached non-valid char so put it back
            chars_read--;
            addToken(tokens, getNonSpecialType(lexeme_buf), lexeme_buf);
            char_number += chars_read;
            continue;
        }

        // If we made it this far, the character was unknown
        else
        {
            printErrPrefix(err_out, argv[1], line_number, char_number);
            fprintf(err_out, "unknown symbol '%c'\n", c);
            fclose(in_file);
            destroyTokenList(tokens);
            return EXIT_FAILURE;
        }
    }

    printLexemeTable(tokens);
    printLexemeList(tokens);
    destroyTokenList(tokens);
    fclose(in_file);
    return EXIT_SUCCESS;
}
