#include "compilation_unit.h"
#include "ast_statement.h"
#include "codegen.h"

using namespace CodeGen;

Procedure::Procedure(string _name, vector<SymbolInfo *> _formals, vector<
                     Statement *> stats, SymbolInfoTable *_locals, TypeInfo _type) {

    name = _name;
    formals = _formals;
    statements = stats;
    locals = _locals;
    type = _type;

    /// initialize symbolinfo for return value .
    return_val  = new SymbolInfo();
    return_val->symbol_name = "";
    return_val->type = type;
}

SymbolInfo * Procedure::execute(Runtime_Context *ctx,
                                vector<SymbolInfo *> actuals) {

    // add all arguments to symbol table

    if (actuals.size() == formals.size()) {
        for (int j = 0; j < formals.size(); ++j) {
            SymbolInfo * info_actual = actuals.at(j);
            SymbolInfo * info_formal = formals.at(j);

            info_actual->symbol_name = info_formal->symbol_name;
            ctx->add_symbol(info_actual);
        }
    }

    // execute statements

    for (int j = 0; j < statements.size(); ++j) {
        Statement *st = statements.at(j);
        return_val = st->execute(ctx);

        if (return_val != NULL) {
            return return_val;
        }
    }

    return NULL;
}

TypeInfo Procedure::typecheck(Compilation_Context *ctx) {
    return TYPE_NUMERIC;
}


void Procedure::update_return_value(Value *val) {
    builder.CreateStore(val, ret_alloca);
}

Function * Procedure::codegen(Execution_Context *ctx) {
    vector<Type *> args;


    for(int i=0;i<formals.size();++i) {
        SymbolInfo *info = formals.at(i);
        Type *type = llvm_type_from_symboltype(info->type);

        if(type != NULL) {
            args.push_back(type);
        }
        else {
            exit_with_message("Invalid type in function");
        }
    }

    vector<Type *> llargs(args);

    Type *ret_type = llvm_type_from_symboltype(type);
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);

    // emit function header
    Function *func = emit_function_block(name.c_str(), llargs, ret_type);

    // Set names for all arguments.
    unsigned i = 0;
    for (Function::arg_iterator ai = func->arg_begin(); i != args.size();
         ++ai, ++i) {
        SymbolInfo *inf = formals.at(i);
        ai->setName(inf->symbol_name);
    }

    exitBB = BasicBlock::Create(getGlobalContext(), "exit",
                                func);
    //
    // ReturnStatement will update return value via execution context .
    // Allocate space for return value

    ret_alloca = emit_stack_variable(return_val);

    // allocate space for parameters
    i = 0;
    for (Function::arg_iterator ai = func->arg_begin(); i != args.size();++ai,++i) {
        SymbolInfo *info = formals.at(i);
        AllocaInst *alcInst = emit_stack_variable(info);
        builder.CreateStore(ai,alcInst);
        ctx->add_symbol(info->symbol_name, alcInst);
    }

    /// insert function into function table
    ctx->add_procedure(name, func);

    // generate code for all statements
    bool skip_branch;
    for(int i=0;i<statements.size();++i) {
        Statement *st = statements.at(i);
        st->codegen(ctx);
    }

    builder.CreateBr(exitBB);

    // emit return block

    builder.SetInsertPoint(exitBB);
    builder.CreateRet(builder.CreateLoad(ret_alloca));

    return func;
}

BasicBlock * Procedure::exitblock()
{
    return exitBB;
}

Tmodule::Tmodule(vector<Procedure *> _procs) {
    procs = _procs;
}

Procedure * Tmodule::find_procedure(string name) {
    for (int i = 0; i < procs.size(); ++i) {
        Procedure *proc = procs.at(i);
        string name1 = proc->name;

        std::transform(name1.begin(), name1.end(), name1.begin(), ::tolower);
        std::transform(name.begin(), name.end(), name.begin(), ::tolower);

        if (name.compare(name1) == 0) {
            return proc;
        }
    }

    return NULL;
}

SymbolInfo * Tmodule::execute(Runtime_Context *ctx,
                              vector<SymbolInfo *> _actuals) {
    Procedure *_main = find_procedure("main");
    if (_main != NULL) {
        return _main->execute(ctx, _actuals);
    }

    return NULL;
}

Value * Tmodule::codegen(Execution_Context *ctx) {

    for (int i = 0; i < procs.size(); ++i) {

        Procedure *proc = procs.at(i);

        // set current procedure in ctx , this will also clear symbol table
        ctx->set_current_procedure(proc);

        // function codegen
        Function *func = proc->codegen(ctx);

    }

}
