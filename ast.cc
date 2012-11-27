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
SymbolInfo *BooleanConstant::evaluate(Runtime_Context *ctx)
{
	return info;
}
TypeInfo BooleanConstant::typecheck(Compilation_Context *ctx)
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
	info->type = TYPE_NUMERIC;
	info->double_val = _value;
}

SymbolInfo *NumericConstant::evaluate(Runtime_Context *ctx)
{
 
   return info;
}

TypeInfo NumericConstant::typecheck(Compilation_Context *ctx)
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
	info->type = TYPE_STRING;
	info->string_val = _value;
}

SymbolInfo *StringLiteral::evaluate(Runtime_Context *ctx)
{
 
   return info;
}

TypeInfo StringLiteral::typecheck(Compilation_Context *ctx)
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
   return name;
}
SymbolInfo *Variable::evaluate(Runtime_Context *ctx)
{
   SymbolTable *st = ctx->get_symboltable();

   if(st == NULL) {
      return NULL;
   }

   SymbolInfo *inf = st->get(name);
   return inf;
}
TypeInfo Variable::typecheck(Compilation_Context *ctx)
{
   SymbolTable *st = ctx->get_symboltable();
   if(st==NULL) {
      return TYPE_ILLEGAL;
   }
   else {
      SymbolInfo *inf = st->get(name);
      if(inf != NULL) {
         type = inf->type;
         return type;
      }

   }

   return TYPE_ILLEGAL;
}
TypeInfo Variable::get_type()
{
   return type;
}

//Binary Plus

BinaryPlus::BinaryPlus(Expression *e1,Expression *e2)
{
   exp1 = e1;
   exp2 = e2;
}
SymbolInfo *BinaryPlus::evaluate(Runtime_Context *ctx)
{
   SymbolInfo *eval_left = exp1->evaluate(ctx);
   SymbolInfo *eval_right = exp1->evaluate(ctx);
   
   if(eval_left->type == TYPE_STRING && eval_right->type == TYPE_STRING) {
      SymbolInfo *info = new SymbolInfo();
      info->type = TYPE_STRING;
      info->string_val = eval_left->string_val + eval_right->string_val;
      return info;
   }

   else if(eval_left->type == TYPE_NUMERIC && eval_right->type == TYPE_NUMERIC) {
      SymbolInfo *info = new SymbolInfo();
      info->type = TYPE_NUMERIC;
      info->double_val = eval_left->double_val + eval_right->double_val;
      return info;
   }
   
   else {
     std::cout << "Type Mismatch";
   }
   
   return NULL;
}
TypeInfo BinaryPlus::typecheck(Compilation_Context *ctx)
{
   TypeInfo info1 = exp1->typecheck(ctx);
   TypeInfo info2 = exp2->typecheck(ctx);

   if(info1 == info2 && info1 != TYPE_BOOL) {
      type = info1;
      return type;   
   }
}
TypeInfo BinaryPlus::get_type()
{
   return type;
}

//Binary Minus


BinaryMinus::BinaryMinus(Expression *e1,Expression *e2)
{
   exp1 = e1;
   exp2 = e2;
}
SymbolInfo *BinaryMinus::evaluate(Runtime_Context *ctx)
{
   SymbolInfo *eval_left = exp1->evaluate(ctx);
   SymbolInfo *eval_right = exp1->evaluate(ctx);

  if(eval_left->type == TYPE_NUMERIC && eval_right->type == TYPE_NUMERIC) {
      SymbolInfo *info = new SymbolInfo();
      info->type = TYPE_NUMERIC;
      info->double_val = eval_left->double_val - eval_right->double_val;
      return info;
   }
   
   else {
     std::cout << "Type Mismatch";
   }
   
   return NULL;
}
TypeInfo BinaryMinus::typecheck(Compilation_Context *ctx)
{
   TypeInfo info1 = exp1->typecheck(ctx);
   TypeInfo info2 = exp2->typecheck(ctx);

   if(info1 == info2 && info1 == TYPE_NUMERIC) {
      type = info1;
      return type;   
   }
}
TypeInfo BinaryMinus::get_type()
{
   return type;
}

//Multiplication


