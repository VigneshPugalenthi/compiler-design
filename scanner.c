#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ctype.h"

typedef enum {id, literal, read, write, becomes,
                lparen, rparen, add, sub, mul, divn, eof} token;

char token_image[100];

char* names[] = {"id", "literal", "read", "write", "becomes",
                 "lparen", "rparen", "add", "sub", "mul", "div", "eof"};

typedef struct{
    token tok;
    int line_number;
} Scanner;

Scanner scanner = {.line_number = 1};

Scanner scan() {
    static int c = ' ';
        /* next available char; extra (int) width accommodates EOF */
    int i = 0;              /* index into token_image */

    /* skip white space */
    while (isspace(c)) {
        if (c == '\n') scanner.line_number++;
        c = getchar();
    }
    if (c == EOF)
        return (Scanner) {.tok = eof, .line_number = scanner.line_number};
    if (isalpha(c)) {
        do {
            token_image[i++] = c;
            c = getchar();
        } while (isalpha(c) || isdigit(c) || c == '_');
        token_image[i] = '\0';
        if (!strcmp(token_image, "read")) return (Scanner) {.tok = read, .line_number = scanner.line_number};
        else if (!strcmp(token_image, "write")) return (Scanner) {.tok = write, .line_number = scanner.line_number};
        else return (Scanner) {.tok = id, .line_number = scanner.line_number};
    }
    else if (isdigit(c)) {
        do {
            token_image[i++] = c;
            c = getchar();
        } while (isdigit(c));
        token_image[i] = '\0';
        return (Scanner) {.tok = literal, .line_number = scanner.line_number};
    } else switch (c) {
        case ':':
            if ((c = getchar()) != '=') {
                fprintf(stderr, "Not an valid assignment operator. Lexical error at line number: %d\n", scanner.line_number);
                exit(1);
            } else {
                c = getchar();
                return (Scanner) {.tok = becomes, .line_number = scanner.line_number};
            }
            break;
        case '+': c = getchar(); return (Scanner) {.tok = add, .line_number = scanner.line_number};
        case '-': c = getchar(); return (Scanner) {.tok = sub, .line_number = scanner.line_number};
        case '*': c = getchar(); return (Scanner) {.tok = mul, .line_number = scanner.line_number};
        case '/': 
            c = getchar(); 
            if (c == '/') {
                // Line comment, skip until newline
                while (c != '\n' && c != EOF) {
                    c = getchar();
                }
                return scan();
            } else if (c == '*') {
                // Block comment, skip until "*/"
                int prev_c = ' ';
                while (!((prev_c == '*' && c == '/') || c == EOF)) {
                    prev_c = c;
                    c = getchar();
                }
                if (c == EOF) {
                    fprintf(stderr, "Unclosed block comment. Syntax error at line number:%d\n", scanner.line_number);
                    exit(1);
                }
                c = getchar();
                return scan();
            }  
            else {
                return (Scanner) {.tok = divn, .line_number = scanner.line_number};
            }
        case '(': c = getchar(); return (Scanner) {.tok = lparen, .line_number = scanner.line_number};
        case ')': c = getchar(); return (Scanner) {.tok = rparen, .line_number = scanner.line_number};
        default:
            printf("The input token is not valid. Lexical error at line number:%d\n", scanner.line_number);
            exit(1);
    }
}
