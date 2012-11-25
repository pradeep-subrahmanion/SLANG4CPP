#include "codegen.h"

namespace CodeGen
{
	llvm::LLVMContext & context = llvm::getGlobalContext();
	llvm::Module *module = new llvm::Module("asdf", context);
	llvm::IRBuilder<> builder(context);

	void generate_top_level_code()
	{

  		llvm::FunctionType *funcType = llvm::FunctionType::get(builder.getVoidTy(), false);
 		llvm::Function *mainFunc = 
   	llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, "main", module);
  		llvm::BasicBlock *entry = llvm::BasicBlock::Create(context, "entrypoint", mainFunc);
  		builder.SetInsertPoint(entry);
	
	}

	void generate_ret_stmt()
	{
		builder.CreateRetVoid();
	}

	void generate_print_stmt(Value *val)
	{
		std::vector<llvm::Type *> putsArgs;
  		putsArgs.push_back(builder.getInt8Ty()->getPointerTo());
  		llvm::ArrayRef<llvm::Type*>  argsRef(putsArgs);
 		llvm::FunctionType *putsType = llvm::FunctionType::get(builder.getInt32Ty(), argsRef, false);
  		llvm::Constant *putsFunc = module->getOrInsertFunction("printf", putsType);
  		builder.CreateCall(putsFunc, val);
	}

	void generate_printline_stmt(Value *val)
	{
		generate_print_stmt(val);
		generate_print_stmt(builder.CreateGlobalStringPtr("\n"));	
	}

	Value	*generate_global_string_for_double(double d)
	{
		char buffer[32];
  		snprintf(buffer, 32, "%g",d);
		Value *val = builder.CreateGlobalStringPtr(buffer);	
		return val;
	}

	

}


