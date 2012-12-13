#include "parser.h"
#include "builder.h"
#include "compilation_unit.h"

Parser::Parser(string str) :
	Lexer(str) {
	module_builder = new TmoduleBuilder();
}

Tmodule *Parser::do_parse() {
	get_next();
	return parse_functions();
}

Tmodule * Parser::parse_functions() {

	while (current_token == TOKEN_FUNCTION) {

		ProcedureBuilder *pb = parse_function();

		if (pb != NULL) {

			Procedure *proc = pb->get_procedure();

			if (proc == NULL) {
				exit_with_message("Error while parsing functions");
				return NULL;
			}

			module_builder->add_procedure(proc);
			get_next();
		}

		else {
			exit_with_message("Error while parsing functions");
		}

	}

	return module_builder->get_program();
}

ProcedureBuilder * Parser::parse_function() {
	ProcedureBuilder *pb = new ProcedureBuilder("", new Compilation_Context());
	if (current_token != TOKEN_FUNCTION) {
		exit_with_message("Error while parsing function");
	}

	get_next();

	if (current_token == TOKEN_VAR_NUMBER) {
		pb->type = TYPE_NUMERIC;
	} else if (current_token == TOKEN_VAR_STRING) {
		pb->type = TYPE_STRING;
	} else if (current_token == TOKEN_VAR_BOOL) {
		pb->type = TYPE_BOOL;
	} else {
		exit_with_message("Error while parsing function");
	}

	get_next();
	if (current_token == TOKEN_UNQUOTED_STRING) {
		pb->proc_name = last_string;
	} else {
		exit_with_message("Error while parsing function");
	}

	get_next();

	if (current_token == TOKEN_OPAREN) {
		formal_parameters(pb);
	} else {
		exit_with_message("Error while parsing function");
	}

	if (current_token != TOKEN_CPAREN) {
		exit_with_message("Error while parsing function");
	}

	get_next();
	vector<Statement *> list = statement_list(pb);
	if (current_token != TOKEN_END) {
		exit_with_message("END expected");
	}

	for (int i = 0; i < list.size(); ++i) {
		pb->add_statement(list.at(i));
	}

	return pb;
}

void Parser::formal_parameters(ProcedureBuilder *ctx) {

	if (current_token != TOKEN_OPAREN) {
		exit_with_message("Opening parenthesis expected");
	}

	get_next();
	vector<TypeInfo> list_types;

	while (current_token == TOKEN_VAR_NUMBER || current_token
			== TOKEN_VAR_STRING || current_token == TOKEN_VAR_BOOL) {

		SymbolInfo *info = new SymbolInfo();
		if (current_token == TOKEN_VAR_NUMBER) {
			info->type = TYPE_NUMERIC;
		} else if (current_token == TOKEN_VAR_STRING) {
			info->type = TYPE_STRING;
		} else if (current_token == TOKEN_VAR_BOOL) {
			info->type = TYPE_BOOL;
		}

		get_next();
		if (current_token != TOKEN_UNQUOTED_STRING) {
			exit_with_message("Variable name expected");
		}
		info->symbol_name = last_string;
		list_types.push_back(info->type);
		ctx->add_local(info);
		ctx->add_formals(info);

		get_next();
		if (current_token != TOKEN_COMMA) {
			break;
		}
		get_next();
	}
	module_builder->add_function_prototypes(ctx->proc_name, ctx->type,
			list_types);
}

