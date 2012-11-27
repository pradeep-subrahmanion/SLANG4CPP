#include "parser.h"

Expression *Parser::call_expression(Compilation_Context *ctx)
{
   current_token = get_token();
   return expr();
}

Expression *Parser::expr(Compilation_Context *ctx)
{
   Token l_token;
   Expression *result = term();

   while(current_token == TOKEN_PLUS || current_token == TOKEN_MINUS) {
      l_token = current_token;
      current_token = get_token();
      Expression *e1 = expr();
      result = new BinaryExpression(result, e1, l_token == TOKEN_PLUS? OPERATOR_PLUS: OPERATOR_MINUS );      
   }

   return result;
}

Expression *Parser::term(Compilation_Context *ctx)
{
   Token l_token;
   Expression *result = factor();

   while(current_token == TOKEN_MUL || current_token == TOKEN_DIV) {
      l_token = current_token;
      current_token = get_token();
      Expression *e1 = term();
      result = new BinaryExpression(result, e1, l_token == TOKEN_MUL? OPERATOR_MUL: OPERATOR_DIV);      
   }

   return result;
}

Expression *Parser::factor(Compilation_Context *ctx)
{
   Token l_token;
   Expression *result = NULL;
   
   if(current_token == TOKEN_DOUBLE) {
      result = new NumericConstant(get_number());
      current_token = get_token();
   }
   else if(current_token == TOKEN_OPAREN) {
      current_token = get_token();
      result = expr();
      if(current_token != TOKEN_CPAREN) {
         cout << "Error : missing closing parenthesis" << "\n";
      }
      current_token = get_token();
   }
   else if(current_token == TOKEN_PLUS || current_token == TOKEN_MINUS) {
      l_token = current_token;
      current_token = get_token();
      result = factor(); 
      result = new UnaryExpression(result, l_token == TOKEN_PLUS? OPERATOR_PLUS: OPERATOR_MINUS);      
   }
   else {
      std::cout << "Error : Illegal token" << "\n";
   }

   return result;
}

Token Parser::get_next(Compilation_Context *ctx)
{
   last_token = current_token;
   current_token = get_token();
   return current_token;
}

vector<Statement*> Parser::parse(Compilation_Context *ctx)
{
   get_next();
   return statement_list();

}
vector<Statement*> Parser::statement_list(Compilation_Context *ctx)
{
   vector<Statement*> temp;

   while(current_token != TOKEN_NULL) {
      Statement *st = get_statement();
      if(st != NULL) {
         temp.insert(temp.end(),st);
      }
   
   }

   return temp;
}

Statement *Parser::get_statement(Compilation_Context *ctx)
{
  Statement *st =  NULL;
  switch(current_token) {
   case TOKEN_VAR_STRING:
   case TOKEN_VAR_NUMERIC:
   case TOKEN_VAR_BOOL:
        st = parse_variabledcl_statement();
        get_next();
        return st;
   case TOKEN_PRINT:
        st = parse_print_statement();
        get_next();
        break;
   case TOKEN_PRINTLN:
         st = parse_printline_statement();
         get_next();
         break;
   case TOKEN_UNQUOTED_STRING:
         st = parse_assignment_statement();
         get_next();
         return st;
   default:
         std::cout << "Exception in statement" << "\n";
         get_next();
         break;

  }
   return st;
}
Statement *Parser::parse_print_statement(Compilation_Context *ctx)
{
   get_next();
   Expression *e = expr();

   if(current_token != TOKEN_SEMI) {
      std::cout << "\n; is expected\n";
   }   
  
   PrintStatement *st =  new PrintStatement(e);
   return st;
}
Statement *Parser::parse_printline_statement(Compilation_Context *ctx)
{
   get_next();
   Expression *e = expr();
   if(current_token != TOKEN_SEMI) {
      std::cout << "\n; is expected\n";
   }
  
   PrintLineStatement *st = new PrintLineStatement(e);
   return st;
}
Statement *parse_variabledcl_statement(Compilation_Context *ctx)
{
   Token token = current_token;
   get_next();

   if(current_token == TOKEN_UNQUOTED_STRING) {
      SymbolInfo *info = new SymbolInfo();
      info->symbol_name = last_str;
   }

}
Statement *parse_assignment_statement(Compilation_Context *ctx)
{

}

