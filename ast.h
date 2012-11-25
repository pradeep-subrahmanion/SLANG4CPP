#include "common.h"
#include "codegen.h"

#ifndef AST_INCLUDED
#define AST_INCLUDED

class Expression
{

public:
   virtual double evaluate() = 0;
	virtual Value *codegen() = 0;
};

class NumericConstant : public Expression
{

   double value;

public:
   NumericConstant(double _val);
   double evaluate();
	Value *codegen();
};

class BinaryExpression : public Expression
{

   Expression *e1, *e2;
   Operator op;  

public:
   BinaryExpression(Expression *_e1, Expression *_e2, Operator _op);
   ~BinaryExpression();
   double evaluate();
	Value *codegen();
};

class UnaryExpression : public Expression 
{
   
   Expression *e1;
   Operator op;

public:
   UnaryExpression(Expression *_e1, Operator op);
   ~UnaryExpression();
   double evaluate();
   Value *codegen();
};



#endif
