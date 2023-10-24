%token TOKEN_EOF
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

%{
    #include <stdio.h>
    #include <stdlib.h>
    extern char *yytext;
    extern int yylex();
    extern int yyerror( char *str );
%}

%%

program     : program decl      { return 0; }
            | decl
            ;

decl        : var_decl
            | func_decl
            ;

var_decl    : TOKEN_IDENTIFIER TOKEN_COLON decl_type TOKEN_SEMICOLON
            | TOKEN_IDENTIFIER TOKEN_COLON decl_type var_init TOKEN_SEMICOLON
            ;

func_decl   : TOKEN_IDENTIFIER TOKEN_COLON func_type TOKEN_SEMICOLON
            | TOKEN_IDENTIFIER TOKEN_COLON func_type func_init
            ;

decl_type   : TOKEN_TYPE
            | TOKEN_VOID
            | TOKEN_ARR TOKEN_OPEN_BRACKET expr TOKEN_CLOSE_BRACKET decl_type
            ;

func_type   : TOKEN_FUNC param_type TOKEN_OPEN_PARENS param_list TOKEN_CLOSE_PARENS
            ;

param_type  : TOKEN_TYPE
            | TOKEN_VOID
            | TOKEN_ARR TOKEN_OPEN_BRACKET TOKEN_CLOSE_BRACKET param_type
            ;

param_list  : param_list TOKEN_COMMA param
            | param
            ;

param       : %empty
            | TOKEN_IDENTIFIER TOKEN_COLON param_type
            ;

var_init    : init
            | arr_init
            ;

init        : TOKEN_ASSIGN expr
            ;

arr_init    : TOKEN_ASSIGN TOKEN_OPEN_BRACE expr_list TOKEN_CLOSE_BRACE
            ;

func_init   : TOKEN_ASSIGN stmt_block
            ;

stmt_block  : TOKEN_OPEN_BRACE TOKEN_CLOSE_BRACE
            | TOKEN_OPEN_BRACE stmt_list TOKEN_CLOSE_BRACE
            ;

stmt_list   : stmt_list stmt
            | stmt
            ;

stmt        : base_stmt
            | cmp_stmt
            | stmt_block
            ;

base_stmt   : decl
            | expr TOKEN_SEMICOLON
            | print TOKEN_SEMICOLON
            | return TOKEN_SEMICOLON
            ;

cmp_stmt    : if_stmt
            | for_stmt
            ;

expr    : %empty
        | TOKEN_IDENTIFIER TOKEN_ASSIGN or_expr
        | TOKEN_IDENTIFIER arr_index_list TOKEN_ASSIGN or_expr
        | or_expr
        ;

expr_list   : expr_list TOKEN_COMMA or_expr
            | or_expr
            ;

or_expr     : or_expr TOKEN_OR and_expr
            | and_expr
            ;

and_expr    : and_expr TOKEN_AND not_expr
            | not_expr
            ;

not_expr    : TOKEN_NOT not_expr
            | rel_expr
            ;

rel_expr    : rel_expr TOKEN_GE add_expr
            | rel_expr TOKEN_LE add_expr
            | rel_expr TOKEN_GREATER add_expr
            | rel_expr TOKEN_LESS add_expr
            | rel_expr TOKEN_EQUALITY add_expr
            | rel_expr TOKEN_INEQUALITY add_expr
            | add_expr
            ;

add_expr    : add_expr TOKEN_ADD mul_expr
            | add_expr TOKEN_SUBTRACT mul_expr
            | mul_expr
            ;

mul_expr    : mul_expr TOKEN_MULTIPLY exp_expr
            | mul_expr TOKEN_DIVIDE exp_expr
            | mul_expr TOKEN_MOD exp_expr
            | exp_expr
            ;

exp_expr    : exp_expr TOKEN_EXPONENT pre_expr
            | pre_expr
            ;

pre_expr    : post_expr
            | TOKEN_ADD pre_expr
            | TOKEN_SUBTRACT pre_expr
            ;

