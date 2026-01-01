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
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_DOT,
    TOKEN_COMMA,
    TOKEN_EXCLAMATION,
    TOKEN_EXCLAMATION_EQUAL,
    TOKEN_EQUALS,
    TOKEN_EQUALS_EQUALS,
    TOKEN_GREATER,
    TOKEN_GREATER_OR_EQ,
    TOKEN_LESS,
    TOKEN_LESS_OR_EQ,
    TOKEN_INT,
    TOKEN_RET,
} TokenType;

typedef struct Token {
    TokenType type;
    char *lexeme;
    int line;
} Token;

static void scan_token (
    FILE *fp,
    unsigned char ch, 
    Token *Token_list, 
    size_t capacity, 
    size_t *count, 
    int *line
);

static void add_lex(unsigned char ch, Token *Token_list, size_t *count, int* line, TokenType tk_type);

int peek(FILE *fp) {
    int c = fgetc(fp);
    if (c != EOF) ungetc(c, fp);
    return c;
}

static bool match(FILE *fp, unsigned char expected_char); 
 
Token* file_scan(const char *src, size_t capacity, size_t *out_count);

int main(void) {

    size_t counter = 0;
    Token *tokens = file_scan("./test.c", 1024, &counter);
    if (!tokens) {
        return 1;
    }
    
    for (size_t i = 0; i < counter; i++) {
        printf("Token Type: %d, lexeme: %s, line: %d\n", 
        tokens[i].type, 
        tokens[i].lexeme, 
        tokens[i].line);
    }

    
    for (size_t i = 0; i < counter; i++) {
        free(tokens[i].lexeme);
    }
    free(tokens);

    return 0;
}

static void add_lex(unsigned char ch, Token *Token_list, size_t *count, int* line, TokenType tk_type) {

    //char *keywords[5] = {"int", "for", "while", "if", "else"};
    char *lex = malloc(sizeof(char) * 2);
                lex[0] = ch;
                lex[1] = '\0';

                Token_list[(*count)++] = (Token){
                    .type = tk_type, 
                    .lexeme = lex,
                    .line = *line
                };
}

static bool match(FILE *fp, unsigned char char_expected) { 
            int next_char = fgetc(fp);
            if (char_expected == next_char) {
                return true;
            } 
            if (next_char != EOF) {
                ungetc(next_char, fp);
            }
        return false;
} 

static void scan_token (
    FILE *fp,
    unsigned char ch, 
    Token *Token_list, 
    size_t capacity, 
    size_t *count, 
    int *line
) {

    if (*count >=capacity) {
        fprintf(stderr, "Token capacity exceeded");
        return;   
    }
    if (isspace(ch)) {
        if (ch == '\n') (*line)++ ;
        return;
    }

    switch (ch) {
        case '(': {
            add_lex(ch, Token_list, count, line, TOKEN_OPEN_PAR);
            break;  
        }
        case ')': {
            add_lex(ch, Token_list, count, line, TOKEN_CLOSE_PAR);
            break;
        }
        case '{': {
            add_lex(ch, Token_list, count, line, TOKEN_OPEN_BRACE);
            break;
        } 
        case '}': {
            add_lex(ch, Token_list, count, line, TOKEN_CLOSE_BRACE);
            break;
        }
        case ';': {
            add_lex(ch, Token_list, count, line, TOKEN_SEMICOL);
            break;
        }
        case '.': {
            add_lex(ch, Token_list, count, line, TOKEN_DOT);
            break;
        }
        case '+': {
            add_lex(ch, Token_list, count, line, TOKEN_PLUS);
            break;
        }
        case '-': {
            add_lex(ch, Token_list, count, line, TOKEN_MINUS);
            break;
        }
        case ',': {
            add_lex(ch, Token_list, count, line, TOKEN_COMMA);
            break;
        }
         case '!': {
            match(fp, '=') ? add_lex(ch, Token_list, count, line, TOKEN_EXCLAMATION_EQUAL):
                             add_lex(ch, Token_list, count, line, TOKEN_EXCLAMATION); 
            break;
        }
        case '=': { 
            match(fp, '=') ? add_lex(ch, Token_list, count, line, TOKEN_EQUALS_EQUALS):
                             add_lex(ch, Token_list, count, line, TOKEN_EQUALS);
        }
        case '>': { 
            match(fp, '=') ? add_lex(ch, Token_list, count, line, TOKEN_GREATER_OR_EQ):
                             add_lex(ch, Token_list, count, line, TOKEN_GREATER);
        }
        case '<': { 
            match(fp, '=') ? add_lex(ch, Token_list, count, line, TOKEN_LESS_OR_EQ):
                             add_lex(ch, Token_list, count, line, TOKEN_LESS);
        }
         default: {
            fprintf(stderr, "Unexpected character at line:%d\n", *line);
            break;
        }
    }
}

Token* file_scan(const char *src, size_t capacity, size_t *out_count) {

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
        scan_token(fp, ch, Token_list, capacity, &count, &line);
    }

    *out_count = count;
    fclose(fp);
    return Token_list;
}
