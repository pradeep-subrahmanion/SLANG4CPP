#include "ast.h"
#include "context.h"

class Statement
{
public:
  virtual SymbolInfo *execute(Runtime_Context *ctx) =0 ;
	Value* codegen(){};
  
};

class PrintStatement: public Statement
{
  
  Expression *exp;
  
public:
  PrintStatement(Expression *_exp);
  ~PrintStatement();
  SymbolInfo *execute(Runtime_Context *ctx);
  
};

class PrintLineStatement: public Statement
{
  
  Expression *exp;
  
public:
  PrintLineStatement(Expression *_exp);
  ~PrintLineStatement();
  SymbolInfo * execute(Runtime_Context *ctx);
};

class VariableDeclStatement: public Statement
{
  SymbolInfo *info;
  Variable *var;
  
public:
  VariableDeclStatement(SymbolInfo *_info);
  SymbolInfo *execute(Runtime_Context *ctx);
  
};

class AssignmentStatement : public Statement
{
  Variable *var;
  Expression *exp;
  
public:
  AssignmentStatement(SymbolInfo *info, Expression *_exp);
  AssignmentStatement(Variable *_var, Expression *_exp);
  SymbolInfo *execute(Runtime_Context *ctx);
  
};



