#include<stdio.h>
#include"tokenizer.h"

typedef struct CompilationEngine CompilationEngine;

struct CompilationEngine{
    JackTokenizer *tokenizer;
    FILE *fp_xml;
};

CompilationEngine *compilationEngineInit(FILE *fp_jack, FILE *fp_xml);
void advanceWrite(CompilationEngine *comp);
bool isSymbolToken(CompilationEngine *comp, char *token);
bool isKeyword(CompilationEngine *comp, KeywordKind kind);
void compileClass(CompilationEngine *comp);
void compileClassVarDec(CompilationEngine *comp);
void compileSubroutine(CompilationEngine *comp);
void compileParameterList(CompilationEngine *comp);
void compileVarDec(CompilationEngine *comp);
void compileStatements(CompilationEngine *comp);
void compileDo(CompilationEngine *comp);
void compileLet(CompilationEngine *comp);
void compileWhile(CompilationEngine *comp);
void compileReturn(CompilationEngine *comp);
void compileIf(CompilationEngine *comp);
void compileExpression(CompilationEngine *comp);
void compileTerm(CompilationEngine *comp);
void compileExpressionList(CompilationEngine *comp);