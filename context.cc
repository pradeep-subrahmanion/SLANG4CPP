#include "context.h"


// Runtime Context

Runtime_Context::Runtime_Context()
{
  st= new SymbolInfoTable();
};

SymbolInfoTable *Runtime_Context::get_symboltable()
{
	return st;
}

void Runtime_Context::set_symboltable(SymbolInfoTable *_st)
{
	st= _st;
}
void Runtime_Context::assign_symbol(Variable *var, SymbolInfo *value)
{
  st->assign(var,value);
}

void Runtime_Context::add_symbol(SymbolInfo *info)
{
  st->add(info);
}

SymbolInfo * Runtime_Context::get_symbol(string name)
{
  return st->get(name);
}

// Compilation Context

Compilation_Context::Compilation_Context()
{
  st = new SymbolInfoTable();
}
SymbolInfoTable * Compilation_Context::get_symboltable()
{
	return st;
}

void Compilation_Context::assign_symbol(Variable *var, SymbolInfo *value)
{
  st->assign(var,value);
}

void Compilation_Context::set_symboltable(SymbolInfoTable  *_st)
{
	st= _st;
}

void Compilation_Context::add_symbol(SymbolInfo *info)
{
  st->add(info);
}

SymbolInfo * Compilation_Context::get_symbol(string name)
{
  return st->get(name);
}

//Execution context

Execution_Context::Execution_Context()
{
  st= new AllocaInstTable();
}
AllocaInstTable *Execution_Context::get_symboltable()
{
  return st;
}
void Execution_Context::set_symboltable(AllocaInstTable *_st)
{
  st = _st;
}
void Execution_Context::add_symbol(string key , AllocaInst *info)
{
  st->add(key,info);
}
void Execution_Context::assign_symbol(Variable *var, AllocaInst *value)
{
  st->assign(var,value);
}
AllocaInst *Execution_Context::get_symbol(string name)
{
  return st->get(name);
}

