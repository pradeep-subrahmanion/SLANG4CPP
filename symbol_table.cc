#include "symbol_table.h"
#include "ast.h"

typedef pair <string,SymbolInfo *> st_pair;

void SymbolTable::add(SymbolInfo *s)
{
   table.insert(std::make_pair(s->symbol_name,s));
}

SymbolInfo * SymbolTable::get(string name)
{
   map<string,SymbolInfo *>::iterator it;
   it = table.find(name);
   return it->second;
}
void SymbolTable::assign(Variable *var, SymbolInfo *value)
{
   string name = var->get_name();
   map<string,SymbolInfo *>::iterator it;
   it = table.find(name);
   it->second = value;
   
}
void SymbolTable::assign(string var, SymbolInfo *value)
{
   map<string,SymbolInfo *>::iterator it;
   it = table.find(var);
   it->second = value;
}
