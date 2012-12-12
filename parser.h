
class TmoduleBuilder;
class Tmodule;
class ProcedureBuilder;

#include "lexer.h"
#include "context.h"


using namespace std;

class Parser:public Lexer
{

private:

   TmoduleBuilder *module_builder;

   Expression *expr(ProcedureBuilder *ctx);
   Expression *term(ProcedureBuilder *ctx);
   Expression *factor(ProcedureBuilder *ctx);
   Expression *bexpr(ProcedureBuilder *ctx);
   Expression *lexpr(ProcedureBuilder *ctx);
   Expression *parse_callproc(ProcedureBuilder *ctx, Procedure *p);

   vector<Statement*> statement_list(ProcedureBuilder *ctx);
   Statement *get_statement(ProcedureBuilder *ctx);
   Statement *parse_print_statement(ProcedureBuilder *ctx);
   Statement *parse_printline_statement(ProcedureBuilder *ctx);
   Statement *parse_variabledcl_statement(ProcedureBuilder *ctx);
   Statement *parse_assignment_statement(ProcedureBuilder *ctx);
   Statement *parse_if_statement(ProcedureBuilder *ctx);
   Statement *parse_while_statement(ProcedureBuilder *ctx);
   Statement *parse_return_statement(ProcedureBuilder *ctx);
  
   ProcedureBuilder *parse_function();
   void formal_parameters(ProcedureBuilder *ctx);
   Tmodule * parse_functions();

protected:
   Token get_next();

public:
   Expression *call_expression(ProcedureBuilder *ctx);
   Parser(string str);
   vector<Statement*> parse(ProcedureBuilder *ctx);
   Tmodule *do_parse();
 
};
