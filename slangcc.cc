#include "codegen.h"
#include	"parser.h"
#include "context.h"
#include "builder.h"

using namespace std;
using namespace CodeGen;

#define KInterpreterMode 1

int main(int argc, char **argv)
{
  
  if(argc < 2) {
    cout<< "Please enter input file\nUsage : slangcc <filename>\n";
    return 0;
  }
  
  std::string cmd;
  char *name = argv[1];
  FILE *fd = fopen(name,"r");
  
  // name without .ext
  
  int lastindex  = std::string(name).find_last_of(".");
  string name_only = std::string(name).substr(0, lastindex);
  
  if(fd != NULL) {
    
    // find file size
    
    fseek(fd,0,SEEK_END);
    int len = ftell(fd);
    rewind(fd);
    
    // allocate memory for program text
    
    char *buffer = (char *)malloc(len);
    
    if (buffer == NULL) {
      fputs ("Memory error",stderr);
      return 0;
    }
    
    int size = fread(buffer,1,len,fd);
    
    // read program text
    
    string program_str(buffer);
   // Compilation_Context *cc= new Compilation_Context();
    // parse , get all statements
        
    Parser *p = new Parser(program_str);
    Tmodule *mod= p->do_parse();

    if(mod == NULL ) {
      exit_with_message("Error while parsing functions");
    }
    
 #if KInterpreterMode   

    vector<SymbolInfo *> d;
    Runtime_Context *rc = new Runtime_Context(mod);
    mod->execute(rc,d);

 #else

    Procedure *proc = mod->find_procedure("MAIN");
    vector<Statement *> v = proc->statements;
     //generate top level code
     Execution_Context *rc = new Execution_Context();
     emit_top_level_code();
    
    // generate code for all statements


    for(int i=0;i<v.size();++i) {
      Statement *st = v.at(i);
      st->codegen(rc);
    }
 
    // generate return statement
    
     emit_ret_stmt();
    
    // write llvm IR to file (name_only.ll)
    
    string old_name = name_only;
    string new_name = std::string(name_only) +".ll";
    FILE *fd = fopen(new_name.c_str(),"w");
    
    raw_fd_ostream file(fileno(fd), 1,0);
    module->print(file, NULL);
    file.close();
    
    // compile to native assembly using 'llc' (name_only.s)
    old_name = new_name;
    new_name = std::string(name_only) +".s";
    
    cmd = std::string("llc ") + old_name + std::string(" -o ") + new_name;
    system(cmd.c_str());
    
    // generate native exe using gcc (name_only)
    old_name = new_name;
    new_name = name_only;
    
    cmd = std::string("gcc ") +old_name + std::string(" -o ") + new_name;
    system(cmd.c_str());
#endif    
    delete p;
    free(buffer);

    
  }
  return 0;
}
