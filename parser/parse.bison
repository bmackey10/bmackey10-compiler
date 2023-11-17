%{
    #include <stdio.h>
    #include "../decl.h"
    #include "../type.h"

    extern char *yytext;
    extern int yylex();
    extern int yyerror( char *str );

    struct decl *program;
%}

%union {
        struct decl *decl;
        struct expr *expr;
        struct stmt *stmt;
        struct type *type;
        struct param_list *param_list;
        char *name;
}
%type <decl> decl_list decl var_decl func_decl
%type <expr> expr opt_expr opt_expr_list expr_list or_expr and_expr not_expr rel_expr add_expr mul_expr exp_expr pre_expr post_expr base_expr arr_index
%type <stmt> stmt_list opt_stmt_list stmt cmp_stmt open_cmp_stmt closed_cmp_stmt other print return
%type <type> decl_type func_type param_type
%type <param_list> param_list opt_param_list
%type <name> ident string TOKEN_IDENTIFIER TOKEN_INTEGER TOKEN_FLOAT TOKEN_BOOL TOKEN_CHAR TOKEN_STRING
%token TOKEN_EOF 0
%token TOKEN_COMMENT
%token TOKEN_IDENTIFIER
%token TOKEN_INTEGER
%token TOKEN_FLOAT
%token TOKEN_CHAR
%token TOKEN_STRING
%token TOKEN_COLON
%token TOKEN_SEMICOLON
%token TOKEN_COMMA
%token TOKEN_OPEN_BRACKET
%token TOKEN_CLOSE_BRACKET
%token TOKEN_OPEN_BRACE
%token TOKEN_CLOSE_BRACE
%token TOKEN_OPEN_PARENS
%token TOKEN_CLOSE_PARENS
%token TOKEN_INC
%token TOKEN_DEC
%token TOKEN_ASSIGN
%token TOKEN_EXPONENT
%token TOKEN_ADD
%token TOKEN_SUBTRACT
%token TOKEN_MULTIPLY
%token TOKEN_DIVIDE
%token TOKEN_MOD
%token TOKEN_EQUALITY
%token TOKEN_GE
%token TOKEN_LE
%token TOKEN_LESS
%token TOKEN_GREATER
%token TOKEN_INEQUALITY
%token TOKEN_NOT
%token TOKEN_AND
%token TOKEN_OR
%token TOKEN_ERROR
%token TOKEN_TYPE
%token TOKEN_FUNC
%token TOKEN_ARR
%token TOKEN_VOID
%token TOKEN_BOOL
%token TOKEN_IF
%token TOKEN_ELSE
%token TOKEN_FOR
%token TOKEN_WHILE
%token TOKEN_PRINT
%token TOKEN_RETURN
%token TOKEN_AUTO
%token TOKEN_TYPE_BOOL
%token TOKEN_TYPE_STRING
%token TOKEN_TYPE_CHAR
%token TOKEN_TYPE_FLOAT                    
%token TOKEN_TYPE_INT

%%

program     : decl_list TOKEN_EOF { program = $1;}
            ;

decl_list       : decl decl_list { $$ = decl_create_list($1, $2); }
                | %empty { $$ = NULL; }
                ;

decl        : var_decl { $$ = $1; }
            | func_decl { $$ = $1; }
            ;

var_decl    : ident TOKEN_COLON decl_type TOKEN_SEMICOLON { $$ = decl_create($1, $3, NULL, NULL, NULL); }
            | ident TOKEN_COLON decl_type TOKEN_ASSIGN expr TOKEN_SEMICOLON { $$ = decl_create($1, $3, $5, NULL, NULL); }
            ;

func_decl   : ident TOKEN_COLON func_type TOKEN_SEMICOLON { $$ = decl_create($1, $3, NULL, NULL, NULL); }
            | ident TOKEN_COLON func_type TOKEN_ASSIGN TOKEN_OPEN_BRACE opt_stmt_list TOKEN_CLOSE_BRACE { $$ = decl_create_func($1, $3, $6 ); }
            ;

decl_type   : TOKEN_TYPE_BOOL { $$ = type_create(TYPE_BOOLEAN, NULL, NULL); }
            | TOKEN_TYPE_INT { $$ = type_create(TYPE_INTEGER, NULL, NULL); }
            | TOKEN_TYPE_FLOAT { $$ = type_create(TYPE_FLOAT, NULL, NULL); }
            | TOKEN_TYPE_STRING { $$ = type_create(TYPE_STRING, NULL, NULL); }
            | TOKEN_TYPE_CHAR { $$ = type_create(TYPE_CHARACTER, NULL, NULL); }
            | TOKEN_VOID { $$ = type_create(TYPE_VOID, NULL, NULL); }
            | TOKEN_ARR TOKEN_OPEN_BRACKET expr TOKEN_CLOSE_BRACKET decl_type { $$ = type_array_create(TYPE_ARRAY, $3, $5 ); }
            ;

