#include "lexer.h"

using namespace std;

class Parser:public Lexer
{

private:
   Token current_token;
   Token last_token;
   Expression *expr();
   Expression *term();
   Expression *factor();

   vector<Statement*> statement_list();
   Statement *get_statement();
   Statement *parse_print_statement();
   Statement *parse_printline_statement();

protected:
   Token get_next();

public:
   Expression *call_expression();
   Parser(string str):Lexer(str){};
   vector<Statement*> parse();
 
};
