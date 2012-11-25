
#ifndef COMMON_INCLUDED
#define COMMON_INCLUDED

#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <vector>

typedef enum {
   OPERATOR_ILLEGAL = -1,
   OPERATOR_PLUS,
   OPERATOR_MINUS,
   OPERATOR_MUL,
   OPERATOR_DIV

}Operator;

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
   TOKEN_SEMI
}Token;

typedef enum {
	TYPE_ILLEGAL,
	TYPE_NUMERIC,
	TYPE_BOOL,
	TYPE_STRING
}TypeInfo;

class SymbolInfo
{
public:
	std::string symbol_name;
	TypeInfo type;
	double double_val;
	std::string string_val;
	bool bool_val;

};

#endif
