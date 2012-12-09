
#ifndef COMMON_INCLUDED
#define COMMON_INCLUDED

#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <vector>

#include "llvm/DerivedTypes.h"
#include "llvm/IRBuilder.h"
#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/Bitcode/ReaderWriter.h"
#include "llvm/Support/raw_ostream.h"

template <class T> class SymbolTable;



// type to rep operator.

typedef enum {
  OPERATOR_ILLEGAL = -1,
  OPERATOR_PLUS,
  OPERATOR_MINUS,
  OPERATOR_MUL,
  OPERATOR_DIV
  
}Operator;

// type to rep token.

typedef enum {
  TOKEN_ILLEGAL = -1,
  TOKEN_PLUS,
  TOKEN_MINUS,
  TOKEN_MUL,
  TOKEN_DIV,
  TOKEN_OPAREN,
  TOKEN_CPAREN,
  TOKEN_DOUBLE,
  TOKEN_NULL,
  TOKEN_PRINT,
  TOKEN_PRINTLN,
  TOKEN_UNQUOTED_STRING,
  TOKEN_SEMI,
  
  ///step 4 additions
  
  TOKEN_VAR_NUMBER,
  TOKEN_VAR_STRING,
  TOKEN_VAR_BOOL,
  TOKEN_NUMERIC,
  TOKEN_COMMENT ,
  TOKEN_BOOL_TRUE,
  TOKEN_BOOL_FALSE ,
  TOKEN_STRING,
  TOKEN_ASSIGN,

// relational operators

  TOKEN_EQUAL,
  TOKEN_NEQUAL,
  TOKEN_GREATER_THAN,
  TOKEN_GREATER_EQUAL,
  TOKEN_LESS_THAN,
  TOKEN_LESS_EQUAL,
  TOKEN_AND,
  TOKEN_OR,
  TOKEN_NOT,

// Control structures

  TOKEN_IF,
  TOKEN_THEN,
  TOKEN_ELSE,
  TOKEN_ENDIF,
  TOKEN_WHILE,
  TOKEN_ENDWHILE

  
}Token;

// type to rep supported variable types.

typedef enum {
	TYPE_ILLEGAL,
	TYPE_NUMERIC,
	TYPE_BOOL,
	TYPE_STRING
}TypeInfo;

// type to rep relational operators.

typedef enum {
  OPTR_ILLEGAL,
  OPTR_EQUAL,
  OPTR_NEQUAL,
  OPTR_GREATER_THAN,
  OPTR_GREATER_EQUAL,
  OPTR_LESS_THAN,
  OPTR_LESS_EQUAL
}RelationalOperator;

typedef enum {
  ExpressionTypeNone,
  ExpressionTypeStringLiteral ,
  ExpressionTypeVariable,
  ExpressionTypeBinaryPlus
}ExpressionType;


// Represents symbol.

class SymbolInfo
{
public:
	std::string symbol_name;
	TypeInfo type;
	double double_val;
	std::string string_val;
	bool bool_val;
  SymbolInfo() {};
  SymbolInfo(std::string _name) { symbol_name = _name;}
  
};

// Some common functions

void exit_with_message(const char *err_msg);
RelationalOperator get_relation_operator(Token token);



typedef SymbolTable<SymbolInfo*> SymbolInfoTable;
typedef SymbolTable<llvm::AllocaInst*> AllocaInstTable;

#endif
