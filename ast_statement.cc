
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