Expression *Parser::parse_callproc(ProcedureBuilder *ctx, Procedure *p) {
	string funcName = last_string;
	Expression *ret = NULL;
	vector<Expression *> _actuals;
	get_next();

	if (current_token != TOKEN_OPAREN) {
		exit_with_message("Opening parenthesis expected");
	}

	get_next();

	if (current_token == TOKEN_CPAREN) {
		if (p != NULL) {
			ret = new CallExpression(p, _actuals);
		}

		else {
			if (ctx ->proc_name.compare(last_string) == 0) {
				ret = new CallExpression(ctx->proc_name, _actuals, true,
						ctx->type);
			}
		}

		return ret;
	}

	while (true) {
		Expression *exp = bexpr(ctx);
		exp->typecheck(ctx->ctx);

		if (current_token == TOKEN_COMMA) {
			_actuals.push_back(exp);
			get_next();
			continue;
		}

		if (current_token != TOKEN_CPAREN) {
			exit_with_message("Closing parenthesis expected");
		} else {
			_actuals.push_back(exp);
			break;
		}

	}

	if (p != NULL) {
		ret = new CallExpression(p, _actuals);
	} else {

		if (ctx ->proc_name.compare(funcName) == 0) {
			ret = new CallExpression(ctx->proc_name, _actuals, true, ctx->type);
		}
	}

	return ret;
}

Expression *Parser::call_expression(ProcedureBuilder *ctx) {
	current_token = get_token();
	return expr(ctx);
}

Expression* Parser::bexpr(ProcedureBuilder *ctx) {
	Token l_token;
	Expression *retValue = lexpr(ctx);
	while (current_token == TOKEN_AND || current_token == TOKEN_OR) {
		l_token = current_token;
		current_token = get_next();
		Expression *e2 = lexpr(ctx);
		retValue = new LogicalExpression(retValue, e2, l_token);
	}

	return retValue;

}
Expression* Parser::lexpr(ProcedureBuilder *ctx) {
	Token l_token;
	Expression *ret = expr(ctx);

	while (current_token == TOKEN_GREATER_THAN || current_token
			== TOKEN_LESS_THAN || current_token == TOKEN_GREATER_EQUAL
			|| current_token == TOKEN_LESS_EQUAL || current_token
			== TOKEN_NEQUAL || current_token == TOKEN_EQUAL) {
		l_token = current_token;
		current_token = get_next();
		Expression *e2 = expr(ctx);
		RelationalOperator relop = get_relation_operator(l_token);
		ret = new RelationalExpression(ret, e2, relop);

	}
	return ret;

}

Expression *Parser::expr(ProcedureBuilder *ctx) {
	Token l_token;
	Expression *result = term(ctx);

	while (current_token == TOKEN_PLUS || current_token == TOKEN_MINUS) {
		l_token = current_token;
		current_token = get_token();
		Expression *e1 = expr(ctx);
		if (l_token == TOKEN_PLUS) {
			result = new BinaryPlus(result, e1);
		} else {
			result = new BinaryMinus(result, e1);
		}

	}

	return result;
}

Expression *Parser::term(ProcedureBuilder *ctx) {
	Token l_token;
	Expression *result = factor(ctx);

	while (current_token == TOKEN_MUL || current_token == TOKEN_DIV) {
		l_token = current_token;
		current_token = get_token();
		Expression *e1 = term(ctx);
		if (l_token == TOKEN_MUL) {
			result = new Mult(result, e1);
		} else {
			result = new Div(result, e1);
		}

	}

	return result;
}

Expression *Parser::factor(ProcedureBuilder *ctx) {
	Token l_token;
	Expression *result = NULL;

	if (current_token == TOKEN_NUMERIC) {
		result = new NumericConstant(get_number());
		current_token = get_token();
	} else if (current_token == TOKEN_STRING) {
		result = new StringLiteral(last_string);
		current_token = get_token();
	} else if (current_token == TOKEN_BOOL_TRUE || current_token
			== TOKEN_BOOL_FALSE) {
		result = new BooleanConstant(current_token == TOKEN_BOOL_TRUE ? true
				: false);
		current_token = get_token();
	} else if (current_token == TOKEN_NOT) {
		l_token = current_token;
		current_token = get_token();
		result = factor(ctx);
		result = new LogicalNot(result);
	}

	else if (current_token == TOKEN_OPAREN) {
		current_token = get_token();
		result = bexpr(ctx);
		if (current_token != TOKEN_CPAREN) {
			exit_with_message("Error : missing closing parenthesis");
		}
		current_token = get_token();
	} else if (current_token == TOKEN_PLUS || current_token == TOKEN_MINUS) {
		l_token = current_token;
		current_token = get_token();
		result = factor(ctx);

		if (l_token == TOKEN_PLUS) {
			result = new UnaryPlus(result);
		} else {
			result = new UnaryMinus(result);
		}

	} else if (current_token == TOKEN_UNQUOTED_STRING) {
		if (module_builder->is_function(last_string)) {
			Procedure *p = module_builder->get_procedure(last_string);
			Expression *e = parse_callproc(ctx, p);
			get_next();
			return e;
		} else {
			SymbolInfo *info = ctx->get_symbol(last_string);
			if (info == NULL) {
				exit_with_message("Undefined Symbol");
				return NULL;
			}

			get_next();
			result = new Variable(info);
		}

	} else {
		exit_with_message("Illegal token - ");
	}

	return result;
}

