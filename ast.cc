#include "ast.h"
#include "common.h"
#include "context.h"

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

Value *BooleanConstant::codegen(Execution_Context *ctx)
{
  int i = (info->bool_val == false) ? 0 : 1;
  return ConstantInt::get(getGlobalContext(), APInt(1,i));
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

Value *NumericConstant::codegen(Execution_Context *ctx)
{
  return ConstantFP::get(getGlobalContext(), APFloat(info->double_val));
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

Value *StringLiteral::codegen(Execution_Context *ctx)
{
  return emit_global_string(info->string_val.c_str());
}
//Variable

Variable::Variable(SymbolInfo *info)
{
  name = info->symbol_name;
  type = info->type;

}
Variable::Variable(Compilation_Context *ctx, std::string _name, double _value)
{
  SymbolInfo *info = new SymbolInfo();
  info->symbol_name = _name;
  info->double_val = _value;
  info->type = TYPE_NUMERIC;
  ///symbol table add
  ctx->add_symbol(info);
  name = info->symbol_name;
}
Variable::Variable(Compilation_Context *ctx, std::string _name, std::string _value)
{
  SymbolInfo *info = new SymbolInfo();
  info->symbol_name = _name;
  info->string_val = _value;
  info->type = TYPE_STRING;
  ///symbol table add
  ctx->add_symbol(info);
  name = info->symbol_name;

}
Variable::Variable(Compilation_Context *ctx, std::string _name, bool _value)
{
  SymbolInfo *info = new SymbolInfo();
  info->symbol_name = _name;
  info->bool_val = _value;
  info->type = TYPE_BOOL;
  ///symbol table add
  ctx->add_symbol(info);
  name = info->symbol_name;
}
std::string Variable::get_name()
{
  return name;
}
SymbolInfo *Variable::evaluate(Runtime_Context *ctx)
{
  SymbolInfoTable *st = ctx->get_symboltable();
  
  if(st == NULL) {
    return NULL;
  }
  
  SymbolInfo *inf = st->get(name);
  return inf;
}
TypeInfo Variable::typecheck(Compilation_Context *ctx)
{

  SymbolInfoTable *st = ctx->get_symboltable();
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

Value *Variable::codegen(Execution_Context *ctx)
{
  AllocaInst *alcInst = ctx->get_symbol(name);
  return emit_load_Instruction(alcInst);
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
  SymbolInfo *eval_right = exp2->evaluate(ctx);
  
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
    exit_with_message("Type Mismatch");
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

  return TYPE_ILLEGAL;
}
TypeInfo BinaryPlus::get_type()
{
  return type;
}

Value *BinaryPlus::codegen(Execution_Context *ctx)
{
  TypeInfo info1 = exp1->get_type();
  TypeInfo info2 = exp2->get_type();

  Value *result = NULL;

  if(info1 == info2) {
    if(info1 == TYPE_NUMERIC) {
      result = emit_add_instruction(exp1->codegen(ctx),exp2->codegen(ctx));
    }
    else if(info1 == TYPE_STRING) {

    }

  }

  return result;
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
  SymbolInfo *eval_right = exp2->evaluate(ctx);
  
  if(eval_left->type == TYPE_NUMERIC && eval_right->type == TYPE_NUMERIC) {
    SymbolInfo *info = new SymbolInfo();
    info->type = TYPE_NUMERIC;
    info->double_val = eval_left->double_val - eval_right->double_val;
    return info;
  }
  
  else {
    exit_with_message("Type Mismatch");
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

Value *BinaryMinus::codegen(Execution_Context *ctx)
{
  Value *val1 = exp1->codegen(ctx);
  Value *val2 = exp2->codegen(ctx);
  
  return emit_sub_instruction(val1,val2);
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
  SymbolInfo *eval_right = exp2->evaluate(ctx);
  
  if(eval_left->type == TYPE_NUMERIC && eval_right->type == TYPE_NUMERIC) {
    SymbolInfo *info = new SymbolInfo();
    info->type = TYPE_NUMERIC;
    info->double_val = eval_left->double_val * eval_right->double_val;
    return info;
  }
  
  else {
    exit_with_message("Type Mismatch");
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

Value *Mult::codegen(Execution_Context *ctx)
{
  Value *val1 = exp1->codegen(ctx);
  Value *val2 = exp2->codegen(ctx);
  
  return emit_mul_instruction(val1,val2);
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
  SymbolInfo *eval_right = exp2->evaluate(ctx);
  
  if(eval_left->type == TYPE_NUMERIC && eval_right->type == TYPE_NUMERIC) {
    SymbolInfo *info = new SymbolInfo();
    info->type = TYPE_NUMERIC;
    info->double_val = eval_left->double_val / eval_right->double_val;
    return info;
  }
  
  else {
    exit_with_message("Type Mismatch");
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

Value *Div::codegen(Execution_Context *ctx)
{
  Value *val1 = exp1->codegen(ctx);
  Value *val2 = exp2->codegen(ctx);
  
  return emit_div_instruction(val1,val2);
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
    exit_with_message("Type Mismatch");
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
    exit_with_message("Type Mismatch");
  }
  
  return TYPE_ILLEGAL;
  
}
TypeInfo UnaryPlus::get_type()
{
  return type;
}

Value *UnaryPlus::codegen(Execution_Context *ctx)
{
  Value *val = exp1->codegen(ctx);
  return val;
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
    exit_with_message("Type Mismatch");
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
    exit_with_message("Type Mismatch");
  }
  
  return TYPE_ILLEGAL;
  
}
TypeInfo UnaryMinus::get_type()
{
  return type;
}

Value *UnaryMinus::codegen(Execution_Context *ctx)
{
  Value *v = exp1->codegen(ctx);
  return emit_unary_minus_instruction(v);
}


// Relational Operator

RelationalExpression::RelationalExpression(Expression *e1,Expression *e2,RelationalOperator _op)
{
  exp1 = e1;
  exp2 = e2;
  optr = _op;
}

SymbolInfo * RelationalExpression::evaluate(Runtime_Context *ctx)
{
  SymbolInfo *eval_left = exp1->evaluate(ctx);
  SymbolInfo *eval_right = exp2->evaluate(ctx);

  SymbolInfo *ret = new SymbolInfo();
  
  if(eval_left->type == TYPE_NUMERIC && eval_right->type == TYPE_NUMERIC) {
    ret->type = TYPE_BOOL;
    ret->symbol_name = "";
     
    if(optr == OPTR_EQUAL) {
      ret->bool_val = (eval_left->double_val == eval_right->double_val);
    }
    else if(optr == OPTR_NEQUAL) {
      ret->bool_val = (eval_left->double_val != eval_right->double_val);
    }
    else if(optr == OPTR_GREATER_THAN) {
      ret->bool_val = (eval_left->double_val > eval_right->double_val);
    }
    else if(optr == OPTR_GREATER_EQUAL) {
      ret->bool_val = (eval_left->double_val >= eval_right->double_val);
    }
    else if(optr == OPTR_LESS_THAN) {
      ret->bool_val = (eval_left->double_val < eval_right->double_val);
    }
    else if(optr == OPTR_LESS_EQUAL) {
      ret->bool_val = (eval_left->double_val <= eval_right->double_val);
    }
 
    return ret;
  }

  else if(eval_left->type == TYPE_STRING && eval_right->type == TYPE_STRING) {
    ret->type = TYPE_BOOL;
    ret->symbol_name = "";
    
    if(optr == OPTR_EQUAL) {
      ret->bool_val = (eval_left->string_val.compare(eval_right->string_val) == 0);
    }
    else if(optr == OPTR_NEQUAL) {
      ret->bool_val = (eval_left->string_val.compare(eval_right->string_val) != 0);
    }
    else {
      ret->bool_val = false;
    }
    return ret;
  }
  else if(eval_left->type == TYPE_BOOL && eval_right->type == TYPE_BOOL) {
    ret->type = TYPE_BOOL;
    ret->symbol_name = "";
    
    if(optr == OPTR_EQUAL) {
      ret->bool_val = (eval_left->bool_val == eval_right->bool_val);
    }
    else if(optr == OPTR_NEQUAL) {
      ret->bool_val = (eval_left->bool_val != eval_right->bool_val);
    }
    else {
      ret->bool_val = false;
    }
    return ret;
  }

}

TypeInfo RelationalExpression::typecheck(Compilation_Context *ctx)
{
  TypeInfo left_type = exp1->typecheck(ctx);
  TypeInfo right_type = exp2->typecheck(ctx);

  if(left_type != right_type) {
    exit_with_message("Wrong type in expression");
  }
  else if((left_type == TYPE_STRING ) && (optr != OPTR_EQUAL && optr != OPTR_NEQUAL)) {
    exit_with_message("Only == and != supported for string type");
  }
  else if((left_type == TYPE_BOOL ) && (optr != OPTR_EQUAL && optr != OPTR_NEQUAL)) {
    exit_with_message("Only == and != supported for bool type");
  }

  operand_type = left_type;
  type = TYPE_BOOL;
  return type;
}

TypeInfo RelationalExpression::get_type()
{
  return type;
}

Value *RelationalExpression::codegen(Execution_Context *ctx)
{

  TypeInfo type1 =  exp1->get_type();
  TypeInfo type2 =  exp2->get_type();
  
  Value *v1= exp1->codegen(ctx);
  Value *v2= exp2->codegen(ctx);

  if(type1 == TYPE_NUMERIC && type2 == TYPE_NUMERIC) {

    if(optr == OPTR_EQUAL) {
      return emit_equalequal_instruction(v1,v2);
    }
    else if(optr == OPTR_NEQUAL) {
      return emit_notequal_instruction(v1,v2);
    }
    else if(optr == OPTR_GREATER_THAN) {
      return emit_greaterthan_instruction(v1,v2);
    }
    else if(optr == OPTR_GREATER_EQUAL) {
      return emit_greaterequal_instruction(v1,v2);
    }
    else if(optr == OPTR_LESS_THAN) {
      return emit_lessthan_instruction(v1,v2);
    }
    else if(optr == OPTR_LESS_EQUAL) {
      return emit_lessequal_instruction(v1,v2);
    }
  }

  return NULL;
 
}


// Logical Operator

LogicalExpression::LogicalExpression(Expression *e1,Expression *e2,Token _op)
{
  exp1 = e1;
  exp2 = e2;
  optr = _op;
}

SymbolInfo * LogicalExpression::evaluate(Runtime_Context *ctx)
{
  SymbolInfo *eval_left = exp1->evaluate(ctx);
  SymbolInfo *eval_right = exp2->evaluate(ctx);

  SymbolInfo *ret = new SymbolInfo();
  if(eval_left->type == TYPE_BOOL && eval_right->type == TYPE_BOOL) {
    ret->type = TYPE_BOOL;
    ret->symbol_name = "";
    
    if(optr == TOKEN_AND) {
      ret->bool_val = (eval_left->bool_val && eval_right->bool_val);
    }
    else if(optr == TOKEN_OR) {
      ret->bool_val = (eval_left->bool_val || eval_right->bool_val);
    }

    else {
      ret->bool_val == TYPE_BOOL;
    }
    return ret;
  }

}

TypeInfo LogicalExpression::typecheck(Compilation_Context *ctx)
{
  TypeInfo left_type = exp1->typecheck(ctx);
  TypeInfo right_type = exp2->typecheck(ctx);

  if(left_type != TYPE_BOOL  || right_type != TYPE_BOOL) {
    exit_with_message("Wrong Type in expression");
  }

  type = TYPE_BOOL;
  return type;
}

TypeInfo LogicalExpression::get_type()
{
  return type;
}

Value *LogicalExpression::codegen(Execution_Context *ctx)
{
  Value *val1 = exp1->codegen(ctx);
  Value *val2 = exp2->codegen(ctx);
  
  if(optr == TOKEN_AND) {
    return emit_and_instruction(val1,val2);
  }
  else if(optr == TOKEN_OR) {
    return emit_or_instruction(val1,val2);
  }
  return NULL;
}

// Logical NOT

LogicalNot::LogicalNot(Expression *e1)
{
  exp = e1;
}

SymbolInfo *LogicalNot::evaluate(Runtime_Context *ctx)
{
  SymbolInfo *val = exp->evaluate(ctx);
  if(val->type == TYPE_BOOL) {
    SymbolInfo *info = new SymbolInfo();
    info->type = TYPE_BOOL;
    info->symbol_name = "";
    info->bool_val = !val->bool_val;

    return info;
  }

  return NULL;
}

TypeInfo LogicalNot::typecheck(Compilation_Context *ctx)
{
  TypeInfo info = exp->typecheck(ctx);
  if(info != TYPE_BOOL) {
    exit_with_message("Wrong type in expression");
  }
  type = TYPE_BOOL;
  return info;
}

TypeInfo LogicalNot::get_type()
{
  return type;
}

Value *LogicalNot::codegen(Execution_Context *ctx)
{
  Value *val = exp->codegen(ctx);
  return emit_not_instruction(val);
}

