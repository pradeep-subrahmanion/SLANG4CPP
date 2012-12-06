#ifndef CODEGEN_INCLUDED
#define CODEGEN_INCLUDED

#include "common.h"
#include <cstdio>
#include <string>
#include <map>
#include <vector>
#include <iostream>

using namespace llvm;

namespace CodeGen
{
  extern LLVMContext & context;
  extern Module *module;
  extern IRBuilder<> builder;
  
  void	emit_top_level_code();
  void	emit_ret_stmt();
 
  Value	*       emit_global_string_for_double(double d);
  AllocaInst *  emit_stack_variable(SymbolInfo *info);
  
  void    emit_store_Instruction(AllocaInst *alloca, Value *val);
  Value * emit_load_Instruction(AllocaInst *alloca);
  
  void    emit_add_instruction(Value *v1, Value *v2);
  
  void    emit_print_stmt(Value *value,Type *type,const char *format);
  void    emit_print_stmt(Value *val,TypeInfo type);
  void    emit_printline_stmt(Value *val,TypeInfo type);

}

#endif