Token Parser::get_next() {
	last_token = current_token;
	current_token = get_token();
	return current_token;
}

vector<Statement*> Parser::parse(ProcedureBuilder *ctx) {
	get_next();
	return statement_list(ctx);

}
vector<Statement*> Parser::statement_list(ProcedureBuilder *ctx) {
	vector<Statement*> temp;

	while (current_token != TOKEN_NULL && current_token != TOKEN_ENDIF
			&& current_token != TOKEN_ENDWHILE && current_token != TOKEN_ELSE
			&& current_token != TOKEN_END) {
		Statement *st = get_statement(ctx);
		if (st != NULL) {
			temp.insert(temp.end(), st);
		}

	}

	return temp;
}

Statement *Parser::get_statement(ProcedureBuilder *ctx) {
	Statement *st = NULL;
	switch (current_token) {
	case TOKEN_VAR_STRING:
	case TOKEN_VAR_NUMBER:
	case TOKEN_VAR_BOOL:
		st = parse_variabledcl_statement(ctx);
		get_next();
		break;
	case TOKEN_PRINT:
		st = parse_print_statement(ctx);
		get_next();
		break;
	case TOKEN_PRINTLN:
		st = parse_printline_statement(ctx);
		get_next();
		break;
	case TOKEN_IF:
		st = parse_if_statement(ctx);
		get_next();
		break;
	case TOKEN_WHILE:
		st = parse_while_statement(ctx);
		get_next();
		break;

	case TOKEN_UNQUOTED_STRING:
		st = parse_unquoted_string(ctx);
		get_next();
		break;
	case TOKEN_RETURN:
		st = parse_return_statement(ctx);
		get_next();
		break;
	default:
		exit_with_message("Exception in statement");
		get_next();
		break;

	}
	return st;
}

Statement * Parser::parse_unquoted_string(ProcedureBuilder *ctx) {
	Statement *st = NULL;
	string var = last_string;
	SymbolInfo *info = ctx->get_symbol(var);

	if (info == NULL) {
		st = parse_call_statement(ctx);

		if (st == NULL) {
			exit_with_message("Not a valid statement");
		}

	} else {
		st = parse_assignment_statement(ctx);
	}

	return st;
}
Statement *Parser::parse_call_statement(ProcedureBuilder *ctx) {
	if (module_builder->is_function(last_string)) {
		Procedure *p = module_builder->get_procedure(last_string);
		Expression *e = parse_callproc(ctx, p);
		get_next();

		if (current_token != TOKEN_SEMI) {
			exit_with_message("\n; is expected\n");
		}

		CallStatement *st = new CallStatement(e);
		return st;

	}

	return NULL;

}
Statement *Parser::parse_return_statement(ProcedureBuilder *ctx) {
	Statement *st = NULL;
	get_next();
	Expression *e = bexpr(ctx);

	if (e == NULL) {
		exit_with_message("Error in return statement");
	}

	ctx->type_check(e);
	st = new ReturnStatement(e);
	return st;
}

