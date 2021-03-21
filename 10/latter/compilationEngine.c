#include"compilationEngine.h"

CompilationEngine *compilationEngineInit(FILE *fp_jack, FILE *fp_xml){
    static CompilationEngine comp;
    comp.tokenizer = tokenizerInit(fp_jack);
    comp.fp_xml = fp_xml;
    return &comp;
}

bool isSymbolToken(CompilationEngine *comp, char *token){
    if(tokenType(comp->tokenizer) != TK_SYMBOL) return false;

    char tok[20];
    symbol(comp->tokenizer, tok);

    if(strcmp(token, tok) == 0) return true;
    else return false;
}

bool isKeyword(CompilationEngine *comp, KeywordKind kind){
    if(tokenType(comp->tokenizer) != TK_KEYWORD) return false;
    
    if(keyWord(comp->tokenizer) == kind) return true;
    else return false;
}

// class = 'class' className '{' classVarDec* subroutineDec* '}'
void compileClass(CompilationEngine *comp){
    fprintf(comp->fp_xml, "<class>\n");

    advanceWrite(comp); // 'class'
    advanceWrite(comp); // className = identifier
    advanceWrite(comp); // '{'
    advance(comp->tokenizer); // classVarDec or subroutineDec or '}'

    // classVarDec* or subroutineDec*
    while(!isSymbolToken(comp, "}")){
        if(isKeyword(comp, KW_STATIC) || isKeyword(comp, KW_FIELD)) compileClassVarDec(comp);
        else if(isKeyword(comp, KW_CONSTRUCTOR) || isKeyword(comp, KW_FUNCTION) || isKeyword(comp, KW_METHOD)){
            compileSubroutine(comp);
        }
        else break;
    }

    writeToken(comp->fp_xml, comp->tokenizer); // '}'

    fprintf(comp->fp_xml, "</class>\n");
}

// classVarDec = ('static' | 'field') type varName (',' varName)* ';'
void compileClassVarDec(CompilationEngine *comp){
    fprintf(comp->fp_xml, "<classVarDec>\n");

    writeToken(comp->fp_xml, comp->tokenizer); // 'static' | 'field'
    advanceWrite(comp); // type = 'int' | 'char' | 'boolean' | className
    advanceWrite(comp); // varName = identifier
    advance(comp->tokenizer); // ',' or ';'

    while(!isSymbolToken(comp, ";")){
        writeToken(comp->fp_xml, comp->tokenizer); // ','
        advanceWrite(comp); // varName
        advance(comp->tokenizer);
    }

    writeToken(comp->fp_xml, comp->tokenizer); // ';'

    fprintf(comp->fp_xml, "</classVarDec>\n");
    advance(comp->tokenizer);
}

// subroutineDec = ('constructor' | 'function' | 'method') ('void' | type) subroutineName '(' parameterList ')' subroutineBody
// subroutineBody = '{' varDec* statements '}'
void compileSubroutine(CompilationEngine *comp){
    // subroutineDec
    if(isKeyword(comp, KW_CONSTRUCTOR) || isKeyword(comp, KW_FUNCTION) || isKeyword(comp, KW_METHOD)){
        fprintf(comp->fp_xml, "<subroutineDec>\n");
        writeToken(comp->fp_xml, comp->tokenizer); // 'constructor' | 'function' | 'method'
        advanceWrite(comp); // 'void' | type
        advanceWrite(comp); // subroutineName
        advanceWrite(comp); // '('

        advance(comp->tokenizer); // parameterList
        compileParameterList(comp);

        writeToken(comp->fp_xml, comp->tokenizer); // ')'

        advance(comp->tokenizer); // subroutineBody
        compileSubroutine(comp);

        fprintf(comp->fp_xml, "</subroutineDec>\n");
    }
    // subroutineBody
    else{
        fprintf(comp->fp_xml, "<subroutineBody>\n");
        writeToken(comp->fp_xml, comp->tokenizer); // '{'
        advance(comp->tokenizer); // varDec or statements

        // varDec
        while(isKeyword(comp, KW_VAR)) compileVarDec(comp);

        // statements
        compileStatements(comp);

        writeToken(comp->fp_xml, comp->tokenizer); // '}'

        fprintf(comp->fp_xml,  "</subroutineBody>\n");
        advance(comp->tokenizer);
    }
}

// parameterList = ((type varName) (',' type varName)*)?
// parameterListは'('と')'に挟まれた形でしか出現しないことから、
// 次のトークンが')'であるかどうか調べることでparameterListが空かどうか判定できる
void compileParameterList(CompilationEngine *comp){
    fprintf(comp->fp_xml, "<parameterList>\n");

    if(!isSymbolToken(comp, ")")){
        writeToken(comp->fp_xml, comp->tokenizer); // type
        advanceWrite(comp); // varName
        advance(comp->tokenizer);

        // (',' type varName)*
        while(isSymbolToken(comp, ",")){
            writeToken(comp->fp_xml, comp->tokenizer); // ','
            advanceWrite(comp); // type
            advanceWrite(comp); // varName
            advance(comp->tokenizer);
        }
    }

    fprintf(comp->fp_xml, "</parameterList>\n");
}

