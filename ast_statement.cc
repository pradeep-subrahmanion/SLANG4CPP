
#include "common.h"
#include "ast_statement.h"
#include "codegen.h"

using namespace std;
using namespace CodeGen;

// Print Statement

PrintStatement:: PrintStatement(Expression *_exp)
{
  exp = _exp;
}

SymbolInfo *PrintStatement:: execute(Runtime_Context *ctx)
{
  SymbolInfo *info = exp->evaluate(ctx);
  
  if(info->type == TYPE_STRING) {
    std::cout << info->string_val;
  }
  else if(info->type == TYPE_NUMERIC) {
    std::cout << info->double_val;
  }
  else if(info->type == TYPE_BOOL) {
    std::cout << info->bool_val;
  }
  
  return NULL;
}

PrintStatement:: ~PrintStatement()
{
  
}

//PrintLine Statement

PrintLineStatement:: PrintLineStatement(Expression *_exp)
{
  exp = _exp;
}

SymbolInfo * PrintLineStatement:: execute(Runtime_Context *ctx)
{
  SymbolInfo *info = exp->evaluate(ctx);
  if(info->type == TYPE_STRING) {
    std::cout << info->string_val << "\n";
  }
  else if(info->type == TYPE_NUMERIC) {
    std::cout << info->double_val << "\n";
  }
  else if(info->type == TYPE_BOOL) {
    std::cout << info->bool_val << "\n";
  }
  
  return NULL;
}

PrintLineStatement:: ~PrintLineStatement()
{
  
}

// Variable Declaration

VariableDeclStatement::VariableDeclStatement(SymbolInfo *_info)
{
  info = _info;
}
SymbolInfo *VariableDeclStatement::execute(Runtime_Context *ctx)
{
  ctx->add_symbol(info);
  var = new Variable(info);
  return NULL;
}

// Assignment Statement

AssignmentStatement::AssignmentStatement(Variable *_var, Expression *_exp)
{
  var = _var;
  exp = _exp;
}
AssignmentStatement::AssignmentStatement(SymbolInfo *info, Expression *_exp)
{
  var = new Variable(info);
  exp = _exp;
}

SymbolInfo *AssignmentStatement::execute(Runtime_Context *ctx)
{
  SymbolInfo *info = exp->evaluate(ctx);
  ctx->assign_symbol(var,info);
  return NULL;
}

//If Statement

IfStatement::IfStatement(Expression *_exp, vector<Statement *> v1,vector<Statement *> v2 )
{
  condition = _exp;
  if_statements = v1;
  else_statements = v2;
}

SymbolInfo *IfStatement::execute(Runtime_Context *ctx)
{
  SymbolInfo *info  = condition->evaluate(ctx);

  if(info->type == TYPE_BOOL) {

      vector<Statement *> *statements = &if_statements;
      if(info->bool_val == false) {
         statements= &else_statements;
      }

      for(int i=0;i<(*statements).size();++i) {
        Statement *st = (*statements).at(i);
        st->execute(ctx);
      }

  }

  return NULL;
}

// While Statement

WhileStatement::WhileStatement(Expression *_exp, vector<Statement *> v)
{
  condition = _exp;
  statements = v;
}
SymbolInfo *WhileStatement::execute(Runtime_Context *ctx)
{

  SymbolInfo *info  = condition->evaluate(ctx);

  while(info->type == TYPE_BOOL && info->bool_val == true) {

      for(int i=0;i<statements.size();++i) {
        Statement *st = statements.at(i);
        st->execute(ctx);
      }

    info = condition->evaluate(ctx);
  }

  return NULL;
}


