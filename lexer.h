#include "common.h"
#include "ast.h"
#include "ast_statement.h"
using namespace std;

class ValueTable
{
public:
   Token token;
   string value;
   ValueTable(Token _token, string _value) { token = _token; value= _value; }
};

class Lexer
{
private:
   double grab_number();
   ValueTable *value_table[7];

protected:
   Token current_token;
   Token last_token;
   string last_string;

public:
   string   input_string;
   int      index;
   int      length;
   double   number;             
 
   Lexer(string input_str);  
   Token    get_token();
   double   get_number();      

};


