#include <stdio.h>
#include <string.h>

typedef enum TokenType {
    TOKEN_IDENTIFIER,
    TOKEN_INT_LITERAL,  
    TOKEN_OPEN_BRACE,
    TOKEN_CLOSE_BRACE,
    TOKEN_OPEN_PAR,
    TOKEN_CLOSE_PAR,
    TOKEN_SEMICOL,
    TOKEN_INT,
    TOKEN_RET,
} TokenType;

typedef struct Token {
    TokenType type;
    char *lexeme;
    int line;
} Token;


Token* Lexer(const char *src);

int main(void) {


return 0;
}


Token* Lexer(char *src) {

    FILE *fp = fopen(src, "r");
    

}
