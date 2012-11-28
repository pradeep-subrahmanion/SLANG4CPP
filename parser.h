#include "lexer.h"
#include "context.h"
using namespace std;

class Parser:public Lexer
{

private:
   Expression *expr(Compilation_Context *ctx);
   Expression *term(Compilation_Context *ctx);
   Expression *factor(Compilation_Context *ctx);

   vector<Statement*> statement_list(Compilation_Context *ctx);
   Statement *get_statement(Compilation_Context *ctx);
   Statement *parse_print_statement(Compilation_Context *ctx);
   Statement *parse_printline_statement(Compilation_Context *ctx);
   Statement *parse_variabledcl_statement(Compilation_Context *ctx);
   Statement *parse_assignment_statement(Compilation_Context *ctx);

protected:
   Token get_next();

public:
   Expression *call_expression(Compilation_Context *ctx);
   Parser(string str):Lexer(str){};
   vector<Statement*> parse(Compilation_Context *ctx);
 
};
