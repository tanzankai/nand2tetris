#include<stdio.h>
#include<stdbool.h>

typedef enum{
    TK_KEYWORD, // keyword
    TK_SYMBOL, // symbol
    TK_INTCONST, // integerConstant
    TK_STRCONST, // stringConstant
    TK_IDENTIFIER, // identifier
} TokenKind;

typedef enum{
    KW_CLASS, // class
    KW_CONSTRUCTOR, // constructor
    KW_FUNCTION, // function
    KW_METHOD, // method
    KW_FIELD, // field
    KW_STATIC, // static
    KW_VAR, // var
    KW_INT, // int
    KW_CHAR, // char
    KW_BOOLEAN, // boolean
    KW_VOID, // void
    KW_TRUE, // true
    KW_FALSE, // false
    KW_NULL, // null
    KW_tokenizer, // tokenizer
    KW_LET, // let
    KW_DO, // do
    KW_IF, // if
    KW_ELSE, // else
    KW_WHILE, // while
    KW_RETURN, // return
} KeywordKind;

struct keyword{
    KeywordKind kind;
    char *str;
};

struct keyword keywords[] = {
    {KW_CLASS, "class"},
    {KW_CONSTRUCTOR, "constructor"},
    {KW_FUNCTION, "function"},
    {KW_METHOD, "method"},
    {KW_FIELD, "field"},
    {KW_STATIC, "static"},
    {KW_VAR, "var"},
    {KW_INT, "int"},
    {KW_CHAR, "char"},
    {KW_BOOLEAN, "boolean"},
    {KW_VOID, "void"},
    {KW_TRUE, "true"},
    {KW_FALSE, "false"},
    {KW_NULL, "null"},
    {KW_tokenizer, "tokenizer"},
    {KW_LET, "let"},
    {KW_DO, "do"},
    {KW_IF, "if"},
    {KW_ELSE, "else"},
    {KW_WHILE, "while"},
    {KW_RETURN, "return"}
};

typedef struct JackTokenizer JackTokenizer;

struct JackTokenizer{
    FILE *fp_jack;
    char token[100];
    TokenKind tokenkind;
    KeywordKind keywordkind;
};

void decideKeywordOrIdentifier(JackTokenizer *tokenizer);
JackTokenizer *tokenizerInit(FILE *fp_jack);
bool hasMoreTokens(JackTokenizer *tokenizer);
void advance(JackTokenizer *tokenizer);
TokenKind tokenType(JackTokenizer *tokenizer);
KeywordKind keyWord(JackTokenizer *tokenizer);
void symbol(JackTokenizer *tokenizer, char *symbol);
void identifier(JackTokenizer *tokenizer, char *identifier);
void intVal(JackTokenizer *tokenizer, int *intVal);
void stringVal(JackTokenizer *tokenizer, char *stringVal);