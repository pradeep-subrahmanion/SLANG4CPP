#include "lexer.h"

double Lexer::grab_number()
{

   char c = input_string.at(index);
   string str = "";
   str += c;

   while(++index < length) {

      c = input_string.at(index);     

      if(c == '0' || c == '1' || c == '2' ||
         c == '3' || c == '4' || c == '5' ||
         c == '6' || c == '7' || c == '8' ||
         c == '9') {
           str += c; 
         }
      else {
         break;
      }
   }

  return atof(str.c_str());
}

void Lexer::skip_to_EOL()
{
  while (input_string.at(index)!='\n' && index < length){
    index++;
  } 
   //exception
}

Lexer::Lexer(string input_str)
{
   index    = 0;
   length   = input_str.length();
   input_string = input_str;

   value_table[0] = new ValueTable(TOKEN_PRINT,"PRINT");
   value_table[1] = new ValueTable(TOKEN_PRINTLN,"PRINTLINE");
   value_table[2] = new ValueTable(TOKEN_BOOL_FALSE,"FALSE");
   value_table[3] = new ValueTable(TOKEN_BOOL_TRUE,"TRUE");
   value_table[4] = new ValueTable(TOKEN_VAR_BOOL,"BOOLEAN");
   value_table[5] = new ValueTable(TOKEN_VAR_NUMBER,"NUMERIC");
   value_table[6] = new ValueTable(TOKEN_VAR_STRING,"STRING");
}

Token Lexer::get_token()
{
start:

   Token token = TOKEN_ILLEGAL;
   char next_char;
   string str = "";

   while(index < length && ((next_char = input_string.at(index)) == ' ' || next_char == '\t')) {
      index++;
   }
   //cout << "index = " << index << "char" << next_char;

   //check for end of string
   while(index == length) {
      return TOKEN_NULL;
   }

   switch(next_char) {
      case '\r':
      case '\n':
         index++;
         goto start;
      case '+':
         token = TOKEN_PLUS;
         index++;
         break;
      case '-':
         token = TOKEN_MINUS;
         index++;
         break;
      case '*':
         token = TOKEN_MUL;
         index++;
         break;
      case '(':
         token = TOKEN_OPAREN;
         index++;
         break;
      case ')':
         token = TOKEN_CPAREN;
         index++;
         break;
      case ';':
         token = TOKEN_SEMI;
         index++;
         break;
     case '/':
         if(input_string.at(index+1) == '/') {
            skip_to_EOL();
            goto start;
         }
         else {
           token = TOKEN_DIV;
           index++;
           
        }

        break;
     case '=':
         token = TOKEN_ASSIGN;
         index++;
         break;
     case '"':
         
        str = "";
        index++;
        next_char = input_string.at(index);
        while(index < length && next_char != '"') {
          str = str + next_char;
          index ++;
          next_char = input_string.at(index);
        }   

        if(index == length) {
         token = TOKEN_ILLEGAL;
         return token;
        }
      
        else {
         index ++;
         last_string = str;
         token = TOKEN_STRING;
         return token;
        }
       break;

      default :
         next_char = input_string.at(index);
         str = "";
         if(isdigit(next_char)) {
          while (index < length && (next_char == '0' ||
                  next_char == '1' || next_char == '2' ||   next_char == '3' ||
                   next_char == '4' || next_char == '5' || next_char == '6' ||
                            next_char == '7' ||
                            next_char == '8' ||
                            next_char== '9'))
                        {
                            str += next_char;
                            index++;
                            next_char = input_string.at(index);
                        }

                        if (next_char == '.')
                        {
                            str = str + ".";
                            index++;
                            next_char = input_string.at(index);
                            while (index < length && (next_char== '0' ||
                                next_char == '1' ||
                                next_char == '2' ||
                                next_char == '3' ||
                                next_char == '4' ||
                                next_char == '5' ||
                                next_char == '6' ||
                                next_char == '7' ||
                                next_char == '8' ||
                              next_char == '9'))
                            {
                                str += next_char;
                                index++;
                                 next_char = input_string.at(index);
                            }

                        }

                        number = atof(str.c_str());
                        token = TOKEN_NUMERIC;

         }

         else if(isalpha(next_char)) {
            str += next_char;

            while(++index < length && isalnum((next_char=input_string.at(index))) || next_char == '_' ) {
               str += next_char;

            }

            //convert to upper case
            std::transform(str.begin(), str.end(),str.begin(), ::toupper);
        
            for(int i=0; i<7; ++i) {

               if(str.compare(value_table[i]->value)==0) {
                  ValueTable *t = value_table[i];
                  return t->token;                  
               }
            }
            last_string = str;
            return TOKEN_UNQUOTED_STRING;
         }
         else {
            std::cout << "Error while analyzing tokens";
            break;
         }
 
     }

  return token;

}

double Lexer::get_number()
{
   return number;
}