param_type  : TOKEN_TYPE_BOOL { $$ = type_create(TYPE_BOOLEAN, NULL, NULL); }
            | TOKEN_TYPE_INT { $$ = type_create(TYPE_INTEGER, NULL, NULL); }
            | TOKEN_TYPE_FLOAT { $$ = type_create(TYPE_FLOAT, NULL, NULL); }
            | TOKEN_TYPE_STRING { $$ = type_create(TYPE_STRING, NULL, NULL); }
            | TOKEN_TYPE_CHAR { $$ = type_create(TYPE_CHARACTER, NULL, NULL); }
            | TOKEN_VOID { $$ = type_create(TYPE_VOID, NULL, NULL); }
            | TOKEN_ARR TOKEN_OPEN_BRACKET opt_expr TOKEN_CLOSE_BRACKET decl_type { $$ = type_array_create(TYPE_ARRAY, $3, $5 ); }
            ;

func_type   : TOKEN_FUNC param_type TOKEN_OPEN_PARENS opt_param_list TOKEN_CLOSE_PARENS { $$ = type_function_create(TYPE_FUNCTION, $2, $4 ); }
            ;

opt_param_list  : param_list { $$ = $1; }
                | %empty { $$ = NULL; }

param_list  : ident TOKEN_COLON param_type TOKEN_COMMA param_list { $$ = param_list_create($1, $3, $5); }
            | ident TOKEN_COLON param_type { $$ = param_list_create($1, $3, NULL); }
            ;

expr    : expr TOKEN_ASSIGN or_expr { $$ = expr_create(EXPR_ASSIGN, $1, $3); }
        | or_expr { $$ = $1; }
        ;

opt_expr    : %empty { $$ = NULL; }
            | expr { $$ = $1; } 
            ;

opt_expr_list   : %empty { $$ = NULL; }
                | expr_list { $$ = $1; }
                ;

expr_list   : expr TOKEN_COMMA expr_list { $$ = expr_create_list($1, $3); }
            | expr { $$ = expr_create_list($1, NULL); }
            ;

or_expr     : or_expr TOKEN_OR and_expr { $$ = expr_create(EXPR_OR, $1, $3); }
            | and_expr { $$ = $1; }
            ;

and_expr    : and_expr TOKEN_AND not_expr { $$ = expr_create(EXPR_AND, $1, $3); }
            | not_expr { $$ = $1; }
            ;

not_expr    : TOKEN_NOT not_expr { $$ = expr_create(EXPR_NOT, $2, NULL); }
            | rel_expr { $$ = $1; }
            ;
            
rel_expr    : rel_expr TOKEN_GE add_expr { $$ = expr_create(EXPR_GE, $1, $3); }
            | rel_expr TOKEN_LE add_expr { $$ = expr_create(EXPR_LE, $1, $3); }
            | rel_expr TOKEN_GREATER add_expr { $$ = expr_create(EXPR_GREATER, $1, $3); }
            | rel_expr TOKEN_LESS add_expr { $$ = expr_create(EXPR_LESS, $1, $3); }
            | rel_expr TOKEN_EQUALITY add_expr { $$ = expr_create(EXPR_EQUALITY, $1, $3); }
            | rel_expr TOKEN_INEQUALITY add_expr { $$ = expr_create(EXPR_INEQUALITY, $1, $3); }
            | add_expr { $$ = $1; }
            ;

add_expr    : add_expr TOKEN_ADD mul_expr { $$ = expr_create(EXPR_ADD, $1, $3); }
            | add_expr TOKEN_SUBTRACT mul_expr { $$ = expr_create(EXPR_SUBTRACT, $1, $3); }
            | mul_expr { $$ = $1; }
            ;

mul_expr    : mul_expr TOKEN_MULTIPLY exp_expr { $$ = expr_create(EXPR_MULTIPLY, $1, $3); }
            | mul_expr TOKEN_DIVIDE exp_expr { $$ = expr_create(EXPR_DIVIDE, $1, $3); }
            | mul_expr TOKEN_MOD exp_expr { $$ = expr_create(EXPR_MOD, $1, $3); }
            | exp_expr { $$ = $1; }
            ;

exp_expr    : exp_expr TOKEN_EXPONENT pre_expr { $$ = expr_create(EXPR_EXP, $1, $3); }
            | pre_expr { $$ = $1; }
            ;

pre_expr    : post_expr  { $$ = $1; }
            | TOKEN_ADD pre_expr { $$ = expr_create(EXPR_NEG, $2, NULL); }
            | TOKEN_SUBTRACT pre_expr { $$ = expr_create(EXPR_POS, $2, NULL); }
            ;

