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
 
  Value	*      emit_global_string_for_double(double d);
  AllocaInst * emit_stack_variable(SymbolInfo *info);
  
  void    emit_store_Instruction(AllocaInst *alloca, Value *val);
  Value * emit_load_Instruction(AllocaInst *alloca);
  
  Value *    emit_add_instruction(Value *v1, Value *v2);
  Value *    emit_sub_instruction(Value *v1, Value *v2);
  Value *    emit_mul_instruction(Value *v1, Value *v2);
  Value *    emit_div_instruction(Value *v1, Value *v2);
  Value *    emit_unary_minus_instruction(Value *v);
  
  Value *    emit_and_instruction(Value *v1, Value *v2);
  Value *    emit_or_instruction(Value *v1, Value *v2);
  Value *    emit_not_instruction(Value *v);
  
  Value *    emit_lessthan_instruction(Value *v1, Value *v2);
  Value *    emit_greaterthan_instruction(Value *v1, Value *v2);
  Value *    emit_lessequal_instruction(Value *v1, Value *v2);
  Value *    emit_greaterequal_instruction(Value *v1, Value *v2);
  Value *    emit_equalequal_instruction(Value *v1, Value *v2);
  Value *    emit_notequal_instruction(Value *v1, Value *v2);


  Value *         emit_condition(Value *val);
  BasicBlock *    emit_block_in_main(const char *name);
  void  emit_conditional_branch(Value *condition_val , BasicBlock *then_block, BasicBlock *else_block);

  void move_to_block(BasicBlock *block);
  BasicBlock *get_insert_block();
  void create_branch(BasicBlock *block);
  void add_blockval_in_phi(PHINode *phi, BasicBlock *block, Value *val);

  PHINode * emit_phi_node();
  

  
  void    emit_print_stmt(Value *value,Type *type,const char *format);
  void    emit_print_stmt(Value *val,TypeInfo type);
  void    emit_printline_stmt(Value *val,TypeInfo type);

}

#endif