// varDec = 'var' type varName (',' varName)* ';'
void compileVarDec(CompilationEngine *comp){
    fprintf(comp->fp_xml, "<varDec>\n");
    writeToken(comp->fp_xml, comp->tokenizer); // 'var'
    advanceWrite(comp); // type
    advanceWrite(comp); // varName
    advance(comp->tokenizer);

    while(!isSymbolToken(comp, ";")){
        writeToken(comp->fp_xml, comp->tokenizer); // ','
        advanceWrite(comp); // varName
        advance(comp->tokenizer);
    }

    writeToken(comp->fp_xml, comp->tokenizer); // ';'

    fprintf(comp->fp_xml, "</varDec>\n");
    advance(comp->tokenizer);
}

// statements = statement*
// statement = letStatement | ifStatement | whileStatement | doStatement | returnStatement

void compileStatements(CompilationEngine *comp){
    fprintf(comp->fp_xml, "<statements>\n");
    while(isKeyword(comp, KW_LET) || isKeyword(comp, KW_IF) || isKeyword(comp, KW_WHILE) || 
        isKeyword(comp, KW_DO) || isKeyword(comp, KW_RETURN)){
        
        if(isKeyword(comp, KW_LET)) compileLet(comp);
        else if(isKeyword(comp, KW_IF)) compileIf(comp);
        else if(isKeyword(comp, KW_WHILE)) compileWhile(comp);
        else if(isKeyword(comp, KW_DO)) compileDo(comp);
        else compileReturn(comp);
    }

    fprintf(comp->fp_xml, "</statements>\n");
}

// doStatement = 'do' subroutineCall ';'
void compileDo(CompilationEngine *comp){
    fprintf(comp->fp_xml, "<doStatement>\n");

    writeToken(comp->fp_xml, comp->tokenizer); // 'do'
    advance(comp->tokenizer);

    // subroutineCall
    writeToken(comp->fp_xml, comp->tokenizer); // subroutineName or (className | varName)
    advance(comp->tokenizer);

    if(isSymbolToken(comp, ".")){
        writeToken(comp->fp_xml, comp->tokenizer); // '.'
        advanceWrite(comp); // subroutineName
        advance(comp->tokenizer);
    }

    writeToken(comp->fp_xml, comp->tokenizer); // '('
    
    advance(comp->tokenizer); // expressionList
    compileExpressionList(comp);

    writeToken(comp->fp_xml, comp->tokenizer); // ')'

    advanceWrite(comp); // ';'
    fprintf(comp->fp_xml, "</doStatement>\n");
    advance(comp->tokenizer);
}

// letStatement = 'let' varName ('[' expression ']')? '=' expression ';'
void compileLet(CompilationEngine *comp){
    fprintf(comp->fp_xml, "<letStatement>\n");

    writeToken(comp->fp_xml, comp->tokenizer); // 'let'
    advanceWrite(comp); // varName
    advance(comp->tokenizer);

    if(isSymbolToken(comp, "[")){
        writeToken(comp->fp_xml, comp->tokenizer); // '['

        advance(comp->tokenizer); // expression
        compileExpression(comp);

        writeToken(comp->fp_xml, comp->tokenizer); // ']'
        advance(comp->tokenizer);
    }

    writeToken(comp->fp_xml, comp->tokenizer); // '='

    advance(comp->tokenizer);
    compileExpression(comp); // expression

    writeToken(comp->fp_xml, comp->tokenizer); // ';'
    
    fprintf(comp->fp_xml, "</letStatement>\n");
    advance(comp->tokenizer);
}

// whileStatement = 'while' '(' expression ')' '{' statements '}'
void compileWhile(CompilationEngine *comp){
    fprintf(comp->fp_xml, "<whileStatement>\n");

    writeToken(comp->fp_xml, comp->tokenizer); // 'while'
    advanceWrite(comp); // '('
    
    advance(comp->tokenizer);
    compileExpression(comp); // expression

    writeToken(comp->fp_xml, comp->tokenizer); // ')'
    advanceWrite(comp); // '{'

    advance(comp->tokenizer);
    compileStatements(comp); // statements

    writeToken(comp->fp_xml, comp->tokenizer); // '}'
    
    fprintf(comp->fp_xml, "</whileStatement>\n");
    advance(comp->tokenizer);
}

// returnStatement = 'return' expression? ';'
void compileReturn(CompilationEngine *comp){
    fprintf(comp->fp_xml, "<returnStatement>\n");

    writeToken(comp->fp_xml, comp->tokenizer); // 'return'
    advance(comp->tokenizer);

    // expression?
    if(!isSymbolToken(comp, ";")){
        compileExpression(comp);
    }

    writeToken(comp->fp_xml, comp->tokenizer); // ';'
    
    fprintf(comp->fp_xml, "</returnStatement>\n");
    advance(comp->tokenizer);
}

