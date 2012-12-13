#include "common.h"

// quits process , shows error message

void exit_with_message(const char *err_msg) {

	std::cout << "\n" << err_msg << "\n";
	exit(0);
}

// returns 'RelationalOperator' corresponding to 'Token'

RelationalOperator get_relation_operator(Token token) {
	if (token == TOKEN_EQUAL) {
		return OPTR_EQUAL;
	} else if (token == TOKEN_NEQUAL) {
		return OPTR_NEQUAL;
	} else if (token == TOKEN_GREATER_THAN) {
		return OPTR_GREATER_THAN;
	} else if (token == TOKEN_GREATER_EQUAL) {
		return OPTR_GREATER_EQUAL;
	} else if (token == TOKEN_LESS_THAN) {
		return OPTR_LESS_THAN;
	} else if (token == TOKEN_LESS_EQUAL) {
		return OPTR_LESS_EQUAL;
	}

	return OPTR_ILLEGAL;
}
