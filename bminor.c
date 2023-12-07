#include "encoder/encode.h"
#include "resolver/resolve.h"
#include "typechecker/typecheck.h"
#include "codegenerator/codegen.h"
#include "scope.h"
#include "decl.h"

extern FILE *yyin;
extern int yylex();
extern char *yytext;
extern int yyparse();
extern int yyleng;

extern struct decl* program;
struct scope *top = NULL;
extern int resolve_error;
extern int typecheck_error;

void usage(int status);
void open_file(char *input_file);
int scan(char *input_file);
int parse(char *input_file);
int print(char *input_file);
int resolve(char *input_file);
int typecheck(char *input_file);
int codegen(char *input_file, char *output_file);

char *PROGRAM_NAME = NULL;
char *token_name[] = {
    "TOKEN_EOF",
    "TOKEN_COMMENT",
    "TOKEN_IDENTIFIER",
    "TOKEN_INTEGER",
    "TOKEN_FLOAT",
    "TOKEN_CHAR",
    "TOKEN_STRING",
    "TOKEN_COLON",
    "TOKEN_SEMICOLON",
    "TOKEN_COMMA",
    "TOKEN_OPEN_BRACKET",
    "TOKEN_CLOSE_BRACKET",
    "TOKEN_OPEN_BRACE",
    "TOKEN_CLOSE_BRACE",
    "TOKEN_OPEN_PARENS",
    "TOKEN_CLOSE_PARENS",
    "TOKEN_INC",
    "TOKEN_DEC",
    "TOKEN_ASSIGN",
    "TOKEN_EXPONENT",
    "TOKEN_ADD",
    "TOKEN_SUBTRACT",
    "TOKEN_MULTIPLY",
    "TOKEN_DIVIDE",
    "TOKEN_MOD",
    "TOKEN_EQUALITY",
    "TOKEN_GE",
    "TOKEN_LE",
    "TOKEN_LESS",
    "TOKEN_GREATER",
    "TOKEN_INEQUALITY",
    "TOKEN_NEGATION",
    "TOKEN_NOT",
    "TOKEN_AND",
    "TOKEN_OR",
    "TOKEN_ERROR",
    "TOKEN_TYPE",
    "TOKEN_FUNC",
    "TOKEN_ARR",
    "TOKEN_VOID",
    "TOKEN_BOOL",
    "TOKEN_IF",
    "TOKEN_ELSE",
    "TOKEN_FOR",
    "TOKEN_WHILE",
    "TOKEN_PRINT",
    "TOKEN_RETURN",
    "TOKEN_AUTO"
    };

void usage(int status) {
    fprintf(stderr, "Usage: %s flag\n\n", PROGRAM_NAME);
    fprintf(stderr, "   --encode INPUT_FILE                 Encode strings from INPUT_FILE\n");
    fprintf(stderr, "   --scan INPUT_FILE                   Scan strings from INPUT_FILE\n");
    fprintf(stderr, "   --parse INPUT_FILE                  Parse strings from INPUT_FILE\n");
    fprintf(stderr, "   --resolve INPUT_FILE                Resolve code from INPUT_FILE\n");
    fprintf(stderr, "   --typecheck INPUT_FILE              Typecheck code from INPUT_FILE\n");
    fprintf(stderr, "   --codegen INPUT_FILE OUTPUT_FILE    Generate code from INPUT_FILE\n");
    exit(status);
}

void open_file(char *input_file) {

    yyin = fopen(input_file,"r");

    if (!yyin) {
        printf("ERROR: File path specified does not exist.");
        exit(EXIT_FAILURE);
    }

}

int scan(char *input_file) {

    open_file(input_file);

    while (1) {
        int t = yylex();

        if (strlen(yytext) > 257) {
            printf("ERROR: The input length is %li, which larger than 255 characters.\n", strlen(yytext) - 2);
            exit(EXIT_FAILURE);
        }

        if (t == TOKEN_EOF) break;
        else if (t == TOKEN_CHAR || t == TOKEN_STRING ) {
            char *s = (char *) calloc(256, sizeof(char));
            string_decode(yytext, s, t);
            printf("%s %s\n", token_name[t - 258], s);
        } else if (t == TOKEN_ERROR ){
            exit(EXIT_FAILURE);
        } else {
            printf("%s %s\n", token_name[t - 258], yytext);
        }
    }

    return 0;
}

int parse(char *input_file) {
    
    open_file(input_file);

    if (yyparse() == 0) {
        printf("Parse successful!\n");
        return 0;
    } else {
        printf("Parse failed.\n");
        return 1;
    }

}

int print(char *input_file) {

    int result = parse(input_file);

    if (!result) {
        decl_print_list(program);
    }

    return result;

}

int resolve(char *input_file) {

    int result = parse(input_file);

    if (result) {
        return 1;
    }

    scope_enter();
    decl_resolve(program, 0);
    scope_exit();

    return resolve_error > 0;
}

int typecheck(char *input_file) {

    int result = resolve(input_file);

    if (result == 1) {
        return result;
    }

    decl_typecheck(program);

    return typecheck_error > 0;
}

int codegen(char *input_file, char *output_file) {

    int result = typecheck(input_file);

    if (result == 1) {
        return result;
    }

    freopen(output_file, "w", stdout);
    decl_codegen(program);

    return 0;
}

int main(int argc, char *argv[]) {
    PROGRAM_NAME = argv[0];
    int argind = 1;

    while (argind < argc && strlen(argv[argind]) > 1 && argv[argind][0] == '-') {
        if (!strcmp(argv[argind], "--help") || !argv[argind + 1]) {
            usage(0);
        }

        if (!strcmp(argv[argind], "--encode")) {
            return encode_file(argv[argind + 1]);
        } else if (!strcmp(argv[argind], "--scan")) {
            return scan(argv[argind + 1]);
        } else if (!strcmp(argv[argind], "--parse")) {
            return parse(argv[argind + 1]);
        } else if (!strcmp(argv[argind], "--print")) {
            return print(argv[argind + 1]);
        } else if (!strcmp(argv[argind], "--resolve")) {
            return resolve(argv[argind + 1]);
        } else if (!strcmp(argv[argind], "--typecheck")) {
            return typecheck(argv[argind + 1]);
        } else if (!strcmp(argv[argind], "--codegen")) {
            return codegen(argv[argind + 1], argv[argind + 2]);
        }
        argind++;
    }

    return 0;
}