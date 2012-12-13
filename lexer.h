#include "common.h"
#include "ast.h"
#include "ast_statement.h"
using namespace std;

#define KEYWORDS_COUNT 16

class ValueTable {
public:
	Token token;
	string value;
	ValueTable(Token _token, string _value) {
		token = _token;
		value = _value;
	}
};

class Lexer {
private:
	double grab_number();
	ValueTable *value_table[KEYWORDS_COUNT];
	void skip_to_EOL();

public:
	string input_string;
	int index;
	int length;
	double number;
	Token current_token;
	Token last_token;
	string last_string;
	Lexer(string input_str);
	Token get_token();
	double get_number();

};


