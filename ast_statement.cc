
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

Value* PrintStatement::codegen(Execution_Context *ctx)
{
  
  Value *val = exp->codegen(ctx);
  TypeInfo type= exp->get_type();
  emit_print_stmt(val,type);
  return NULL;

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

Value* PrintLineStatement::codegen(Execution_Context *ctx)
{
  Value *val = exp->codegen(ctx);
  TypeInfo type= exp->get_type();
  emit_printline_stmt(val,type);
  return NULL;
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

Value* VariableDeclStatement::codegen(Execution_Context *ctx)
{
  string name = info->symbol_name;
  AllocaInst *alcInst = emit_stack_variable(info);
  ctx->add_symbol(info->symbol_name,alcInst);

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

Value* AssignmentStatement::codegen(Execution_Context *ctx)
{
  AllocaInst *alcInst = ctx->get_symbol(var->get_name());
  emit_store_Instruction(alcInst,exp->codegen(ctx));
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

Value* IfStatement::codegen(Execution_Context *ctx)
{
  Value *CondV = condition->codegen(ctx);

  Value *ThenV = ConstantFP::get(getGlobalContext(), APFloat(1.0));
  Value *ElseV = ConstantFP::get(getGlobalContext(), APFloat(2.0));

  if (CondV == NULL) {cout <<"null";return 0;};

  // Convert condition to a bool by comparing equal to 0.0.
  CondV = builder.CreateFCmpONE(CondV,
                              ConstantFP::get(getGlobalContext(), APFloat(0.0)),
                                "ifcond");
#if 0
  Function *TheFunction = builder.GetInsertBlock()->getParent();

  // Create blocks for the then and else cases.  Insert the 'then' block at the
  // end of the function.
  BasicBlock *ThenBB =  BasicBlock::Create(getGlobalContext(), "then", TheFunction);
  BasicBlock *ElseBB =  BasicBlock::Create(getGlobalContext(), "else");
  BasicBlock *MergeBB = BasicBlock::Create(getGlobalContext(), "ifcont");

  builder.CreateCondBr(CondV, ThenBB, ElseBB);

  // Emit then value.
  builder.SetInsertPoint(ThenBB);

  for(int i=0;i<if_statements.size();++i) {
    
    Statement *st = if_statements.at(i);
    st->codegen(ctx);
  }

  builder.CreateBr(MergeBB);

  // Codegen of 'Then' can change the current block, update ThenBB for the PHI.
  ThenBB = builder.GetInsertBlock();

  // Emit else block.
  TheFunction->getBasicBlockList().push_back(ElseBB);
  builder.SetInsertPoint(ElseBB);

  for(int i=0;i<else_statements.size();++i) {
    
    Statement *st = else_statements.at(i);
    st->codegen(ctx);
  }

  builder.CreateBr(MergeBB);

  // Codegen of 'Else' can change the current block, update ElseBB for the PHI.
  ElseBB = builder.GetInsertBlock();

  // Emit merge block.
  TheFunction->getBasicBlockList().push_back(MergeBB);
  builder.SetInsertPoint(MergeBB);
  PHINode *PN = builder.CreatePHI(Type::getDoubleTy(getGlobalContext()), 2,
                                  "iftmp");

  PN->addIncoming(ThenV, ThenBB);
  PN->addIncoming(ElseV, ElseBB);
  return PN;
#endif
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

Value* WhileStatement::codegen(Execution_Context *ctx)
{
  return NULL;
}

