#include "parser.h"

Expression *Parser::call_expression(Compilation_Context *ctx)
{
  current_token = get_token();
  return expr(ctx);
}

Expression* Parser::bexpr(Compilation_Context *ctx)
{
  Token l_token;
  Expression *retValue = lexpr(ctx);
  while (current_token == TOKEN_AND || current_token == TOKEN_OR)
  {
    l_token = current_token;
    current_token = get_next();
    Expression *e2 = lexpr(ctx);
    retValue = new LogicalExpression(retValue, e2,l_token);
  }

 return retValue;

}
Expression* Parser::lexpr(Compilation_Context *ctx)
{
 Token l_token;
 Expression *ret = expr(ctx);

 while (current_token == TOKEN_GREATER_THAN ||
        current_token == TOKEN_LESS_THAN ||
        current_token == TOKEN_GREATER_EQUAL ||
        current_token == TOKEN_LESS_EQUAL ||
        current_token == TOKEN_NEQUAL ||
        current_token == TOKEN_EQUAL)
        {
          l_token = current_token;
          current_token = get_next();
          Expression *e2 = expr(ctx);
          RelationalOperator relop = get_relation_operator(l_token);
          ret = new RelationalExpression(ret, e2,relop);

        }
 return ret;

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
  else if(current_token == TOKEN_NOT) {
    cout << "NOt....";
    l_token = current_token;
    current_token = get_token();
    result = factor(ctx);
    result = new LogicalNot(result);
  }
  
  else if(current_token == TOKEN_OPAREN) {
    current_token = get_token();
    result = bexpr(ctx);
    if(current_token != TOKEN_CPAREN) {
      exit_with_message("Error : missing closing parenthesis");
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
      exit_with_message("Undefined Symbol");
      return NULL;
    }
    
    get_next();
    result = new Variable(info);
  }
  else {
    exit_with_message("Illegal token - " );
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
  
  while(current_token != TOKEN_NULL && 
        current_token != TOKEN_ENDIF && 
        current_token != TOKEN_ENDWHILE && 
        current_token != TOKEN_ELSE ) {
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
    case TOKEN_IF:
      st = parse_if_statement(ctx);
      get_next();
      break;
    case TOKEN_WHILE:
      st = parse_while_statement(ctx);
      get_next();
      break;
  
    case TOKEN_UNQUOTED_STRING:
      st = parse_assignment_statement(ctx);
      get_next();
      break;
    default:
      exit_with_message("Exception in statement");
      get_next();
      break;
      
  }
  return st;
}
Statement *Parser::parse_print_statement(Compilation_Context *ctx)
{
  get_next();
  Expression *e = bexpr(ctx);
  e->typecheck(ctx);
 
  if(current_token != TOKEN_SEMI) {
    exit_with_message("\n; is expected\n");
  }
  PrintStatement *st =  new PrintStatement(e);
  return st;
}
Statement *Parser::parse_printline_statement(Compilation_Context *ctx)
{
  get_next();
  Expression *e = bexpr(ctx);
  e->typecheck(ctx);

  if(current_token != TOKEN_SEMI) {
    exit_with_message("\n; is expected\n");
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
      exit_with_message("Expected ; at the end of statement");
    }
    
  }
  else {
    exit_with_message("invalid variable declaration");
  }
  return NULL;
}
Statement *Parser::parse_assignment_statement(Compilation_Context *ctx)
{
  string var = last_string;
  
  SymbolInfo *info = ctx->get_symbol(var);
  
  if(info == NULL) {
    exit_with_message("Error - variable not found");
    return NULL;
  }
  
  get_next();
  
  if(current_token != TOKEN_ASSIGN) {
    exit_with_message(" = expected");
    return NULL;
  }
  
  get_next();
  Expression *exp = bexpr(ctx);
  if(exp->typecheck(ctx) != info->type) {
    exit_with_message("Type Mismatch");
  }
  
  if(current_token != TOKEN_SEMI) {
    exit_with_message("Missing ; symbol");
    return NULL;
  }

  if(info->type == TYPE_STRING) {
    /// compute string expression like '+' in compile time . store the result in string_val of info

    string str = exp->evaluate_string(NULL);
    info->string_val = str;
  }
  
  return new AssignmentStatement(info,exp);
}

Statement *Parser::parse_if_statement(Compilation_Context *ctx) {
  get_next();
  vector<Statement *> true_part;
  vector<Statement *> false_part;

  Expression *exp = bexpr(ctx);

  if(exp->typecheck(ctx) != TYPE_BOOL) {
    exit_with_message("Expected boolean expression");
  }

  if(current_token != TOKEN_THEN) {
    exit_with_message("THEN expected");
  }

  get_next();
  true_part = statement_list(ctx);

  if(current_token == TOKEN_ENDIF) {
    return new IfStatement(exp,true_part,false_part);
  }

  if(current_token != TOKEN_ELSE) {
    exit_with_message("ELSE expected");
  }

  get_next();
  false_part = statement_list(ctx);

  if(current_token == TOKEN_ENDIF) {
    return new IfStatement(exp,true_part,false_part);
  }

  exit_with_message("ENDIF expected");
}

Statement *Parser::parse_while_statement(Compilation_Context *ctx) {
  get_next();
  vector<Statement *> stmts;
  Expression *exp = bexpr(ctx);

  if(exp->typecheck(ctx) != TYPE_BOOL) {
    exit_with_message("Expected boolean expression");
  }

  stmts = statement_list(ctx);

  if(current_token != TOKEN_ENDWHILE) {
    exit_with_message("WEND expected");
  }
  return new WhileStatement(exp,stmts);
}

