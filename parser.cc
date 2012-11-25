#include "parser.h"

Expression *Parser::call_expression()
{
   current_token = get_token();
   return expr();
}

Expression *Parser::expr()
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

Expression *Parser::term()
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

Expression *Parser::factor()
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

Token Parser::get_next()
{
   last_token = current_token;
   current_token = get_token();
   return current_token;
}

vector<Statement*> Parser::parse()
{
   get_next();
   return statement_list();

}
vector<Statement*> Parser::statement_list()
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

Statement *Parser::get_statement()
{
  Statement *st =  NULL;
  switch(current_token) {
   case TOKEN_PRINT:
        st = parse_print_statement();
        get_next();
        break;
   case TOKEN_PRINTLN:
         st = parse_printline_statement();
         get_next();
         break;
   default:
         std::cout << "Exception in statement" << "\n";
         get_next();
         break;

  }
   return st;
}
Statement *Parser::parse_print_statement()
{
   get_next();
   Expression *e = expr();

   if(current_token != TOKEN_SEMI) {
      std::cout << "\n; is expected\n";
   }   
  
   PrintStatement *st =  new PrintStatement(e);
   return st;
}
Statement *Parser::parse_printline_statement()
{
   get_next();
   Expression *e = expr();
   if(current_token != TOKEN_SEMI) {
      std::cout << "\n; is expected\n";
   }
  
   PrintLineStatement *st = new PrintLineStatement(e);
   return st;
}

