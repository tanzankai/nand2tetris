#include"codegen.h"

void writeKeyword(FILE *fp, JackTokenizer *this){
    KeywordKind kind = keyWord(this);
    fprintf(fp, "<keyword> ");
    for(size_t i=0;i<sizeof(keywords)/sizeof(keywords[0]);++i){
        if(kind == keywords[i].kind){
            fprintf(fp, "%s", keywords[i].str);
            break;
        }
    }
    fprintf(fp, " </keyword>\n");
}

void writeSymbol(FILE *fp, JackTokenizer *this){
    char token[100];
    symbol(this, token);
    fprintf(fp, "<symbol> ");
    if(strcmp(token, "<") == 0) fprintf(fp, "&lt;");
    else if(strcmp(token, ">") == 0) fprintf(fp, "&gt;");
    else if(strcmp(token, "&") == 0) fprintf(fp, "&amp;");
    else fprintf(fp, "%s", token);
    fprintf(fp, " </symbol>\n");
}

void writeIntConst(FILE *fp, JackTokenizer *this){
    int val;
    intVal(this, &val);
    fprintf(fp, "<integerConstant> %d </integerConstant>\n", val);
}

void writeStrConst(FILE *fp, JackTokenizer *this){
    char token[100];
    stringVal(this, token);
    fprintf(fp, "<stringConstant> %s </stringConstant>\n", token);
}

void writeIdentifier(FILE *fp, JackTokenizer *this){
    char token[100];
    identifier(this, token);
    fprintf(fp, "<identifier> %s </identifier>\n", token);
}

void writeToken(FILE *fp, JackTokenizer *this){
    TokenKind kind = tokenType(this);
    if(kind == TK_KEYWORD){
        writeKeyword(fp, this);
    }
    else if(kind == TK_SYMBOL){
        writeSymbol(fp, this);
    }
    else if(kind == TK_INTCONST){
        writeIntConst(fp, this);
    }
    else if(kind == TK_STRCONST){
        writeStrConst(fp, this);
    }
    else if(kind == TK_IDENTIFIER){
        writeIdentifier(fp, this);
    }
    else{
        error("Error: cannot identify the token \"%s\".", this->token);
    }
}

void advanceWrite(CompilationEngine *comp){
    advance(comp->tokenizer);
    writeToken(comp->fp_xml, comp->tokenizer);
}