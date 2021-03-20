#include "tokenizer.h"

void writeKeyword(FILE *fp, JackTokenizer *tokenizer);
void writeSymbol(FILE *fp, JackTokenizer *tokenizer);
void writeIntConst(FILE *fp, JackTokenizer *tokenizer);
void writeStrConst(FILE *fp, JackTokenizer *tokenizer);
void writeIdentifier(FILE *fp, JackTokenizer *tokenizer);
void writeToken(FILE *fp, JackTokenizer *tokenizer);
void codegen(FILE *fp, JackTokenizer *tokenizer);