Mult::Mult(Expression *e1,Expression *e2)
{
   exp1 = e1;
   exp2 = e2;
}
SymbolInfo *Mult::evaluate(Runtime_Context *ctx)
{
   SymbolInfo *eval_left = exp1->evaluate(ctx);
   SymbolInfo *eval_right = exp1->evaluate(ctx);

  if(eval_left->type == TYPE_NUMERIC && eval_right->type == TYPE_NUMERIC) {
      SymbolInfo *info = new SymbolInfo();
      info->type = TYPE_NUMERIC;
      info->double_val = eval_left->double_val * eval_right->double_val;
      return info;
   }
   
   else {
     std::cout << "Type Mismatch";
   }
   
   return NULL;
}
TypeInfo Mult::typecheck(Compilation_Context *ctx)
{
   TypeInfo info1 = exp1->typecheck(ctx);
   TypeInfo info2 = exp2->typecheck(ctx);

   if(info1 == info2 && info1 == TYPE_NUMERIC) {
      type = info1;
      return type;   
   }
}
TypeInfo Mult::get_type()
{
   return type;
}

//Division


Div::Div(Expression *e1,Expression *e2)
{
   exp1 = e1;
   exp2 = e2;
}
SymbolInfo *Div::evaluate(Runtime_Context *ctx)
{
   SymbolInfo *eval_left = exp1->evaluate(ctx);
   SymbolInfo *eval_right = exp1->evaluate(ctx);

  if(eval_left->type == TYPE_NUMERIC && eval_right->type == TYPE_NUMERIC) {
      SymbolInfo *info = new SymbolInfo();
      info->type = TYPE_NUMERIC;
      info->double_val = eval_left->double_val / eval_right->double_val;
      return info;
   }
   
   else {
     std::cout << "Type Mismatch";
   }
   
   return NULL;
}
TypeInfo Div::typecheck(Compilation_Context *ctx)
{
   TypeInfo info1 = exp1->typecheck(ctx);
   TypeInfo info2 = exp2->typecheck(ctx);

   if(info1 == info2 && info1 == TYPE_NUMERIC) {
      type = info1;
      return type;   
   }
}
TypeInfo Div::get_type()
{
   return type;
}

//UnaryPlus

UnaryPlus::UnaryPlus(Expression *e1)
{
   exp1 = e1;
}
SymbolInfo *UnaryPlus::evaluate(Runtime_Context *ctx)
{
  SymbolInfo *eval_left = exp1->evaluate(ctx);

  if(eval_left->type == TYPE_NUMERIC) {
      SymbolInfo *info = new SymbolInfo();
      info->type = TYPE_NUMERIC;
      info->double_val = eval_left->double_val;
      return info;
   }
   
   else {
     std::cout << "Type Mismatch";
   }
   
   return NULL;
}
TypeInfo UnaryPlus::typecheck(Compilation_Context *ctx)
{

   TypeInfo info1 = exp1->typecheck(ctx);

   if(info1 == TYPE_NUMERIC) {
      type = info1;
      return type;   
   }

  else {
     std::cout << "Type Mismatch";
   }

   return TYPE_ILLEGAL;

}
TypeInfo UnaryPlus::get_type()
{
   return type;
}


//UnaryMinus

UnaryMinus::UnaryMinus(Expression *e1)
{
   exp1 = e1;
}
SymbolInfo *UnaryMinus::evaluate(Runtime_Context *ctx)
{
  SymbolInfo *eval_left = exp1->evaluate(ctx);

  if(eval_left->type == TYPE_NUMERIC) {
      SymbolInfo *info = new SymbolInfo();
      info->type = TYPE_NUMERIC;
      info->double_val = -eval_left->double_val;
      return info;
   }
   
   else {
     std::cout << "Type Mismatch";
   }
   
   return NULL;
}
TypeInfo UnaryMinus::typecheck(Compilation_Context *ctx)
{

   TypeInfo info1 = exp1->typecheck(ctx);

   if(info1 == TYPE_NUMERIC) {
      type = info1;
      return type;   
   }

  else {
     std::cout << "Type Mismatch";
   }

   return TYPE_ILLEGAL;

}
TypeInfo UnaryMinus::get_type()
{
   return type;
}




