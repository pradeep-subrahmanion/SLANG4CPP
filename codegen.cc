#include "codegen.h"

namespace CodeGen
{
  llvm::LLVMContext & context = llvm::getGlobalContext();
  llvm::Module *module = new llvm::Module("main_module", context);
  llvm::IRBuilder<> builder(context);
  llvm::Function *mainFunc;

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

  AllocaInst * emit_stack_variable(SymbolInfo *info) {
    
    IRBuilder<> TmpB(&mainFunc->getEntryBlock(),
                     mainFunc->getEntryBlock().begin());
    AllocaInst *alc = NULL;
    
    if(info->type == TYPE_STRING) {
    }
    else if(info->type == TYPE_NUMERIC) {
      
     alc = TmpB.CreateAlloca(Type::getDoubleTy(getGlobalContext()), 0,
                               info->symbol_name.c_str());
    }
    else if(info->type == TYPE_BOOL) {
      
      alc = TmpB.CreateAlloca(Type::getInt8Ty(getGlobalContext()), 0,
                              info->symbol_name.c_str());
    }
    
    return alc;
  }


  void emit_store_Instruction(AllocaInst *alloca, Value *val) {
    builder.CreateStore(val, alloca);
  }

  Value *emit_load_Instruction(AllocaInst *alloca) {
    return builder.CreateLoad(alloca);
  }

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
  
  Value *  emit_and_instruction(Value *v1, Value *v2) {
    return builder.CreateAnd(v1,v2,"var");
  }
  
  Value *  emit_or_instruction(Value *v1, Value *v2) {
    return builder.CreateOr(v1,v2,"var");
  }
  
  Value *  emit_not_instruction(Value *v) {
    return builder.CreateNot(v,"var");
  }

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
    }
    else if(type == TYPE_NUMERIC) {
      
      emit_print_stmt(val,builder.getDoubleTy(),"%f");

    }
    else if(type == TYPE_BOOL) {
      emit_print_stmt(val,builder.getInt8Ty(),"%d");
    }
    
  }
  
  void emit_printline_stmt(Value *val,TypeInfo type)
  {
    if(type == TYPE_STRING) {
    }
    else if(type == TYPE_NUMERIC) {
      emit_print_stmt(val,builder.getDoubleTy(),"%f\n");
      
    }
    else if(type == TYPE_BOOL) {
      emit_print_stmt(val,builder.getInt8Ty(),"%d\n");
    }
  }

  Value * emit_condition(Value *val)
  {
    
    return builder.CreateFCmpONE(val,
                              ConstantFP::get(getGlobalContext(), APFloat(0.0)),
                                "ifcond");
  }

  BasicBlock * emit_block_in_main(const char *name) 
  {

    return BasicBlock::Create(getGlobalContext(), name, mainFunc);  

  }
  void  emit_conditional_branch(Value *condition_val , BasicBlock *then_block, BasicBlock *else_block) 
  {
    builder.CreateCondBr(condition_val, then_block, else_block);
  }

  void move_to_block(BasicBlock *block)
  {
    builder.SetInsertPoint(block);
  }

  BasicBlock *get_insert_block()
  {
    return builder.GetInsertBlock();
  }

  void create_branch(BasicBlock *block)
  {
     builder.CreateBr(block);
  }

  PHINode * emit_phi_node()
  {
    return builder.CreatePHI(Type::getDoubleTy(getGlobalContext()), 2,
                                  "iftmp");
  }

  void add_blockval_in_phi(PHINode *phi, BasicBlock *block, Value *val)
  {
      phi->addIncoming(val, block); 
  }
  
}


