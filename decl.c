#include "decl.h"

struct decl * decl_create( char *name, struct type *type, struct expr *value, struct stmt *code, struct decl *next ) {
    
    struct decl *d = malloc(sizeof(struct decl));
    memset(d, 0, sizeof(struct decl));

    d->name = name;
    d->type = type;

    if (value) {
        d->value = value;
    }
    
    if (code) {
        d->code = code;
    }

    d->next = NULL;
    return d;

}

void decl_print( struct decl *d, int indent ) {
    
    for (int i = 0; i < indent; i++) {
        printf("\t");
    }

    printf("%s: ", d->name);
    print_type(d->type);
    
    if (d->value) {
        printf(" = ");
        expr_print(d->value);
    } else if (d->code) {
        printf(" = ");
        stmt_print(d->code, indent + 1);
    }

    printf(";");

}
