#include "decl.h"

struct decl * decl_create( char *name, struct type *type, struct expr *value, struct stmt *code, struct decl *next ) {
    
    struct decl *d = calloc(1, sizeof(struct decl));

    d->name = name;
    d->type = type;
    d->value = value;
    d->code = code;
    d->next = next;
    return d;

}

struct decl * decl_create_func( char *name, struct type *type, struct stmt *code ) {

    struct stmt *body = stmt_create(STMT_LIST, NULL, NULL, NULL, NULL, NULL, NULL, code);
    struct decl *f = decl_create(name, type, NULL, body, NULL);
    return f;

}

struct decl * decl_create_list(struct decl *current, struct decl *next) {

    current->next = next;
    return current;

}

void decl_print( struct decl *d, int indent ) {

    if (!d) {
        return;
    }

    printf("%s: ", d->name);
    type_print(d->type);

    if (d->type->kind == TYPE_FUNCTION) {

        if (!d->code) {
            printf(";");
            return;
        }

        printf(" = {\n");
        stmt_print(d->code, 0);
        printf("}");

    } else {

        if (!d->value) {
            printf(";");
            return;
        }

        printf(" = ");
        expr_print(NULL, d->value);
        printf(";");
    }

}

void decl_print_list(struct decl *d) {
    
    if (!d) {
        return;
    }

    while (d->next) {
        decl_print(d, 0);
        d = d->next;
        printf("\n");
    }

    decl_print(d, 0);
    printf("\n");
    return;
}