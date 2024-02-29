# Objective
The main goal of the assignment is to build a scanner and parser that compiles a given input against a specific context-free grammar and determines whether parsing can be successful or not.

## Scanner
The scanner.c will read input from the given file and return a sequence of tokens. Additionally, it validates whether comments present in the input are valid or not.

## Parser
The parser.c file will receive tokens from the scanner and use LL(1) parsing (also known as table-driven top-down parsing) to parse the tokens. The idea is to push symbols into the stack based on the production rules and validate them against the tokens. If both the token and symbol match, then the parser moves to the next token; otherwise, it returns a syntax error if production rule can't be extended further.

## Implementation
1. The parser starts with an empty stack and receives the first token from the scanner.
2. It looks at the current token and the symbol at the top of the stack.
3. If the token and symbol match, the parser moves to the next token and pops the symbol from the stack.
4. If the symbol at the top of the stack is a non-terminal, the parser expands it by replacing it with the corresponding production rule from the grammar.
5. The parser pushes the symbols of the production rule onto the stack in reverse order.
6. Steps 2-5 are repeated until the stack is empty or a syntax error occurs.
7. If the stack is empty and there are no more tokens, it means the parsing was successful and the input adheres to the grammar. Otherwise, if there are tokens remaining or the stack is not empty, it indicates a syntax error.

## Context-Free Grammar
Below is the production rule used by the parser
``` 
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
```
## Execution
- To check gcc installation
`gcc --version`
- To compile both the programs
`gcc scanner.c parser.c -o parser`
- To run the program with input file
`./parser < testFiles/prog1`

## Conflicts
- Encountered a duplicate symbol error when trying to compile both the scanner and parser. To address this, I've used the scanner.h file, which essentially serves as a reference for the compiler.
- I believe prog2 given in the assignment should return syntax error, since all tokens are valid, it can't be lexically incorrect.

## Testing
:point_right: Input file: `prog1`
![](/output-screenshots/prog1.gif)

:point_right: Input file: `prog2`
![](/output-screenshots/prog2.png)

:point_right: Input file: `prog3`
![](/output-screenshots/prog3.png)

:point_right: Input file: `prog4`
![](/output-screenshots/prog4.gif)

:point_right: Input file: `prog5`
![](/output-screenshots/prog5.gif)

:point_right: Input file: `prog6`
![](/output-screenshots/prog6.gif)

:point_right: Input file: `prog7`
![](/output-screenshots/prog7.png)

:point_right: Input file: `prog8`
![](/output-screenshots/prog8.gif)


[:file_folder: GitHub repository](https://github.com/VigneshPugalenthi/compiler-design)