post_expr   : base_expr { $$ = $1; }
            | post_expr TOKEN_INC { $$ = expr_create(EXPR_INC, $1, NULL); }
            | post_expr TOKEN_DEC { $$ = expr_create(EXPR_DEC, $1, NULL); }
            ;

base_expr   : TOKEN_OPEN_PARENS expr TOKEN_CLOSE_PARENS { $$ = $2; }
            | TOKEN_INTEGER { $$ = expr_create_integer_literal(atoi(yytext)); }
            | TOKEN_FLOAT { $$ = expr_create_float_literal(parse_float(yytext));}
            | ident { $$ = expr_create_name($1); }
            | TOKEN_CHAR { $$ = expr_create_char_literal(parse_char(yytext)); }
            | string { $$ = expr_create_string_literal(parse_string($1)); }
            | TOKEN_BOOL { $$ = expr_create_boolean_literal(yytext); }
            | ident TOKEN_OPEN_PARENS opt_expr_list TOKEN_CLOSE_PARENS { $$ = expr_create_func_call(expr_create_name($1), $3); }
            | ident arr_index { $$ = expr_create_arr_subscript(expr_create_name($1), $2); }
            | TOKEN_OPEN_BRACE expr_list TOKEN_CLOSE_BRACE { $$ = expr_create_array_literal($2); }
            ;

ident   : TOKEN_IDENTIFIER { $$ = strdup(yytext); }
        ;

string  : TOKEN_STRING { $$ = strdup(yytext); }
        ;

opt_stmt_list   : stmt_list { $$ = $1; }
                | %empty { $$ = NULL; }
                ;

stmt_list   : stmt stmt_list { $$ = stmt_create_list($1, $2); } 
            | stmt { $$ = stmt_create_list($1, NULL); } 
            ;

stmt        : cmp_stmt { $$ = $1; }
            ;

arr_index       : TOKEN_OPEN_BRACKET expr TOKEN_CLOSE_BRACKET arr_index { $$ = expr_create_arr_index($2, $4); }
                | TOKEN_OPEN_BRACKET expr TOKEN_CLOSE_BRACKET { $$ = expr_create_arr_index($2, NULL); }
                ;

cmp_stmt    : open_cmp_stmt { $$ = $1; }
            | closed_cmp_stmt { $$ = $1; }
            ;

open_cmp_stmt   : TOKEN_IF TOKEN_OPEN_PARENS expr TOKEN_CLOSE_PARENS cmp_stmt { $$ = stmt_create_if($3, $5, NULL); }
                | TOKEN_IF TOKEN_OPEN_PARENS expr TOKEN_CLOSE_PARENS closed_cmp_stmt TOKEN_ELSE open_cmp_stmt { $$ = stmt_create_if($3, $5, $7); }
                | TOKEN_FOR TOKEN_OPEN_PARENS opt_expr TOKEN_SEMICOLON opt_expr TOKEN_SEMICOLON opt_expr TOKEN_CLOSE_PARENS open_cmp_stmt { $$ = stmt_create_for($3, $5, $7, $9); }
                ;

closed_cmp_stmt : TOKEN_IF TOKEN_OPEN_PARENS expr TOKEN_CLOSE_PARENS closed_cmp_stmt TOKEN_ELSE closed_cmp_stmt { $$ = stmt_create_if($3, $5, $7); }
                | TOKEN_FOR TOKEN_OPEN_PARENS opt_expr TOKEN_SEMICOLON opt_expr TOKEN_SEMICOLON opt_expr TOKEN_CLOSE_PARENS closed_cmp_stmt { $$ = stmt_create_for($3, $5, $7, $9); }
                | other { $$ = $1; }
                ;

other   : TOKEN_OPEN_BRACE stmt_list TOKEN_CLOSE_BRACE { $$ = stmt_create_block($2); }
        | var_decl { $$ = stmt_create(STMT_DECL, $1, NULL, NULL, NULL, NULL, NULL, NULL); }
        | expr TOKEN_SEMICOLON { $$ = stmt_create(STMT_EXPR, NULL, NULL, $1, NULL, NULL, NULL, NULL); }
        | print TOKEN_SEMICOLON { $$ = $1; }
        | return TOKEN_SEMICOLON { $$ = $1; }
        ;

print       : TOKEN_PRINT opt_expr_list { $$ = stmt_create(STMT_PRINT, NULL, NULL, $2, NULL, NULL, NULL, NULL); }
            ;

return      : TOKEN_RETURN opt_expr { $$ = stmt_create(STMT_RETURN, NULL, NULL, $2, NULL, NULL, NULL, NULL); }
            ;

%%

int yyerror(char *s) {
        printf("Parse error: %s\n", s);
        return 1;
}