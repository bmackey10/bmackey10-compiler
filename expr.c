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

    struct expr *a = expr_create(EXPR_ARR_SUB, name, index);
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
                char first_num = *(str_in + 3);
                char second_num = *(str_in + 4);
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
                    char first_num = *(curr + 3);
                    char second_num = *(curr + 4);
                    char temp[2] = {first_num, second_num};
                    str_out[i] = (int) strtol(temp, 0, 16);
                    curr += 3;
                    break;
            }
            curr += 2;
        } else {
            str_out[i] = *curr;
            curr++;
        }
        i++;
    }

    str_out[i] = '\0';
    free(str_in);

    return str_out;
    
}

void expr_print_list(struct expr *e) {

    if (!e) {
        return;
    }
    struct expr* curr = e;

    while (curr->right) {
        expr_print(curr, curr->left);
        curr = curr->right;
        printf(",");
    }

    expr_print(curr, curr->left);
    return;

}

char* get_char(char c) {
    char* str_out = malloc(sizeof(char) * 7);

    *str_out = '\'';
    int i = 1;

    switch (c) {
        case '\a':
            str_out[i] = '\\';
            str_out[i + 1] = 'a';
            i++;
            break;
        case '\b':
            str_out[i] = '\\';
            str_out[i + 1] = 'b';
            i++;
            break;
        case '\e':
            str_out[i] = '\\';
            str_out[i = 1] = 'e';
            i++;
            break;
        case '\f':
            str_out[i] = '\\';
            str_out[i + 1] = 'f';
            i++;
            break;
        case '\n':
            str_out[i] = '\\';
            str_out[i + 1] = 'n';
            i++;
            break;
        case '\r':
            str_out[i] = '\\';
            str_out[i + 1] = 'r';
            i++;
            break;
        case '\t':
            str_out[i] = '\\';
            str_out[i + 1] = 't';
            i++;
            break;
        case '\v':
            str_out[i] = '\\';
            str_out[i + 1] = 'v';
            i++;
            break;
        default:
            if (((int) c) <= 126 && ((int) c) >= 32) {
                str_out[i] = c;
            } else if ((((int) c) > 126 && ((int) c) <= 255) || ((int) c) < 32) {
                char hex_value[2];
                unsigned char hex_temp = c;
                sprintf(hex_value, "%x", hex_temp);
                str_out[i] = '\\';
                str_out[i + 1] = '0';
                str_out[i + 2] = 'x';
                str_out[1 + 3] = hex_value[0];
                str_out[1 + 4] = hex_value[1];
                i = i + 4;
            }
        
    }

    str_out[i + 1] = '\'';

    return str_out;
}

char* get_string(const char* str) {

    int length = strlen(str);
    char* str_out = malloc(sizeof(char) * (length * 2 + 3));

    *str_out = '\"';
    int j = 1;

    for (int i = 0; i < strlen(str); i++) {
        
        switch (str[i]) {
            case '\a':
                str_out[j] = '\\';
                str_out[j + 1] = 'a';
                j++;
                break;
            case '\b':
                str_out[j] = '\\';
                str_out[j + 1] = 'b';
                j++;
                break;
            case '\e':
                str_out[j] = '\\';
                str_out[j + 1] = 'e';
                j++;
                break;
            case '\f':
                str_out[j] = '\\';
                str_out[j + 1] = 'f';
                j++;
                break;
            case '\n':
                str_out[j] = '\\';
                str_out[j + 1] = 'n';
                j++;
                break;
            case '\r':
                str_out[j] = '\\';
                str_out[j + 1] = 'r';
                j++;
                break;
            case '\t':
                str_out[j] = '\\';
                str_out[j + 1] = 't';
                j++;
                break;
            case '\v':
                str_out[j] = '\\';
                str_out[j + 1] = 'v';
                j++;
                break;
            default:
                if (((int) str[i]) <= 126 && ((int) str[i]) >= 32) {
                    str_out[j] = str[i];
                } else if ((((int) str[i]) > 126 && ((int) str[i]) <= 255) || ((int) str[i]) < 32) {
                    char hex_value[2];
                    unsigned char hex_temp = str[i];
                    sprintf(hex_value, "%x", hex_temp);
                    str_out[j] = '\\';
                    str_out[j + 1] = '0';
                    str_out[j + 2] = 'x';
                    str_out[j + 3] = hex_value[0];
                    str_out[j + 4] = hex_value[1];
                    j = j + 4;
                }
        }

        j++;
    }

    str_out[j] = '\"';
    str_out[j + 1] = '\0';

    return str_out;
}

