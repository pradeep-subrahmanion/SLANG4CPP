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
  
  void emit_print_stmt(Value *val)
  {
    std::vector<llvm::Type *> putsArgs;
    putsArgs.push_back(builder.getInt8Ty()->getPointerTo());
    llvm::ArrayRef<llvm::Type*>  argsRef(putsArgs);
    llvm::FunctionType *putsType = llvm::FunctionType::get(builder.getInt32Ty(), argsRef, false);
    llvm::Constant *putsFunc = module->getOrInsertFunction("printf", putsType);
    builder.CreateCall(putsFunc, val);
  }
  
  void emit_printline_stmt(Value *val)
  {
    emit_print_stmt(val);
    emit_print_stmt(builder.CreateGlobalStringPtr("\n"));
  }
  
  Value	*emit_global_string_for_double(double d)
  {
    char buffer[32];
    snprintf(buffer, 32, "%g",d);
    Value *val = builder.CreateGlobalStringPtr(buffer);
    return val;
  }

  AllocaInst *emit_numeric_stack_variable(const std::string &VarName) {
    IRBuilder<> TmpB(&mainFunc->getEntryBlock(),
                 mainFunc->getEntryBlock().begin());
    return TmpB.CreateAlloca(Type::getDoubleTy(getGlobalContext()), 0,
                           VarName.c_str());
  }

  void emit_store_Instruction(AllocaInst *alloca, Value *val) {
    builder.CreateStore(val, alloca);
  }

  Value *emit_load_Instruction(AllocaInst *alloca) {
    return builder.CreateLoad(alloca);
  }

  void emit_add_instruction(Value *v1, Value *v2) {
    builder.CreateFAdd(v1, v2, "nextvar");
  }

  void  emit_print_stmt_dbl(Value *value) {

    Value *val = builder.CreateGlobalStringPtr("%f");
    std::vector<llvm::Type *> putsArgs;
    putsArgs.push_back(builder.getInt8Ty()->getPointerTo());
    putsArgs.push_back(builder.getDoubleTy());
    llvm::ArrayRef<llvm::Type*>  argsRef(putsArgs);
    llvm::FunctionType *putsType = llvm::FunctionType::get(builder.getInt32Ty(), argsRef, false);
    llvm::Constant *putsFunc = module->getOrInsertFunction("printf", putsType);

    std::vector<llvm::Value *> putsArgs1;
    putsArgs1.push_back(val);
    putsArgs1.push_back(value);

    llvm::ArrayRef<llvm::Value*>  argsRef1(putsArgs1);
    builder.CreateCall(putsFunc,argsRef1);
  }
  
}


