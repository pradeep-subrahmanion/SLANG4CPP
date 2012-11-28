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
  while (input_string.at(index)!='\n'){
    index++;
  } 
  return;
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

}

Token Lexer::get_token()
{
start:

   Token token = TOKEN_ILLEGAL;
   char next_char;
     
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
      case '/':
         token = TOKEN_DIV;
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
           break;
       }
     case '=':
         token = TOKEN_ASSIGN;
         index++;
         break;
     case '"':
        string str = "";
        index++;
        char c = input_string.at(index);
        while(index < length && c != '"') {
          str = str+c;
          index ++;
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
       

      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
         number = grab_number();
         token  = TOKEN_DOUBLE;
         break;
      default :
         char c = input_string.at(index);
         string str = "";
         if(isdigit(c)) {
          while (index < length && (c == '0' ||
                  c == '1' || c == '2' ||   c == '3' ||
                   c == '4' || c == '5' || c == '6' ||
                            c == '7' ||
                            c == '8' ||
                            c == '9'))
                        {
                            str += c;
                            index++;
                            c = input_string.at(index);
                        }

                        if (c == '.')
                        {
                            str = str + ".";
                            index++;
                            char c = input_string.at(index);
                            while (index < length && (c== '0' ||
                                c == '1' ||
                                c == '2' ||
                                c == '3' ||
                                c == '4' ||
                                c == '5' ||
                                c == '6' ||
                                c == '7' ||
                                c == '8' ||
                               c == '9'))
                            {
                                str += c;
                                index++;
                            }

                        }

                        number = atof(str);
                        tok = TOKEN_NUMERIC;

         }

         else if(isalpha(c)) {
            str += c;

            while(++index < length && isalnum((c=input_string.at(index))) || c == '_' ) {
               str += c;
            }

            //convert to upper case
            std::transform(str.begin(), str.end(),str.begin(), ::toupper);
        
            for(int i=0; i<2; ++i) {

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
