#include "codegen.h"
#include	"parser.h"
#include "context.h"
#include "builder.h"

typedef enum{
  SlangCCModeCompile,
  SlangCCModeJIT
}SlangCCMode;

using namespace std;
using namespace CodeGen;
using namespace llvm;

static char *name;
static SlangCCMode mode;

void show_usage() {
  cout << "Usage : slangcc [Option] <filename>" <<"\n\n"
  << " -i  - JIT mode" <<"\n"
  << " -c  - Compile mode" <<"\n"
  << " -h  - help" <<"\n";

}
void parse_options(int argc , char **argv) {
  
  if(argc == 2) {
    name = argv[1];
    mode = SlangCCModeCompile;
  }
  else if(argc == 3) {
    if(strcmp(argv[1],"-i") == 0) {
      mode = SlangCCModeJIT;
    }
    else if(strcmp(argv[1],"-c") == 0) {
      mode = SlangCCModeCompile;
    }
    else if(strcmp(argv[1],"-h") == 0) {
      show_usage();
      exit(0);
    }
    else  {
      show_usage();
      exit(0);
    }
    
    name = argv[2];
  }
  
  else {
    show_usage();
    exit(0);
  }

}

int main(int argc, char **argv) {

    std::string cmd;
  
    parse_options(argc,argv);

    FILE *fd = fopen(name, "r");

    //name without .ext
    int lastindex = std::string(name).find_last_of(".");
    string name_only = std::string(name).substr(0, lastindex);

    if (fd != NULL) {

        //find file size
        fseek(fd, 0, SEEK_END);
        int len = ftell(fd);
        rewind(fd);

        //allocate memory for program text
        char *buffer = (char *) malloc(len);

        if (buffer == NULL) {
            fputs("Memory error", stderr);
            return 0;
        }
      
        //read program text
        int size = fread(buffer, 1, len, fd);
        string program_str(buffer);
      
        //parse , get all statements
        Parser *p = new Parser(program_str);
        Tmodule *mod = p->do_parse();

        if (mod == NULL) {
            exit_with_message("Error while parsing functions");
        }

      if(mode == SlangCCModeJIT) {
        
        //Setup native target
        InitializeNativeTarget();
        
        //Create execution engine .
        Execution_Context *rc = new Execution_Context();
        
        //Create llvm data structures in memory.
        mod->codegen(rc);
        
        //start execution engine for JIT , run main function .
        ExecutionEngine* ee = EngineBuilder(module).create();
        Function *proc = rc->get_procedure("main");
        std::vector<GenericValue> noargs;
        GenericValue gv = ee->runFunction(proc, noargs);
        
        //cleanup
        ee->freeMachineCodeForFunction(proc);
        delete ee;
        delete rc;
        llvm_shutdown();
      }
      else {
        
        //generate top level code
        Execution_Context *rc = new Execution_Context();
        mod->codegen(rc);
        
        //write llvm IR to file (name_only.ll)
        string old_name = name_only;
        string new_name = std::string(name_only) +".ll";
        FILE *fd = fopen(new_name.c_str(),"w");
        
        raw_fd_ostream file(fileno(fd), 1,0);
        module->print(file, NULL);
        file.close();
        
        //compile to native assembly using 'llc' (name_only.s)
        old_name = new_name;
        new_name = std::string(name_only) +".s";
        
        cmd = std::string("llc ") + old_name + std::string(" -o ") + new_name;
        system(cmd.c_str());
        
        //generate native exe using gcc (name_only)
        old_name = new_name;
        new_name = name_only;
        
        cmd = std::string("clang ") +old_name + std::string(" -o ") + new_name;
        system(cmd.c_str());
        
      }

      delete p;
      free(buffer);

    }
  
    else {
      cout << "Error - Unable to open file.\n\n";
      show_usage();
    }
  
    return 0;
}