int get_rank( expr_t curr) {

    switch (curr) {
        case EXPR_OR:
            return 1;
            break;
        case EXPR_AND:
            return 2;
            break;
        case EXPR_NOT:
            return 3;
            break;
        case EXPR_GREATER:
        case EXPR_LESS:
        case EXPR_GE:
        case EXPR_LE:
        case EXPR_EQUALITY:
        case EXPR_INEQUALITY:
            return 4;
            break;
        case EXPR_ADD:
        case EXPR_SUBTRACT:
            return 5;
            break;
        case EXPR_MULTIPLY:
        case EXPR_DIVIDE:
        case EXPR_MOD:
            return 6;
            break;
        case EXPR_EXP:
            return 7;
            break;
        case EXPR_POS:
        case EXPR_NEG:
            return 8;
            break;
        case EXPR_INC:
        case EXPR_DEC:
            return 9;
            break;
        default:
            return -1;
    }

}

int get_parens(struct expr *prev, struct expr *curr) {

    if (!prev) {
        return 0;
    }

    int prev_rank = get_rank(prev->kind);
    int curr_rank = get_rank(curr->kind);

    return (prev_rank > curr_rank && prev_rank > 0 && curr_rank > 0);

}

void expr_print( struct expr *p, struct expr *e ) {

    if (!e) {
        return;
    }

    int parens = get_parens(p, e);

    if (parens) {
        printf("(");
    }

    switch (e->kind) {
        case EXPR_LIST:
            expr_print_list(e);
            break;
        case EXPR_ARR:
            printf("{");
            expr_print(e, e->left);
            printf("}");
            break;
        case EXPR_ASSIGN:
            expr_print(e, e->left);
            printf(" = ");
            expr_print(e, e->right);
            break;
        case EXPR_ADD:
            expr_print(e, e->left);
            printf("+");
            expr_print(e, e->right);
            break;
        case EXPR_SUBTRACT:
            expr_print(e, e->left);
            printf("-");
            expr_print(e, e->right);
            break;
        case EXPR_MULTIPLY:
            expr_print(e, e->left);
            printf("*");
            expr_print(e, e->right);
            break;
        case EXPR_DIVIDE:
            expr_print(e, e->left);
            printf("/");
            expr_print(e, e->right);
            break;
        case EXPR_MOD:
            expr_print(e, e->left);
            printf("%%");
            expr_print(e, e->right);
            break;
        case EXPR_EXP:
            expr_print(e, e->left);
            printf("^");
            expr_print(e, e->right);
            break;
        case EXPR_OR:
            expr_print(e, e->left);
            printf("||");
            expr_print(e, e->right);
            break;
        case EXPR_AND:
            expr_print(e, e->left);
            printf("&&");
            expr_print(e, e->right);
            break;
        case EXPR_NOT:
            printf("!");
            expr_print(e, e->left);
            break;
        case EXPR_GREATER:
            expr_print(e, e->left);
            printf(">");
            expr_print(e, e->right);
            break;
        case EXPR_LESS:
            expr_print(e, e->left);
            printf("<");
            expr_print(e, e->right);
            break;
        case EXPR_GE:
            expr_print(e, e->left);
            printf(">=");
            expr_print(e, e->right);
            break;
        case EXPR_LE:
            expr_print(e, e->left);
            printf("<=");
            expr_print(e, e->right);
            break;
        case EXPR_EQUALITY:
            expr_print(e, e->left);
            printf("==");
            expr_print(e, e->right);
            break;
        case EXPR_INEQUALITY:
            expr_print(e, e->left);
            printf("!=");
            expr_print(e, e->right);
            break;
        case EXPR_POS:
            printf("+");
            expr_print(e, e->left);
            break;
        case EXPR_NEG:
            printf("-");
            expr_print(e, e->left);
            break;
        case EXPR_INC:
            expr_print(e, e->left);
            printf("++");
            break;
        case EXPR_DEC:
            expr_print(e, e->left);
            printf("--");
            break;
        case EXPR_INTEGER:
            printf("%d", e->integer_literal);
            break;
        case EXPR_FLOAT:
            printf("%g", e->float_literal);
            break;
        case EXPR_IDENTIFIER:
            printf("%s", e->name);
            break;
        case EXPR_CHAR:
            printf("%s", get_char(e->char_literal));
            break;
        case EXPR_STRING:
            printf("%s", get_string(e->string_literal));
            break;
        case EXPR_BOOL:
            if (e->boolean_literal == 1) {
                printf("true");
            } else {
                printf("false");
            }
            break;
        case EXPR_FUNC:
            expr_print(e, e->left);
            printf("(");
            expr_print(e, e->right);
            printf(")");
            break;
        case EXPR_ARR_SUB:
            expr_print(e, e->left);
            expr_print(e, e->right);
            break;
        case EXPR_INDEX:
            printf("[");
            expr_print(e, e->left);
            printf("]");
            expr_print(e, e->right);
            break;
    }

    if (parens) {
        printf(")");
    }
}