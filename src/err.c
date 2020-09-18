// Carter Albrecht, ca845545
// COP3402 Montagne, Spring 2020

#include "../include/err.h"
#include <stdio.h>
#include <stdlib.h>

void err_msg_exit(char *msg)
{
    //printf(ANSI_COLOR_RED "ERROR: " ANSI_COLOR_RESET "%s\n", msg);
    printf("ERROR: %s\n", msg);
    exit(1);
}
