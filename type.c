#include "type.h"

struct type * type_create( type_t kind, struct type *subtype, struct param_list *params ) {

    struct type *t = calloc(1, sizeof(struct type));
    t->kind = kind;
    t->subtype = subtype;
    t->params = params;
    t->contains_ident = 0;

    return t;

}

struct type * type_function_create( type_t kind, struct type *subtype, struct param_list *params ) {

    struct type *f = type_create(kind, subtype, params);

    return f;

}

struct type * type_array_create( type_t kind, struct expr *expr, struct type *subtype ) {

    struct type *a = type_create(kind, subtype, NULL);
    a->expr = expr;

    return a;

}

void type_print( struct type *t ) {

    switch (t->kind) {
        case TYPE_VOID:
            printf("void");
            break;
        case TYPE_BOOLEAN:
            printf("boolean");
            break;
        case TYPE_CHARACTER:
            printf("char");
            break;
        case TYPE_INTEGER:
            printf("integer");
            break;
        case TYPE_FLOAT:
            printf("float");
            break;
        case TYPE_STRING:
            printf("string");
            break;
        case TYPE_ARRAY:
            printf("array [");
            if (t->expr) {
                expr_print(NULL, t->expr);
            }
            printf("]");
            if (t->subtype) {
                printf(" ");
                type_print(t->subtype);
            }
            break;
        case TYPE_FUNCTION:
            printf("function ");
            type_print(t->subtype);
            param_list_print(t->params);
            break;
        case TYPE_ERROR:
            printf("error");
            break;
    };

}