#include "expr.h"

struct expr * expr_create( expr_t kind, struct expr *left, struct expr *right ) {
    
    struct expr *e = calloc(1, sizeof(struct expr));

    e->kind = kind;
    e->left = left;
    e->right = right;

    return e;
}

struct expr * expr_create_name( const char *n ) {

    struct expr *e = expr_create(EXPR_IDENTIFIER, NULL, NULL);
    e->name = n;
    return e;

}

struct expr * expr_create_integer_literal( int c ) {

    struct expr *e = expr_create(EXPR_INTEGER, NULL, NULL);
    e->integer_literal = c;
    return e;

}

struct expr * expr_create_float_literal( double c ) {

    struct expr *e = expr_create(EXPR_FLOAT, NULL, NULL);
    e->float_literal = c;
    return e;

}

struct expr * expr_create_boolean_literal( char *c ) {

    struct expr *e = expr_create(EXPR_BOOL, NULL, NULL);
    if (!strcmp(c, "true")) {
        e->boolean_literal = 1;
    } else {
        e->boolean_literal = 0;
    }
    return e;

}

struct expr * expr_create_char_literal( char c ) {

    struct expr *e = expr_create(EXPR_CHAR, NULL, NULL);
    e->char_literal = c;
    return e;

}

struct expr * expr_create_string_literal( const char *str ) {

    struct expr *e = expr_create(EXPR_STRING, NULL, NULL);
    e->string_literal = str;
    return e;

}

struct expr * expr_create_array_literal( struct expr *expr ) {

    struct expr *a = expr_create(EXPR_ARR, expr, NULL);
    return a;

}

struct expr * expr_create_list( struct expr *current, struct expr *next ) {

    struct expr *l = expr_create(EXPR_LIST, current, next);
    return l;

}

struct expr * expr_create_func_call( struct expr *name, struct expr *args) {

    struct expr *f = expr_create(EXPR_FUNC, name, args);
    return f;

}

struct expr * expr_create_arr_subscript( struct expr *name, struct expr *index) {

    struct expr *a = expr_create(EXPR_ARR_INDEX, name, index);
    return a;

}

struct expr * expr_create_arr_index( struct expr *current, struct expr *next) {

    struct expr *a = expr_create(EXPR_INDEX, current, next);
    return a;

}

double parse_float(char *str_in) {

    double double_out;
    char *end_first;
    char *end_second;

    if (strchr(str_in, 'e')) {
            char *first = strtok(str_in, "e");
            char *second = strtok(NULL, "");
            double_out = strtod(first, &end_first) * pow(10, strtod(second, &end_second));
    } else if (strchr(str_in, 'E')) {
            char *first = strtok(str_in, "E");
            char *second = strtok(NULL, "");
            double_out = strtod(first, &end_first) * pow(10, strtod(second, &end_second));
    } else {
            double_out = strtod(str_in, &end_first);
    }

    return double_out;

}

char parse_char(char *str_in) {

    char char_out;
    str_in++;
    char *next = str_in + 1;

    if (*str_in == '\\') {
        switch (*next) {
                case 'a':
                    char_out = '\a';
                    break;
                case 'b':
                    char_out = '\b';
                    break;
                case 'e':
                    char_out = '\e';
                    break;
                case 'f':
                    char_out = '\f';
                    break;
                case 'n':
                    char_out = '\n';
                    break;
                case 'r':
                    char_out = '\r';
                    break;
                case 't':
                    char_out = '\t';
                    break;
                case 'v':
                    char_out = '\v';
                    break;
                case '\\':
                    char_out = '\\';
                    break;
                case '\'':
                    char_out = '\'';
                    break;
                case '\"':
                    char_out = '\"';
                    break;
                case '0': ;
                    char first_num = *str_in + 3;
                    char second_num = *str_in + 4;
                    char temp[2] = {first_num, second_num};
                    char_out = (int) strtol(temp, 0, 16);
                    break;
        }
    } else {
        char_out = *str_in;
    }

    return char_out;

}

