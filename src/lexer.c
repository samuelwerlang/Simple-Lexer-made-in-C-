#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

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

static void scan_token (
    unsigned char ch, 
    Token *Token_list, 
    size_t capacity, 
    size_t *count, 
    int *line
);

Token* Scanner(const char *src, size_t capacity, size_t *out_count);

int main(void) {

    size_t counter = 0;
    Token *tokens = Scanner("./test.c", 1024, &counter);
    if (!tokens) {
        return 1;
    }
    
    for (size_t i = 0; i < counter; i++) {
        printf("Token Type: %d\n, lexeme: %s\n, line: %d", 
        tokens[i].type, tokens[i].lexeme, tokens[i].line);
    }

    
    for (size_t i = 0; i < counter; i++) {
        free(tokens[i].lexeme);
    }
    free(tokens);

    return 0;
}

static void scan_token (
    unsigned char ch, 
    Token *Token_list, 
    size_t capacity, 
    size_t *count, 
    int *line
) {

    if (isspace(ch)) {
        if (ch == '\n') (*line)++ ;
    return;
}
    switch (ch) {
        case '(': {
                char *lex = malloc(sizeof(char) * 2);
                lex[0] = '(';
                lex[1] = '\0';

                Token_list[(*count)++] = (Token){
                    .type = TOKEN_OPEN_PAR, 
                    .lexeme = lex,
                    .line = *line
                };
               break;  
        }
    }
}

Token* Scanner(const char *src, size_t capacity, size_t *out_count) {

    Token *Token_list = malloc(capacity * sizeof(Token));
    if(!Token_list) { 
        return NULL;
    }

    FILE *fp = fopen(src, "r");
    if (fp == NULL)  {
        fprintf(stderr, "Error while reading the file");
        free(Token_list);
        return NULL; 
    }

    // Scanner main loop
    int c;
    size_t count = 0;
    int line = 1;

    while ((c = fgetc(fp)) != EOF) {
        unsigned char ch = (unsigned char)c;
        scan_token(ch, Token_list, capacity, &count, &line);
    }

    *out_count = count;
    fclose(fp);
    return Token_list;
}
