
#include "common.h"
#include "ast_statement.h"
#include "codegen.h"

using namespace std;
using namespace CodeGen;

PrintStatement:: PrintStatement(Expression *_exp)
{
   exp = _exp;
}

bool PrintStatement:: execute()
{
   double d = exp->evaluate();
   std::cout << d ;
   return true;
}

Value* PrintStatement::codegen()
{
  llvm::Value *val = exp->codegen();
  CodeGen::generate_print_stmt(val);
  return NULL; // no value to return
}


PrintStatement:: ~PrintStatement()
{

}

PrintLineStatement:: PrintLineStatement(Expression *_exp)
{
   exp = _exp;
}

bool PrintLineStatement:: execute()
{
   double d = exp->evaluate();
   std::cout << d << "\n";
   return true;
}

Value* PrintLineStatement::codegen()
{
  llvm::Value *val = exp->codegen();
  CodeGen::generate_printline_stmt(val);
  return NULL; // no value to return
}


PrintLineStatement:: ~PrintLineStatement()
{

}
