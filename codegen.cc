#include "codegen.h"

namespace CodeGen
{

//
// setup context , create new module . 
//

  llvm::LLVMContext & context = llvm::getGlobalContext();
  llvm::Module *module = new llvm::Module("main_module", context);
  llvm::IRBuilder<> builder(context);
  llvm::Function *mainFunc;

//
// Create main function , basic block for main . 
//
  void emit_top_level_code()
  {
    
    llvm::FunctionType *funcType = llvm::FunctionType::get(builder.getVoidTy(), false);
    mainFunc = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, "main", module);
    llvm::BasicBlock *entry = llvm::BasicBlock::Create(context, "entrypoint", mainFunc);
    builder.SetInsertPoint(entry);
    
  }
  
  void emit_ret_stmt()
  {
    builder.CreateRetVoid();
  }
  
  Value	*emit_global_string_for_double(double d)
  {
    char buffer[32];
    snprintf(buffer, 32, "%g",d);
    Value *val = builder.CreateGlobalStringPtr(buffer);
    return val;
  }

  Value	*emit_global_string(const char *buffer)
  {
    Value *val = builder.CreateGlobalStringPtr(buffer);
    return val;
  }

//
// Method to create stack variable from SymbolInfo
//

  AllocaInst * emit_stack_variable(SymbolInfo *info) {
    
    AllocaInst *alc = NULL;
    
    if(info->type == TYPE_STRING) {

      alc = builder.CreateAlloca(Type::getInt8PtrTy(getGlobalContext()), 0,
                               info->symbol_name.c_str());
    }
    else if(info->type == TYPE_NUMERIC) {
      
      alc = builder.CreateAlloca(Type::getDoubleTy(getGlobalContext()), 0,

                               info->symbol_name.c_str());
    }
    else if(info->type == TYPE_BOOL) {
      
      alc = builder.CreateAlloca(Type::getInt1Ty(getGlobalContext()), 0,
                              info->symbol_name.c_str());
    }
    
    return alc;
  }

//
// load / store stack variable.
//
  void emit_store_Instruction(AllocaInst *alloca, Value *val) {
    builder.CreateStore(val, alloca);
  }

  Value *emit_load_Instruction(AllocaInst *alloca) {
    return builder.CreateLoad(alloca);
  }

//
// codegen for arithmetic operators
//

  Value * emit_add_instruction(Value *v1, Value *v2) {
    return builder.CreateFAdd(v1, v2, "var");
  }
  
  Value *  emit_sub_instruction(Value *v1, Value *v2) {
    return builder.CreateFSub(v1, v2, "var");
  }
  
  Value *  emit_mul_instruction(Value *v1, Value *v2) {
    return builder.CreateFMul(v1, v2, "var");
  }
  
  Value *  emit_div_instruction(Value *v1, Value *v2) {
    return builder.CreateFDiv(v1, v2,"var");
  }
  
  Value *  emit_unary_minus_instruction(Value *v)  {
    return builder.CreateFNeg(v,"var");
  }
  
//
// codegen for logical operators
//

  Value *  emit_and_instruction(Value *v1, Value *v2) {
    return builder.CreateAnd(v1,v2,"var");
  }
  
  Value *  emit_or_instruction(Value *v1, Value *v2) {
    return builder.CreateOr(v1,v2,"var");
  }
  
  Value *  emit_not_instruction(Value *v) {
    return builder.CreateNot(v,"var");
  }


//
// codegen for relational operators
//

  Value *    emit_lessthan_instruction(Value *v1, Value *v2)
  {
    return builder.CreateFCmpULT(v1,v2,"var");
  }
  Value *    emit_greaterthan_instruction(Value *v1, Value *v2)
  {
    return builder.CreateFCmpUGT(v1,v2,"var");
  }
  Value *    emit_lessequal_instruction(Value *v1, Value *v2)
  {
    return builder.CreateFCmpULE(v1,v2,"var");
  }
  Value *    emit_greaterequal_instruction(Value *v1, Value *v2)
  {
    return builder.CreateFCmpUGE(v1,v2,"var");
  }
  Value *    emit_equalequal_instruction(Value *v1, Value *v2)
  {
    return builder.CreateFCmpOEQ(v1,v2,"var123");
  }
  Value *    emit_notequal_instruction(Value *v1, Value *v2)
  {
    return builder.CreateFCmpUNE(v1,v2,"var");
  }

//
// codegen for print statements
//

  void  emit_print_stmt(Value *value, Type *type, const char *format) {

    Value *val = builder.CreateGlobalStringPtr(format);
    std::vector<llvm::Type *> putsArgs;
    putsArgs.push_back(builder.getInt8Ty()->getPointerTo());
    putsArgs.push_back(type);
    
    llvm::ArrayRef<llvm::Type*>  argsRef(putsArgs);
    llvm::FunctionType *putsType = llvm::FunctionType::get(builder.getInt32Ty(), argsRef, true);
    llvm::Constant *putsFunc = module->getOrInsertFunction("printf", putsType);

    std::vector<llvm::Value *> putsArgs1;
    putsArgs1.push_back(val);
    putsArgs1.push_back(value);

    llvm::ArrayRef<llvm::Value*>  argsRef1(putsArgs1);
    builder.CreateCall(putsFunc,argsRef1);
  }
  
  void emit_print_stmt(Value *val,TypeInfo type)
  {
    
    if(type == TYPE_STRING) {
      emit_print_stmt(val,builder.getInt8Ty()->getPointerTo(),"%s");
    }
    else if(type == TYPE_NUMERIC) {      
      emit_print_stmt(val,builder.getDoubleTy(),"%f");
    }
    else if(type == TYPE_BOOL) {
      emit_print_stmt(val,builder.getInt1Ty(),"%d");
    }
    
  }
  
  void emit_printline_stmt(Value *val,TypeInfo type)
  {
    if(type == TYPE_STRING) {
      emit_print_stmt(val,builder.getInt8Ty()->getPointerTo(),"%s\n");
    }
    else if(type == TYPE_NUMERIC) {
      emit_print_stmt(val,builder.getDoubleTy(),"%f\n");
      
    }
    else if(type == TYPE_BOOL) {
      emit_print_stmt(val,builder.getInt8Ty(),"%d\n");
    }
  }

 
  
  
}