char * parse_string(char *str_in) {

    int length = strlen(str_in);
    char *str_out = malloc(sizeof(char) * (length + 1));
    int i = 0;
    char *curr = str_in;
    curr++;

    while (curr < str_in + length - 1) {
        if (*curr == '\\') {
            char *next = curr + 1;
            switch (*next) {
                    case 'a':
                        str_out[i] = '\a';
                        break;
                    case 'b':
                        str_out[i] = '\b';
                        break;
                    case 'e':
                        str_out[i] = '\e';
                        break;
                    case 'f':
                        str_out[i] = '\f';
                        break;
                    case 'n':
                        str_out[i] = '\n';
                        break;
                    case 'r':
                        str_out[i] = '\r';
                        break;
                    case 't':
                        str_out[i] = '\t';
                        break;
                    case 'v':
                        str_out[i] = '\v';
                        break;
                    case '\\':
                        str_out[i] = '\\';
                        break;
                    case '\'':
                        str_out[i] = '\'';
                        break;
                    case '\"':
                        str_out[i] = '\"';
                        break;
                    case '0': ;
                        char first_num = *curr + 3;
                        char second_num = *curr + 4;
                        char temp[2] = {first_num, second_num};
                        str_out[i] = (int) strtol(temp, 0, 16);
                        curr += 3;
                        break;
            }
            curr += 2;
        } else {
            str_out[i] = *str_in;
            curr++;
        }
        i++;
    }

    str_out[i] = '\0';

    return str_out;
    
}

void expr_print( struct expr *e ) {

    if (!e) {
        return;
    }

    switch (e->kind) {
        case EXPR_LIST:
            expr_print(e->left);
            printf(", ");
            expr_print(e->right);
            break;
        case EXPR_ARR:
            printf("{");
            expr_print(e->left);
            printf("}");
            break;
        case EXPR_ASSIGN:
            expr_print(e->left);
            printf("=");
            expr_print(e->right);
            break;
        case EXPR_ADD:
            expr_print(e->left);
            printf("+");
            expr_print(e->right);
            break;
        case EXPR_SUBTRACT:
            expr_print(e->left);
            printf("-");
            expr_print(e->right);
            break;
        case EXPR_MULTIPLY:
            expr_print(e->left);
            printf("*");
            expr_print(e->right);
            break;
        case EXPR_DIVIDE:
            expr_print(e->left);
            printf("/");
            expr_print(e->right);
            break;
        case EXPR_MOD:
            expr_print(e->left);
            printf("%%");
            expr_print(e->right);
            break;
        case EXPR_EXP:
            expr_print(e->left);
            printf("^");
            expr_print(e->right);
            break;
        case EXPR_OR:
            expr_print(e->left);
            printf("||");
            expr_print(e->right);
            break;
        case EXPR_AND:
            expr_print(e->left);
            printf("&&");
            expr_print(e->right);
            break;
        case EXPR_NOT:
            printf("!");
            expr_print(e->left);
            break;
        case EXPR_GREATER:
            expr_print(e->left);
            printf(">");
            expr_print(e->right);
            break;
        case EXPR_LESS:
            expr_print(e->left);
            printf("<");
            expr_print(e->right);
            break;
        case EXPR_GE:
            expr_print(e->left);
            printf(">=");
            expr_print(e->right);
            break;
        case EXPR_LE:
            expr_print(e->left);
            printf("<=");
            expr_print(e->right);
            break;
        case EXPR_EQUALITY:
            expr_print(e->left);
            printf("==");
            expr_print(e->right);
            break;
        case EXPR_INEQUALITY:
            expr_print(e->left);
            printf("!=");
            expr_print(e->right);
            break;
        case EXPR_POS:
            printf("+");
            expr_print(e->left);
            break;
        case EXPR_NEG:
            printf("-");
            expr_print(e->left);
            break;
        case EXPR_INC:
            expr_print(e->left);
            printf("++");
            break;
        case EXPR_DEC:
            expr_print(e->left);
            printf("--");
            break;
        case EXPR_INTEGER:
            printf("%d", e->integer_literal);
            break;
        case EXPR_FLOAT:
            printf("%f", e->float_literal);
            break;
        case EXPR_IDENTIFIER:
            printf("%s", e->name);
            break;
        case EXPR_CHAR:
            printf("%c", e->char_literal);
            break;
        case EXPR_STRING:
            printf("%s", e->string_literal);
            break;
        case EXPR_BOOL:
            if (e->boolean_literal == 1) {
                printf("true");
            } else {
                printf("false");
            }
            break;
        case EXPR_FUNC:
            expr_print(e->left);
            printf("(");
            expr_print(e->right);
            printf(")");
            break;
        case EXPR_ARR_INDEX:
            expr_print(e->left);
            expr_print(e->right);
            break;
        case EXPR_INDEX:
            printf("[");
            expr_print(e->left);
            printf("]");
            expr_print(e->right);
            break;
    }
}