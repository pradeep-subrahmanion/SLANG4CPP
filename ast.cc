#include "ast.h"

using namespace CodeGen;

NumericConstant::NumericConstant(double _value)
{
	value = _value;
}

double NumericConstant::evaluate()
{
   
   return value;

}

Value *NumericConstant::codegen()
{
 	double d = evaluate();
 	Value *val = generate_global_string_for_double(d);
 	return val;
}

BinaryExpression::BinaryExpression(Expression *_e1, Expression *_e2, Operator _op)
{
   op = _op;
  	e1 = _e1;
  	e2 = _e2;
}

double BinaryExpression::evaluate()
{

  switch(op) {

      case OPERATOR_PLUS: 
         return e1->evaluate() + e2->evaluate();

      case OPERATOR_MINUS:
         return e1->evaluate() - e2->evaluate();

      case OPERATOR_MUL:
         return e1->evaluate() * e2->evaluate();

      case OPERATOR_DIV:
         return e1->evaluate() / e2->evaluate();

  }

  return NAN;
}

Value *BinaryExpression::codegen()
{
	double d = evaluate();
 	Value *val = generate_global_string_for_double(d);
 	return val;
}

BinaryExpression:: ~BinaryExpression()
{
   delete e1;
   delete e2;
}

UnaryExpression::UnaryExpression(Expression *_e1, Operator _op)
{
   e1 = _e1;
   op = _op;
}

double UnaryExpression::evaluate()
{
   if(op == OPERATOR_PLUS)
      return e1->evaluate();

   else if(op == OPERATOR_MINUS) 
      return -e1->evaluate();

}

Value *UnaryExpression::codegen()
{
	double d = evaluate();
 	Value *val = generate_global_string_for_double(d);
 	return val;
}


UnaryExpression:: ~UnaryExpression()
{
   delete e1;
}
