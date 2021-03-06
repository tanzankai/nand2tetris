<h1>CompilationEngineの作成</h1>

<h2>終端記号</h2>

keyword  
symbol  
integerConstant  
stringConstant  
identifier  


<h2>生成規則</h2>
<h3>生成規則はEBNF記法による.</h3>

class = 'class' className '{' classVarDec* subroutineDec* '}'  
classVarDec = ('static' | 'field') type varName (',' varName)* ';'  
subroutineDec = ('constructor' | 'function' | 'method') ('void' | type) subroutineName '(' parameterList ')' subroutineBody  
parameterList = ((type varName) (',' type varName)*)?  
subroutineBody = '{' varDec* statements '}'  
varDec = 'var' type varName (',' varName)* ';'  
type = 'int' | 'char' | 'boolean' | className  
className = identifier  
subroutineName = identifier  
varName = identifier  

// subroutineBody以外は上記の生成規則で閉じている.  
// Class, ClassVarDec, Subroutine, ParameterList, VarDec  

statements = statement*  
statement = letStatement | ifStatement | whileStatement | doStatement | returnStatement  
letStatement = 'let' varName ('[' expression ']')? '=' expression ';'  
ifStatement = 'if' '(' expression ')' '{' statements '}' ('else' '{' statements '}')?  
whileStatement = 'while' '(' expression ')' '{' statements '}'  
doStatement = 'do' subroutineCall ';'  
returnStatement = 'return' expression? ';'  

// 文(statement)は5種類からなる.  
// Statements, Do, Let, While, Return, If  

expression = term (op term)*  
term = integerConstant | stringConstant | keywordConstant | varName | varName '[' expression ']' |   
       subroutineCall | '(' expression ')' | unaryOp term  
subroutineCall = subroutineName '(' expressionList ')' | (className | varName) '.' subroutineName '(' expressionList ')'  
expressionList = (expression (',' expression)* )?  
op = '+' | '-' | '*' | '/' | '&' | '|' | '<' | '>' | '='  
unaryOp = '-' | '~'  
keywordConstant = 'true' | 'false' | 'null' | 'this'  

// 式(experssion), 項(term), 0個以上の式(expressionList)  
// Expression, Term, ExpressionList  
