#include "encoder/encode.h"
#include "scanner/scan.h"

extern FILE *yyin;
extern int yylex();
extern char *yytext;

char *PROGRAM_NAME = NULL;
char *token_name[] = {
    "TOKEN_EOF",
    "TOKEN_COMMENT",
    "TOKEN_KEYWORD",
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
    "TOKEN_ERROR"
    };

void usage(int status) {
    fprintf(stderr, "Usage: %s flag\n\n", PROGRAM_NAME);
    fprintf(stderr, "   --encode INPUT_FILE     Encode strings from INPUT_FILE\n");
    fprintf(stderr, "   --scan INPUT_FILE     Scan strings from INPUT_FILE\n");
    exit(status);
}

int scan_file(char *input_file) {

    yyin = fopen(input_file,"r");

    if (!yyin) {
        printf("ERROR: File path specified does not exist.");
        exit(EXIT_FAILURE);
    }

    while (1) {
        token_t t = yylex();

        if (strlen(yytext) > 257) {
            printf("ERROR: The input length is %li, which larger than 255 characters.\n", strlen(yytext) - 2);
            exit(EXIT_FAILURE);
        }

        if (t == TOKEN_EOF) break;
        else if (t == TOKEN_CHAR || t == TOKEN_STRING ) {
            char *s = (char *) calloc(256, sizeof(char));
            string_decode(yytext, s);
            printf("%s %s\n", token_name[t], s);
        } else if (t == TOKEN_ERROR ){
            exit(EXIT_FAILURE);
        } else {
            printf("%s %s\n", token_name[t], yytext);
        }
    }

    return 0;
}

int main(int argc, char *argv[]) {
    PROGRAM_NAME = argv[0];
    int argind = 1;

    while (argind < argc && strlen(argv[argind]) > 1 && argv[argind][0] == '-') {
        if (!strcmp(argv[argind], "--encode")) {
            if (!argv[argind + 1]) {
                usage(0);
            }
            return encode_file(argv[argind + 1]);
        } else if (!strcmp(argv[argind], "--scan")) {
            if (!argv[argind + 1]) {
                usage(0);
            }
            return scan_file(argv[argind + 1]);
        } else if (!strcmp(argv[argind], "--help")) {
            usage(0);
        }
        argind++;
    }

    return 0;
}