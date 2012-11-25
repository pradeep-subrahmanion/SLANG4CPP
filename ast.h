#include "common.h"
#include "codegen.h"
#include "context.h"

#ifndef AST_INCLUDED
#define AST_INCLUDED

class Expression
{

public:
   virtual SymbolInfo *evaluate() = 0;
	virtual Value *codegen() = 0;
	virtual TypeInfo typecheck() = 0;
	virtual TypeInfo get_type() = 0;
};

class BooleanConstant: public Expression
{
	SymbolInfo *info;

public:
	BooleanConstant(bool val);
	SymbolInfo *evaluate();
	TypeInfo typecheck();
	TypeInfo get_type();
};

class NumericConstant : public Expression
{

	SymbolInfo *info;

public:
	NumericConstant(double _value);
	SymbolInfo *evaluate();
	TypeInfo typecheck();
	TypeInfo get_type();
};


class StringLiteral : public Expression
{

	SymbolInfo *info;

public:
	StringLiteral(std::string _value);
	SymbolInfo *evaluate();
	TypeInfo typecheck();
	TypeInfo get_type();
};


class Variable:public Expression
{
   std::string name;
   TypeInfo type;

public:

   Variable(SymbolInfo *info);
   Variable(Compilation_Context *ctx, std::string _name, double _value);
   Variable(Compilation_Context *ctx, std::string _name, std::string _value);
   Variable(Compilation_Context *ctx, std::string _name, bool _value);
   std::string get_name();
   SymbolInfo *evaluate();
	TypeInfo typecheck();
	TypeInfo get_type();
    
};


class BinaryExpression : public Expression
{

   Expression *e1, *e2;
   Operator op;  

public:
   BinaryExpression(Expression *_e1, Expression *_e2, Operator _op);
   ~BinaryExpression();
   SymbolInfo *evaluate();
	Value *codegen();
};

class UnaryExpression : public Expression 
{
   
   Expression *e1;
   Operator op;

public:
   UnaryExpression(Expression *_e1, Operator op);
   ~UnaryExpression();
   SymbolInfo *evaluate();
   Value *codegen();
};



#endif
