#include "tokenizer.h"

void decideKeywordOrIdentifier(JackTokenizer *tokenizer){
    bool flag = false;
    for(size_t i=0;i<sizeof(keywords)/sizeof(keywords[0]);++i){
        if(strcmp(tokenizer->token, keywords[i].str) == 0){
            tokenizer->tokenkind = TK_KEYWORD;
            tokenizer->keywordkind = keywords[i].kind;
            flag = true;
            break;
        }
    }

    if(!flag) tokenizer->tokenkind = TK_IDENTIFIER;
}

JackTokenizer *init(FILE *fp_jack){
    static JackTokenizer tokenizer;
    tokenizer.fp = fp_jack;
    fseek(tokenizer.fp, 0, SEEK_SET);
    skip(tokenizer.fp);
    strcpy(tokenizer.token, "");

    return &tokenizer;
}

bool hasMoreTokens(JackTokenizer *tokenizer){
    return !isEOF(tokenizer->fp);
}

void advance(JackTokenizer *tokenizer){
    if(getSymbol(tokenizer->fp, tokenizer->token)) tokenizer->tokenkind = TK_SYMBOL;
    else if(getIntConst(tokenizer->fp, tokenizer->token)) tokenizer->tokenkind = TK_INTCONST;
    else if(getStringConst(tokenizer->fp, tokenizer->token)) tokenizer->tokenkind = TK_STRCONST;
    else{
        getKeywordOrIdentifier(tokenizer->fp, tokenizer->token);
        decideKeywordOrIdentifier(tokenizer);
    }
    skip(tokenizer->fp);
}

TokenKind tokenType(JackTokenizer *tokenizer){
    return tokenizer->tokenkind;
}

KeywordKind keyWord(JackTokenizer *tokenizer){
    return tokenizer->keywordkind;
}

void symbol(JackTokenizer *tokenizer, char *symbol){
    if(tokenizer->tokenkind == TK_SYMBOL) strcpy(symbol, tokenizer->token);
}

void identifier(JackTokenizer *tokenizer, char *identifier){
    if(tokenizer->tokenkind == TK_IDENTIFIER) strcpy(identifier, tokenizer->token);
}

void intVal(JackTokenizer *tokenizer, int *intVal){
    if(tokenizer->tokenkind == TK_INTCONST) *intVal = atoi(tokenizer->token);
}

void stringVal(JackTokenizer *tokenizer, char *stringVal){
    if(tokenizer->tokenkind == TK_STRCONST) strcpy(stringVal, tokenizer->token);
}