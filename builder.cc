#include "builder.h"
#include <string>
#include <iostream>
using namespace std;

ProcedureBuilder::ProcedureBuilder(string _name, Compilation_Context *_ctx) {
	proc_name = _name;
	ctx = _ctx;
}

void ProcedureBuilder::add_local(SymbolInfo *_info) {
	ctx->add_symbol(_info);
}

void ProcedureBuilder::add_formals(SymbolInfo *_info) {
	formals.push_back(_info);
}

TypeInfo ProcedureBuilder::type_check(Expression *exp) {
	return exp->typecheck(ctx);
}

void ProcedureBuilder::add_statement(Statement *stmt) {
	statements.push_back(stmt);
}

SymbolInfo *ProcedureBuilder::get_symbol(string name) {
	return ctx->get_symbol(name);
}

bool ProcedureBuilder::check_proto(string name) {
	return true;
}

Procedure * ProcedureBuilder::get_procedure() {
	return new Procedure(proc_name, formals, statements,
			ctx->get_symboltable(), type);
}

void ProcedureBuilder::add_symbol(SymbolInfo *info) {
	ctx->add_symbol(info);
}

TmoduleBuilder::TmoduleBuilder() {

}

bool TmoduleBuilder::is_function(string name) {
	for (int i = 0; i < protos.size(); ++i) {
		FunctionInfo *info = protos.at(i);
		if (info->name.compare(name) == 0) {
			return true;
		}
	}
	return false;

}

void TmoduleBuilder::add_function_prototypes(string _name, TypeInfo _ret_type,
		vector<TypeInfo> _formals) {
	FunctionInfo *info = new FunctionInfo(_name, _ret_type, _formals);
	protos.push_back(info);
}

bool TmoduleBuilder::check_function_prototypes(string _name,
		TypeInfo _ret_type, vector<TypeInfo> _formals) {
	for (int i = 0; i < protos.size(); ++i) {
		FunctionInfo *info = protos.at(i);
		if (info->name.compare(_name) == 0) {
			if (info->ret_type == _ret_type) {
				for (int j = 0; j < _formals.size(); ++j) {

					TypeInfo a = _formals.at(j);
					TypeInfo b = info->type_infos.at(j);

					if (a != b)
						return false;

				}
			}
		}
	}

	return true;
}

void TmoduleBuilder::add_procedure(Procedure *p) {
	procs.push_back(p);
}

Procedure *TmoduleBuilder::get_procedure(string name) {
	for (int i = 0; i < procs.size(); ++i) {
		Procedure *proc = procs.at(i);

		string name1 = proc->name;

		std::transform(name1.begin(), name1.end(), name1.begin(), ::tolower);
		std::transform(name.begin(), name.end(), name.begin(), ::tolower);

		if (name1.compare(name) == 0) {
			return proc;
		}
	}

	return NULL;
}

Tmodule *TmoduleBuilder::get_program() {
	return new Tmodule(procs);
}
