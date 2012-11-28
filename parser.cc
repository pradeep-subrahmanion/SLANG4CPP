#include "parser.h"

Expression *Parser::call_expression(Compilation_Context *ctx)
{
   current_token = get_token();
   return expr(ctx);
}

Expression *Parser::expr(Compilation_Context *ctx)
{
   Token l_token;
   Expression *result = term(ctx);

   while(current_token == TOKEN_PLUS || current_token == TOKEN_MINUS) {
      l_token = current_token;
      current_token = get_token();
      Expression *e1 = expr(ctx);
      if(l_token == TOKEN_PLUS) {
         result = new BinaryPlus(result , e1);
      }
      else {
         result = new BinaryMinus(result , e1);
      }

   }

   return result;
}

Expression *Parser::term(Compilation_Context *ctx)
{
   Token l_token;
   Expression *result = factor(ctx);

   while(current_token == TOKEN_MUL || current_token == TOKEN_DIV) {
      l_token = current_token;
      current_token = get_token();
      Expression *e1 = term(ctx);
      if(l_token == TOKEN_MUL) {
         result = new Mult(result , e1);
      }
      else {
         result = new Div(result , e1);
      }

   }

   return result;
}

Expression *Parser::factor(Compilation_Context *ctx)
{
   Token l_token;
   Expression *result = NULL;
   
   if(current_token == TOKEN_NUMERIC) {
      result = new NumericConstant(get_number());
      current_token = get_token();
   }
   else if(current_token == TOKEN_STRING) {
      result = new StringLiteral(last_string);
      current_token = get_token();
   }
   else if (current_token == TOKEN_BOOL_TRUE ||
            current_token == TOKEN_BOOL_FALSE)
   {
      result = new BooleanConstant(
      current_token == TOKEN_BOOL_TRUE ? true : false);
      current_token = get_token();
   }

   else if(current_token == TOKEN_OPAREN) {
      current_token = get_token();
      result = expr(ctx);
      if(current_token != TOKEN_CPAREN) {
         cout << "Error : missing closing parenthesis" << "\n";
      }
      current_token = get_token();
   }
   else if(current_token == TOKEN_PLUS || current_token == TOKEN_MINUS) {
      l_token = current_token;
      current_token = get_token();
      result = factor(ctx); 

      if(l_token == TOKEN_PLUS) {
         result = new UnaryPlus(result);
      }
      else {
         result = new UnaryMinus(result);
      }

   }
   else if(current_token == TOKEN_UNQUOTED_STRING) {
      SymbolInfo *info = ctx->get_symbol(last_string);
      if(info == NULL) {
         std::cout << "Undefined Symbol";
         return NULL;
      }

      get_next();
      result = new Variable(info);
   }
   else {
      std::cout << "Illegal token - " << current_token<<"\n";
   }

   return result;
}

Token Parser::get_next()
{
   last_token = current_token;
   current_token = get_token();
   return current_token;
}

vector<Statement*> Parser::parse(Compilation_Context *ctx)
{
   get_next();
   return statement_list(ctx);

}
vector<Statement*> Parser::statement_list(Compilation_Context *ctx)
{
   vector<Statement*> temp;

   while(current_token != TOKEN_NULL) {
      Statement *st = get_statement(ctx);
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
   case TOKEN_VAR_NUMBER:
   case TOKEN_VAR_BOOL:
        st = parse_variabledcl_statement(ctx);
        get_next();
        break;
   case TOKEN_PRINT:
        st = parse_print_statement(ctx);
        get_next();
        break;
   case TOKEN_PRINTLN:
         st = parse_printline_statement(ctx);
         get_next();
         break;
   case TOKEN_UNQUOTED_STRING:
         st = parse_assignment_statement(ctx);
         get_next();
         break;
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
   Expression *e = expr(ctx);

   if(current_token != TOKEN_SEMI) {
      std::cout << "\n; is expected\n";
   }   
   PrintStatement *st =  new PrintStatement(e);
   return st;
}
Statement *Parser::parse_printline_statement(Compilation_Context *ctx)
{
   get_next();
   Expression *e = expr(ctx);
   if(current_token != TOKEN_SEMI) {
      std::cout << "\n; is expected\n";
   }
  
   PrintLineStatement *st = new PrintLineStatement(e);
   return st;
}
Statement *Parser::parse_variabledcl_statement(Compilation_Context *ctx)
{
   Token token = current_token;
   get_next();

   if(current_token == TOKEN_UNQUOTED_STRING) {
      SymbolInfo *info = new SymbolInfo();
      info->symbol_name = last_string;
      if(token == TOKEN_VAR_BOOL) {
         info->type = TYPE_BOOL;
      }
      else if(token == TOKEN_VAR_NUMBER) {
         info->type = TYPE_NUMERIC;
      }
      else if(token == TOKEN_VAR_STRING) {
         info->type = TYPE_STRING;
      }
      get_next();

      if(current_token ==TOKEN_SEMI ) {
         ctx->add_symbol(info);
         return new VariableDeclStatement(info);
      }
      else {
         std:cout << "Expected ; at the end of statement";
      }

   }
   else {
      std::cout << "invalid variable declaration";
   }
   return NULL;
}
Statement *Parser::parse_assignment_statement(Compilation_Context *ctx)
{
   string var = last_string;

   SymbolInfo *info = ctx->get_symbol(var);

   if(info == NULL) {
      std::cout << "Error - variable not found";
      return NULL;
   }

   get_next();

   if(current_token != TOKEN_ASSIGN) {
      std::cout << " = expected";
      return NULL;
   }

   get_next();
     // std::cout << "index" << index;
   Expression *exp = expr(ctx);
   if(exp->typecheck(ctx) != info->type) {
      std::cout << "Type mismatch";
      return NULL;
   }

   if(current_token != TOKEN_SEMI) {
      std::cout << "Missing ; symbol";
      return NULL;
   }

   return new AssignmentStatement(info,exp);
}

