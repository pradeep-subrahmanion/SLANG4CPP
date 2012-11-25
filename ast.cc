#include "ast.h"
#include "common.h"

using namespace CodeGen;

// Boolean Constant 

BooleanConstant::BooleanConstant(bool val)
{
	info = new SymbolInfo();
	info->symbol_name = "";
	info->type = TYPE_BOOL;
	info->bool_val = val;
}
SymbolInfo *BooleanConstant::evaluate()
{
	return info;
}
TypeInfo BooleanConstant::typecheck()
{
	return info->type;
}
TypeInfo BooleanConstant::get_type()
{
	return info->type;
}

// Numeric Constant 

NumericConstant::NumericConstant(double _value)
{
	info = new SymbolInfo();
	info->symbol_name = "";
	info->type = TYPE_BOOL;
	info->double_val = _value;
}

SymbolInfo *NumericConstant::evaluate()
{
 
   return info;
}

TypeInfo NumericConstant::typecheck()
{
	return info->type;
}
TypeInfo NumericConstant::get_type()
{
	return info->type;
}




// String Literal

StringLiteral::StringLiteral(std::string _value)
{
	info = new SymbolInfo();
	info->symbol_name = "";
	info->type = TYPE_BOOL;
	info->string_val = _value;
}

SymbolInfo *StringLiteral::evaluate()
{
 
   return info;
}

TypeInfo StringLiteral::typecheck()
{
	return info->type;
}
TypeInfo StringLiteral::get_type()
{
	return info->type;
}

//Variable

Variable::Variable(SymbolInfo *info)
{
   name = info->symbol_name;
}
Variable::Variable(Compilation_Context *ctx, std::string _name, double _value)
{
   SymbolInfo *info = new SymbolInfo();
   info->symbol_name = _name;
   info->double_val = _value;
   info->type = TYPE_NUMERIC;
   ///symbol table add
   ctx->add_symbol(info);

   
}
Variable::Variable(Compilation_Context *ctx, std::string _name, std::string _value)
{
   SymbolInfo *info = new SymbolInfo();
   info->symbol_name = _name;
   info->string_val = _value;
   info->type = TYPE_STRING;
   ///symbol table add
   ctx->add_symbol(info);
}
Variable::Variable(Compilation_Context *ctx, std::string _name, bool _value)
{
   SymbolInfo *info = new SymbolInfo();
   info->symbol_name = _name;
   info->string_val = _value;
   info->type = TYPE_BOOL;
   ///symbol table add
   ctx->add_symbol(info);
}
std::string Variable::get_name()
{

}
SymbolInfo *Variable::evaluate()
{

}
TypeInfo Variable::typecheck()
{

}
TypeInfo Variable::get_type()
{
}



#if 0
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
#endif
