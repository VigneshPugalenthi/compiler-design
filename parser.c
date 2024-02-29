#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "scanner.h"

#define NUM_NON_TERMINALS 10
#define NUM_TERMINALS 12
#define STACK_SIZE 200

// Grammar for the parser
/* =========================================
program -> stmt_list $$
stmt_list -> stmt stmt_list | ε
stmt -> id := expr | read id | write expr
expr -> term term_tail
term_tail -> add_op term term_tail | ε
term -> factor factor_tail
factor_tail -> mult_op factor factor_tail | ε
factor -> ( expr ) | id | number
add_op -> + | -
mult_op -> * | /
=========================================== */

// Hardcoded predict sets for terminal and non-terminal symbols
/* ---------------------------------------------------------------------------
| State       | id | number | read | write | := | ( | ) | + | - | * | / | $$ |
|-------------|----|--------|------|-------|----|---|---|---|---|---|---|----|
| program     | 1  | 0      | 1    | 1     | 0  | 0 | 0 | 0 | 0 | 0 | 0 | 1  |
| stmt_list   | 2  | 0      | 2    | 2     | 0  | 0 | 0 | 0 | 0 | 0 | 0 | 3  |
| stmt        | 4  | 0      | 5    | 6     | 0  | 0 | 0 | 0 | 0 | 0 | 0 | 0  |
| expr        | 7  | 7      | 0    | 0     | 0  | 7 | 0 | 0 | 0 | 0 | 0 | 0  |
| term_tail   | 9  | 0      | 9    | 9     | 0  | 0 | 9 | 8 | 8 | 0 | 0 | 9  |
| term        | 10 | 10     | 0    | 0     | 0  | 10| 0 | 0 | 0 | 0 | 0 | 0  |
| factor_tail | 12 | 0      | 12   | 12    | 0  | 0 | 12| 12| 12| 11| 11| 12 |
| factor      | 14 | 15     | 0    | 0     | 0  | 13| 0 | 0 | 0 | 0 | 0 | 0  |
| add_op      | 0  | 0      | 0    | 0     | 0  | 0 | 0 | 16| 17| 0 | 0 | 0  |
| mult_op     | 0  | 0      | 0    | 0     | 0  | 0 | 0 | 0 | 0 | 18| 19| 0  |
------------------------------------------------------------------------------*/

Scanner scanner_token;
typedef enum
{
    program = 22,
    stmt_list,
    stmt,
    expr,
    term_tail,
    term,
    factor_tail,
    factor,
    add_op,
    mult_op
} Symbol;
char *terminalNames[NUM_TERMINALS] = {"id", "number", "read", "write", ":=", "(", ")", "+", "-", "*", "/", "$$"};
char *nonTerminalNames[NUM_NON_TERMINALS] = {"program", "stmt_list", "stmt", "expr", "term_tail", "term", "factor_tail", "factor", "add_op", "mult_op"};

void push(int stack[], int *top, int symbol)
{
    stack[++(*top)] = symbol;
}

int pop(int stack[], int *top)
{
    return stack[(*top)--];
}

bool isNonTerminal(Symbol symbol)
{
    if (symbol >= program && symbol <= mult_op)
    {
        return true;
    }
    return false;
}

