#include "expr.h"

struct expr * expr_create( expr_t kind, struct expr *left, struct expr *right ) {
    
    struct expr *e = malloc(sizeof(struct expr));
    memset(e, 0, sizeof(struct expr));

    e->kind = kind;

    if (left) {
        e->left = left;
    }

    if (right) {
        e->right = right;
    }

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

struct expr * expr_create_boolean_literal( int c ) {

    struct expr *e = expr_create(EXPR_BOOL, NULL, NULL);
    if (c == 0) {
        e->boolean_literal = true;
    } else {
        e->boolean_literal = false;
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

void expr_print( struct expr *e ) {

    switch (e->kind) {
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
            if (e->boolean_literal) {
                printf("true");
            } else {
                printf("false");
            }
            break;
    }
}