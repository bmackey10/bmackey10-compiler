%{
#include "../parser/token.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
%}

DIGIT   [0-9]
ALPHA   [a-zA-Z]
ALPDIG  [a-zA-Z0-9]

%%

    /* comments */
"/*"([^/*]|"*"[^/])*("*/"|"**/")    /* do nothing for comment */
"//"[^\n]*                          /* do nothing for comment */

    /* keywords */
boolean|char|float|integer|string                       { return TOKEN_TYPE; }
function                                                { return TOKEN_FUNC; }
array                                                   { return TOKEN_ARR; }
void                                                    { return TOKEN_VOID; }
true|false                                              { return TOKEN_BOOL; }   
if                                                      { return TOKEN_IF; }
else                                                    { return TOKEN_ELSE; }  
for                                                     { return TOKEN_FOR; }  
while                                                   { return TOKEN_WHILE; }
print                                                   { return TOKEN_PRINT; }
return                                                  { return TOKEN_RETURN; }                                        
auto                                                    { return TOKEN_AUTO; }

    /* identifiers */
({ALPHA}|_)({ALPDIG}|_)*    { return TOKEN_IDENTIFIER; }

    /* integers and floats */
        [1-9]{DIGIT}*|0                 {    char *end;
                                            long int temp_int = strtoll(yytext, &end, 10);
                                            if ((LONG_MIN == temp_int || LONG_MAX == temp_int) && ERANGE == errno) {
                                                printf("ERROR: %s out of range of type long.", yytext);
                                                return TOKEN_ERROR;
                                            } else {
                                                return TOKEN_INTEGER;
                                            }
                                        }
{DIGIT}*"."{DIGIT}*[eE][+-]?{DIGIT}*    { return TOKEN_FLOAT; }
{DIGIT}*[eE][+-]?{DIGIT}*               { return TOKEN_FLOAT; }
{DIGIT}*"."{DIGIT}+                     {    char *end;
                                            double temp_float = strtod(yytext, &end);
                                            if ((HUGE_VAL == temp_float || -HUGE_VAL == temp_float) && ERANGE == errno) {
                                                printf("ERROR: %s out of range of type long.", yytext);
                                                return TOKEN_ERROR;
                                            } else {
                                                return TOKEN_FLOAT;
                                            }
                                        }

    /* characters and strings */
'(.|(\\.)|(\\0x{ALPDIG}{2}))'   { return TOKEN_CHAR; }
\"([^\\"\n]|(\\.))*\"           { return TOKEN_STRING; }

    /* operators */
":"     { return TOKEN_COLON; }
";"     { return TOKEN_SEMICOLON; }
","     { return TOKEN_COMMA; }
"["     { return TOKEN_OPEN_BRACKET; }
"]"     { return TOKEN_CLOSE_BRACKET; }
"{"     { return TOKEN_OPEN_BRACE; }
"}"     { return TOKEN_CLOSE_BRACE; }
"("     { return TOKEN_OPEN_PARENS; }
")"     { return TOKEN_CLOSE_PARENS; }
"++"    { return TOKEN_INC; }
"--"    { return TOKEN_DEC; }
"="     { return TOKEN_ASSIGN; }
"^"     { return TOKEN_EXPONENT; }
"+"     { return TOKEN_ADD; }
"-"     { return TOKEN_SUBTRACT; }
"*"     { return TOKEN_MULTIPLY; }
"/"     { return TOKEN_DIVIDE; }
"%"     { return TOKEN_MOD; }
"=="    { return TOKEN_EQUALITY; }
">="    { return TOKEN_GE; }
"<="    { return TOKEN_LE; }
"<"     { return TOKEN_LESS; }
">"     { return TOKEN_GREATER; }
"!="    { return TOKEN_INEQUALITY; }
"!"     { return TOKEN_NOT; }
"&&"    { return TOKEN_AND; }
"||"    { return TOKEN_OR; }

    /* whitespace */
[ \t\n\r] /* do nothing for whitespace */

    /* Unrecognized */
.   { printf("ERROR: Unrecognized token.");
        return TOKEN_ERROR; 
    }

%%

int yywrap() { return 1; }