Statement *Parser::parse_print_statement(ProcedureBuilder *ctx) {
	get_next();
	Expression *e = bexpr(ctx);
	e->typecheck(ctx->ctx);

	if (current_token != TOKEN_SEMI) {
		exit_with_message("\n; is expected\n");
	}
	PrintStatement *st = new PrintStatement(e);
	return st;
}
Statement *Parser::parse_printline_statement(ProcedureBuilder *ctx) {
	get_next();
	Expression *e = bexpr(ctx);
	e->typecheck(ctx->ctx);

	if (current_token != TOKEN_SEMI) {
		exit_with_message("\n; is expected\n");
	}

	PrintLineStatement *st = new PrintLineStatement(e);
	return st;
}
Statement *Parser::parse_variabledcl_statement(ProcedureBuilder *ctx) {
	Token token = current_token;
	get_next();

	if (current_token == TOKEN_UNQUOTED_STRING) {
		SymbolInfo *info = new SymbolInfo();
		info->symbol_name = last_string;
		if (token == TOKEN_VAR_BOOL) {
			info->type = TYPE_BOOL;
		} else if (token == TOKEN_VAR_NUMBER) {
			info->type = TYPE_NUMERIC;
		} else if (token == TOKEN_VAR_STRING) {
			info->type = TYPE_STRING;
		}
		get_next();

		if (current_token == TOKEN_SEMI) {
			ctx->add_symbol(info);
			return new VariableDeclStatement(info);
		} else {
			exit_with_message("Expected ; at the end of statement");
		}

	} else {
		exit_with_message("invalid variable declaration");
	}
	return NULL;
}
Statement *Parser::parse_assignment_statement(ProcedureBuilder *ctx) {
	string var = last_string;

	SymbolInfo *info = ctx->get_symbol(var);

	if (info == NULL) {
		exit_with_message("Error - variable not found");
		return NULL;
	}

	get_next();

	if (current_token != TOKEN_ASSIGN) {
		exit_with_message(" = expected");
		return NULL;
	}

	get_next();
	Expression *exp = bexpr(ctx);
	if (exp->typecheck(ctx->ctx) != info->type) {
		cout << "var type is " << info->type << "func type is"
				<< exp->typecheck(ctx->ctx) << "\n";
		exit_with_message("Type Mismatch");
	}

	if (current_token != TOKEN_SEMI) {
		exit_with_message("Missing ; symbol");
		return NULL;
	}

	if (info->type == TYPE_STRING) {
		/// compute string expression like '+' in compile time . store the result in string_val of info

		string str = exp->evaluate_string(NULL);
		info->string_val = str;
	}

	return new AssignmentStatement(info, exp);
}

Statement *Parser::parse_if_statement(ProcedureBuilder *ctx) {
	get_next();
	vector<Statement *> true_part;
	vector<Statement *> false_part;

	Expression *exp = bexpr(ctx);

	if (exp->typecheck(ctx->ctx) != TYPE_BOOL) {
		exit_with_message("Expected boolean expression");
	}

	if (current_token != TOKEN_THEN) {
		exit_with_message("THEN expected");
	}

	get_next();
	true_part = statement_list(ctx);

	if (current_token == TOKEN_ENDIF) {
		return new IfStatement(exp, true_part, false_part);
	}

	if (current_token != TOKEN_ELSE) {
		exit_with_message("ELSE expected");
	}

	get_next();
	false_part = statement_list(ctx);

	if (current_token == TOKEN_ENDIF) {
		return new IfStatement(exp, true_part, false_part);
	}

	exit_with_message("ENDIF expected");
}

Statement *Parser::parse_while_statement(ProcedureBuilder *ctx) {
	get_next();
	vector<Statement *> stmts;
	Expression *exp = bexpr(ctx);

	if (exp->typecheck(ctx->ctx) != TYPE_BOOL) {
		exit_with_message("Expected boolean expression");
	}

	stmts = statement_list(ctx);

	if (current_token != TOKEN_ENDWHILE) {
		exit_with_message("WEND expected");
	}
	return new WhileStatement(exp, stmts);
}

