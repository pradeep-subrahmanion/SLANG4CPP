#ifndef CODEGEN_INCLUDED
#define CODEGEN_INCLUDED

#include "llvm/DerivedTypes.h"
#include "llvm/IRBuilder.h"
#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/Bitcode/ReaderWriter.h"
#include "llvm/Support/raw_ostream.h"
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

	void	generate_top_level_code();
	void	generate_ret_stmt();
	void  generate_print_stmt(Value *val);
	void  generate_printline_stmt(Value *val);
	Value	*generate_global_string_for_double(double d);
}

#endif
