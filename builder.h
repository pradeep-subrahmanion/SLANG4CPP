#ifndef BUILDER_INCLUDED
#define BUILDER_INCLUDED

class Statement;

#include "common.h"
#include "context.h"
#include "ast.h"
#include "compilation_unit.h"

class Builder {

};

class ProcedureBuilder: Builder {

public:

	string proc_name;
	vector<SymbolInfo *> formals;
	vector<Statement *> statements;
	TypeInfo type;
	Compilation_Context *ctx;

	ProcedureBuilder(string _name, Compilation_Context *_ctx);
	void add_local(SymbolInfo *_info);
	void add_formals(SymbolInfo *_info);
	TypeInfo type_check(Expression *exp);
	void add_statement(Statement *stmt);
	SymbolInfo *get_symbol(string name);
	bool check_proto(string name);
	Procedure * get_procedure();
	void add_symbol(SymbolInfo *info);
};

class TmoduleBuilder: Builder {
	vector<Procedure *> procs;
	vector<FunctionInfo *> protos;

public:
	TmoduleBuilder();
	bool is_function(string name);
	void add_function_prototypes(string _name, TypeInfo _ret_type, vector<
			TypeInfo> _formals);
	bool check_function_prototypes(string _name, TypeInfo _ret_type, vector<
			TypeInfo> _formals);
	void add_procedure(Procedure *p);
	Procedure *get_procedure(string name);
	Tmodule *get_program();

};

#endif