void parse()
{
    int predictSets[NUM_NON_TERMINALS][NUM_TERMINALS] = {
        {1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1},          // program
        {2, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 3},          // stmt_list
        {4, 0, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0},          // stmt
        {7, 7, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0},          // expr
        {9, 0, 9, 9, 0, 0, 9, 8, 8, 0, 0, 9},          // term_tail
        {10, 10, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0},       // term
        {12, 0, 12, 12, 0, 0, 12, 12, 12, 11, 11, 12}, // factor_tail
        {14, 15, 0, 0, 0, 13, 0, 0, 0, 0, 0, 0},       // factor
        {0, 0, 0, 0, 0, 0, 0, 16, 17, 0, 0, 0},        // add_op
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 18, 19, 0}         // mult_op
    };

    int stack[STACK_SIZE];
    int top = -1;
    push(stack, &top, program);
    scanner_token = scan();
    printf("Scanner token: %s\n", terminalNames[scanner_token.tok]);

    while (top != -1)
    {
        printf("\n");
        int symbol = pop(stack, &top);
        if (isNonTerminal(symbol))
        {
            printf("Popped non-terminal: %s\n", nonTerminalNames[symbol - 22]);
            int row = symbol - 22;
            int col = scanner_token.tok;
            int production = predictSets[row][col];
            switch (production)
            {
            case 1: // program -> stmt_list $$
                printf("Production rule: program -> stmt_list $$\n");
                push(stack, &top, stmt_list);
                break;
            case 2: // stmt_list -> stmt stmt_list | ε
                printf("Production rule: stmt_list -> stmt stmt_list | ε\n");
                push(stack, &top, stmt_list);
                push(stack, &top, stmt);
                break;
            case 3: // stmt_list -> stmt stmt_list | ε
                break;
            case 4: // stmt -> id := expr
                printf("Production rule: stmt -> id := expr\n");
                push(stack, &top, expr);
                push(stack, &top, becomes);
                push(stack, &top, id);
                break;
            case 5: // stmt -> read id
                printf("Production rule: stmt -> read id\n");
                push(stack, &top, id);
                push(stack, &top, read);
                break;
            case 6: // stmt -> write expr
                printf("Production rule: stmt -> write expr\n");
                push(stack, &top, expr);
                push(stack, &top, write);
                break;
            case 7: // expr -> term term_tail
                printf("Production rule: expr -> term term_tail\n");
                push(stack, &top, term_tail);
                push(stack, &top, term);
                break;
            case 8: // term_tail -> add_op term term_tail | ε
                printf("Production rule: term_tail -> add_op term term_tail | ε\n");
                push(stack, &top, term_tail);
                push(stack, &top, term);
                push(stack, &top, add_op);
                break;
            case 9: // term_tail -> add_op term term_tail | ε
                break;
            case 10: // term -> factor factor_tail
                printf("Production rule: term -> factor factor_tail\n");
                push(stack, &top, factor_tail);
                push(stack, &top, factor);
                break;
            case 11: // factor_tail -> mult_op factor factor_tail | ε
                printf("Production rule: factor_tail -> mult_op factor factor_tail | ε\n");
                push(stack, &top, factor_tail);
                push(stack, &top, factor);
                push(stack, &top, mult_op);
                break;
            case 12: // factor_tail -> mult_op factor factor_tail | ε
                break;
            case 13: // factor -> ( expr )
                printf("Production rule: factor -> ( expr )\n");
                push(stack, &top, rparen);
                push(stack, &top, expr);
                push(stack, &top, lparen);
                break;
            case 14: // factor -> id
                printf("Production rule: factor -> id\n");
                push(stack, &top, id);
                break;
            case 15: // factor -> number
                printf("Production rule: factor -> number\n");
                push(stack, &top, literal);
                break;
            case 16: // add_op -> +
                printf("Production rule: add_op -> +\n");
                push(stack, &top, add);
                break;
            case 17: // add_op -> -
                printf("Production rule: add_op -> -\n");
                push(stack, &top, sub);
                break;
            case 18: // mult_op -> *
                printf("Production rule: mult_op -> *\n");
                push(stack, &top, mul);
                break;
            case 19: // mult_op -> /
                printf("Production rule: mult_op -> /\n");
                push(stack, &top, divn);
                break;
            default:
                fprintf(stderr, "Syntax error at line number %d\n", scanner_token.line_number);
                exit(1);
            }
        }
        else
        {
            if (symbol == scanner_token.tok)
            {
                printf("Token matched and popped from the stack: %s\n", terminalNames[symbol]);
                scanner_token = scan();
                printf("Next scanner token: %s\n", terminalNames[scanner_token.tok]);
            }
            else
            {
                fprintf(stderr, "Syntax error at line number: %d\n", scanner_token.line_number-1);
                exit(1);
            }
        }
    }
    printf("Parsing successful: No lexical or syntax error\n");
}

int main()
{
    parse();
    return 0;
}