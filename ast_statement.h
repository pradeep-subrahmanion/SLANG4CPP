#include "ast.h"


class Statement
{
public:
   virtual bool execute() =0 ;
	virtual Value* codegen() = 0;

};

class PrintStatement: public Statement  
{
private:
   Expression *exp;

public:
   PrintStatement(Expression *_exp);
   ~PrintStatement();
   bool execute();
	virtual Value* codegen();

};

class PrintLineStatement: public Statement
{
private:
   Expression *exp;

public:
   PrintLineStatement(Expression *_exp);
   ~PrintLineStatement();
   bool execute();
	virtual Value* codegen();
};
