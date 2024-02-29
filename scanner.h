#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ctype.h"

typedef enum {id, literal, read, write, becomes, lparen, rparen,
                add, sub, mul, divn, eof} token;

typedef struct{
    token tok;
    int line_number;
} Scanner;

extern Scanner scan();

#endif /* SCANNER_H */
