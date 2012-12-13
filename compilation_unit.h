#ifndef CUNIT_INCLUDED
#define CUNIT_INCLUDED

class Statement;

#include "common.h"
#include "context.h"
#include "symbol_table.h"

class CompilationUnit
{
public:
  virtual SymbolInfo * execute(Runtime_Context *ctx, vector<SymbolInfo *>_actuals) = 0;
};

class Proc
{
public:
  virtual SymbolInfo * execute(Runtime_Context *ctx, vector<SymbolInfo *>_actuals) = 0;
};

class Procedure:Proc
{
public:

  string name;
  vector<Statement *> statements;
  vector<SymbolInfo *> formals;
  SymbolInfoTable * locals;
  SymbolInfo *return_val;
  TypeInfo type;

  Procedure(string _name, vector<SymbolInfo *> _formals, vector<Statement *>stats, SymbolInfoTable *_locals,
    TypeInfo _type);
  SymbolInfo * execute(Runtime_Context *ctx, vector<SymbolInfo *>formals);
  TypeInfo typecheck(Compilation_Context *ctx);
};

class Tmodule:CompilationUnit
{
  vector<Procedure *> procs;

public:
  Tmodule(vector<Procedure *>_procs);
  Procedure * find_procedure(string name);
  SymbolInfo * execute(Runtime_Context *ctx, vector<SymbolInfo *>_actuals);
  TypeInfo typecheck(Compilation_Context *ctx);
};



#endif