post_expr   : base_expr
            | base_expr TOKEN_INC
            | base_expr TOKEN_DEC
            ;

base_expr   : TOKEN_OPEN_PARENS expr TOKEN_CLOSE_PARENS
            | TOKEN_INTEGER
            | TOKEN_FLOAT
            | TOKEN_IDENTIFIER
            | TOKEN_CHAR
            | TOKEN_STRING
            | TOKEN_BOOL
            | TOKEN_IDENTIFIER TOKEN_OPEN_PARENS expr_list TOKEN_CLOSE_PARENS
            | TOKEN_IDENTIFIER arr_index_list
            ;

arr_index_list  : arr_index_list TOKEN_OPEN_BRACKET expr TOKEN_CLOSE_BRACKET
                | TOKEN_OPEN_BRACKET expr TOKEN_CLOSE_BRACKET
                ;

if_stmt     : TOKEN_IF TOKEN_OPEN_PARENS expr TOKEN_CLOSE_PARENS lim_cond_stmt { printf("Parse: if_stmt\n"); }
            | TOKEN_IF TOKEN_OPEN_PARENS expr TOKEN_CLOSE_PARENS cond_stmt { printf("Parse: if_stmt\n"); }
            | TOKEN_IF TOKEN_OPEN_PARENS expr TOKEN_CLOSE_PARENS lim_cond_stmt TOKEN_ELSE cond_stmt { printf("Parse: if_stmt\n"); }
            | TOKEN_IF TOKEN_OPEN_PARENS expr TOKEN_CLOSE_PARENS lim_cond_stmt TOKEN_ELSE lim_cond_stmt { printf("Parse: if_stmt\n"); }
            ;

for_stmt    : TOKEN_FOR TOKEN_OPEN_PARENS expr TOKEN_SEMICOLON expr TOKEN_SEMICOLON expr TOKEN_CLOSE_PARENS cond_stmt { printf("Parse: for_stmt\n"); }
            | TOKEN_FOR TOKEN_OPEN_PARENS expr TOKEN_SEMICOLON expr TOKEN_SEMICOLON expr TOKEN_CLOSE_PARENS lim_cond_stmt { printf("Parse: for_stmt\n"); }
            ;

cond_stmt   : TOKEN_IF TOKEN_OPEN_PARENS expr TOKEN_CLOSE_PARENS other { printf("Parse: cond_stmt\n"); }
            | TOKEN_IF TOKEN_OPEN_PARENS expr TOKEN_CLOSE_PARENS cond_stmt { printf("Parse: cond_stmt\n"); }
            | TOKEN_IF TOKEN_OPEN_PARENS expr TOKEN_CLOSE_PARENS lim_cond_stmt TOKEN_ELSE cond_stmt { printf("Parse: cond_stmt\n"); }
            | TOKEN_FOR TOKEN_OPEN_PARENS expr TOKEN_SEMICOLON expr TOKEN_SEMICOLON expr TOKEN_CLOSE_PARENS cond_stmt { printf("Parse: cond_stmt\n"); }
            ;

lim_cond_stmt   : TOKEN_IF TOKEN_OPEN_PARENS expr TOKEN_CLOSE_PARENS lim_cond_stmt TOKEN_ELSE lim_cond_stmt { printf("Parse: lim_cond_stmt\n"); }
                | TOKEN_FOR TOKEN_OPEN_PARENS expr TOKEN_SEMICOLON expr TOKEN_SEMICOLON expr TOKEN_CLOSE_PARENS lim_cond_stmt { printf("Parse: lim_cond_stmt\n"); }
                | other
                ;

other   : stmt_block { printf("Parse: other\n"); }
        | base_stmt { printf("Parse: other\n"); }
        ;

print       : TOKEN_PRINT expr_list
            ;

return      : TOKEN_RETURN expr
            ;

%%

int yyerror(char *s) {
        printf("Parse error: %s\n", s);
        return 1;
}