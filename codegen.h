#ifndef CODEGEN_INCLUDED
#define CODEGEN_INCLUDED

#include "common.h"
#include <cstdio>
#include <string>
#include <map>
#include <vector>
#include <iostream>

using namespace llvm;

namespace CodeGen {
extern LLVMContext & context;
extern Module *module;
extern IRBuilder<> builder;

Type *llvm_type_from_symboltype(TypeInfo type);

void emit_top_level_code();
Function * emit_function_block(const char *name, ArrayRef<Type*> argsRef, Type *type);
void emit_ret_stmt();

Value *emit_global_string(const char *buffer);
Value * emit_global_string_for_double(double d);
AllocaInst * emit_stack_variable(SymbolInfo *info);

// load / store stack variable.

void emit_store_Instruction(AllocaInst *alloca, Value *val);
Value * emit_load_Instruction(AllocaInst *alloca);

// codegen for arithmetic operators

Value * emit_add_instruction(Value *v1, Value *v2);
Value * emit_sub_instruction(Value *v1, Value *v2);
Value * emit_mul_instruction(Value *v1, Value *v2);
Value * emit_div_instruction(Value *v1, Value *v2);
Value * emit_unary_minus_instruction(Value *v);

// codegen for logical operators

Value * emit_and_instruction(Value *v1, Value *v2);
Value * emit_or_instruction(Value *v1, Value *v2);
Value * emit_not_instruction(Value *v);

// codegen for relational operators

Value * emit_lessthan_instruction(Value *v1, Value *v2);
Value * emit_greaterthan_instruction(Value *v1, Value *v2);
Value * emit_lessequal_instruction(Value *v1, Value *v2);
Value * emit_greaterequal_instruction(Value *v1, Value *v2);
Value * emit_equalequal_instruction(Value *v1, Value *v2);
Value * emit_notequal_instruction(Value *v1, Value *v2);
Value * emit_int_equal_instruction(Value *v1, Value *v2);
Value * emit_int_notequal_instruction(Value *v1, Value *v2);

// codegen for print statements

void emit_print_stmt(Value *value, Type *type, const char *format);
void emit_print_stmt(Value *val, TypeInfo type);
void emit_printline_stmt(Value *val, TypeInfo type);

Value * emit_strcmp_stmt(Value *val1, Value *val2);

}

#endif