// ifStatement = 'if' '(' expression ')' '{' statements '}' ('else' '{' statements '}')?
void compileIf(CompilationEngine *comp){
    fprintf(comp->fp_xml, "<ifStatement>\n");

    writeToken(comp->fp_xml, comp->tokenizer); // 'if'
    advanceWrite(comp); // '('

    advance(comp->tokenizer);
    compileExpression(comp); // expression

    writeToken(comp->fp_xml, comp->tokenizer); // ')'
    advanceWrite(comp); // '{'

    advance(comp->tokenizer);
    compileStatements(comp); // statements

    writeToken(comp->fp_xml, comp->tokenizer); // '}'

    advance(comp->tokenizer);
    if(isKeyword(comp, KW_ELSE)){
        writeToken(comp->fp_xml, comp->tokenizer); // 'else'
        advanceWrite(comp); // '{'
        
        advance(comp->tokenizer);
        compileStatements(comp); // statements

        writeToken(comp->fp_xml, comp->tokenizer); // '}'
        advance(comp->tokenizer);
    }
    
    fprintf(comp->fp_xml, "</ifStatement>\n");
}

// expression = term (op term)*
void compileExpression(CompilationEngine *comp){
    fprintf(comp->fp_xml, "<expression>\n");

    compileTerm(comp);

    while(isSymbolToken(comp, "+") || isSymbolToken(comp, "-") || isSymbolToken(comp, "*") || 
        isSymbolToken(comp, "/") || isSymbolToken(comp, "&") || isSymbolToken(comp, "|") || 
        isSymbolToken(comp, "<") || isSymbolToken(comp, ">") || isSymbolToken(comp, "=")){

        writeToken(comp->fp_xml, comp->tokenizer); // op

        advance(comp->tokenizer);
        compileTerm(comp);
    }

    fprintf(comp->fp_xml, "</expression>\n");
}

/* 
term = integerConstant | stringConstant | keywordConstant | varName | varName '[' expression ']' | 
       subroutineCall | '(' expression ')' | unaryOp term 
*/
void compileTerm(CompilationEngine *comp){
    fprintf(comp->fp_xml, "<term>\n");

    // integerConstant | stringConstant | keywordConstant
    if(tokenType(comp->tokenizer) == TK_INTCONST || tokenType(comp->tokenizer) == TK_STRCONST ||
        tokenType(comp->tokenizer) == TK_KEYWORD){

        writeToken(comp->fp_xml, comp->tokenizer);
        advance(comp->tokenizer);
    }
    // '(' expression ')'
    else if(isSymbolToken(comp, "(")){
        writeToken(comp->fp_xml, comp->tokenizer); // '('

        advance(comp->tokenizer);
        compileExpression(comp); // expression

        writeToken(comp->fp_xml, comp->tokenizer); // ')'
        advance(comp->tokenizer);
    }
    // unaryOp term
    else if(isSymbolToken(comp, "-") || isSymbolToken(comp, "~")){
        writeToken(comp->fp_xml, comp->tokenizer); // unaryOp

        advance(comp->tokenizer);
        compileTerm(comp); // term
    }
    // varName | varName '[' expression ']' | subroutineCall
    // いずれも最初に来るのはidentifier
    // その次に何が来るかで全体が確定
    // '['が来れば varName '[' expression ']'
    // '('または'.'が来れば subroutineCall
    // いずれでもなければ varName
    else{
        writeToken(comp->fp_xml, comp->tokenizer);
        advance(comp->tokenizer);

        // '[' expression ']'
        if(isSymbolToken(comp, "[")){
            writeToken(comp->fp_xml, comp->tokenizer); // '['

            advance(comp->tokenizer);
            compileExpression(comp); // expression

            writeToken(comp->fp_xml, comp->tokenizer); // ']'
            advance(comp->tokenizer);
        }
        // subroutineCall
        else if(isSymbolToken(comp, "(") || isSymbolToken(comp, ".")){
            if(isSymbolToken(comp, ".")){
                writeToken(comp->fp_xml, comp->tokenizer); // '.'
                advanceWrite(comp); // subroutineName
                advance(comp->tokenizer);
            }

            writeToken(comp->fp_xml, comp->tokenizer); // '('
                
            advance(comp->tokenizer); // expressionList
            compileExpressionList(comp);

            writeToken(comp->fp_xml, comp->tokenizer); // ')'
            advance(comp->tokenizer);
        }
        // varName(何もしない)
    }

    fprintf(comp->fp_xml, "</term>\n");
}

// expressionList = (expression (',' expression)* )?
// expressionListは'('と')'に挟まれた形でしか出現しないことから、
// 次のトークンが')'かどうか確認することでexpressionListが空かどうか判定できる
void compileExpressionList(CompilationEngine *comp){
    fprintf(comp->fp_xml, "<expressionList>\n");

    if(!isSymbolToken(comp, ")")){
        // expression
        compileExpression(comp);

        // (',' expression)*
        while(isSymbolToken(comp, ",")){
            writeToken(comp->fp_xml, comp->tokenizer); // ','
            
            advance(comp->tokenizer);
            compileExpression(comp);
        }
    }

    fprintf(comp->fp_xml, "</expressionList>\n");
}