
#include <map>
#include "common.h"

#ifndef ST_INCLUDED
#define ST_INCLUDED

class Variable;

using namespace std;

class SymbolTable
{
  map<string,SymbolInfo *> table;
  
public:
  void add(SymbolInfo *s);
  SymbolInfo *get(string name);
  void assign(Variable *var, SymbolInfo *value);
  void assign(string var, SymbolInfo *value);
};

#endif
