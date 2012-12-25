all: codegen jsrun common context symboltable ast ast_statement lexer parser cunit builder
	ar -rvs libslang.a ast.o ast_statement.o lexer.o parser.o codegen.o common.o context.o symboltable.o compilation_unit.o builder.o js_run.o 
	g++  slangcc.cc  -L $(PWD) -lslang -L $(PWD)/external/lib -lv8_base -lv8_snapshot -o slangcc `llvm-config --libs core jit interpreter nativecodegen --cppflags` `llvm-config --ldflags`
builder:builder.cc
	g++ -c builder.cc  -o builder.o `llvm-config --cxxflags`

cunit:compilation_unit.cc
	g++ -c compilation_unit.cc -o compilation_unit.o `llvm-config --cxxflags`

common:common.cc
	g++ -c common.cc -o common.o `llvm-config --cxxflags`

codegen:codegen.cc
	g++ -c codegen.cc -o codegen.o `llvm-config --cxxflags`

context: context.cc
	g++ -c context.cc -o context.o `llvm-config --cxxflags`

symboltable: symbol_table.cc
	g++ -c symbol_table.cc -o symboltable.o `llvm-config --cxxflags`

ast:ast.cc
	g++ -c ast.cc -o ast.o `llvm-config --cxxflags`

ast_statement:ast_statement.cc
	g++ -c ast_statement.cc -o ast_statement.o `llvm-config --cxxflags`

parser:parser.cc
	g++ -c parser.cc -o parser.o `llvm-config --cxxflags`

lexer:lexer.cc
	g++ -c lexer.cc -o lexer.o `llvm-config --cxxflags`

jsrun:js_run.cc
	g++ -c js_run.cc -o js_run.o `llvm-config --cxxflags`

slangcc:slangcc.cc

	g++ -g -O3 slangcc.cc `llvm-config --libs core --cppflags` `llvm-config --ldflags`  -L $(PWD) -lslang -o slangcc

clean:
	rm -rf *.o *.a
