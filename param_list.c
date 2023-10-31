#include "param_list.h"

struct param_list * param_list_create( char *name, struct type *type, struct param_list *next ) {

    struct param_list *p = calloc(1, sizeof(struct param_list));

    p->name = name;
    p->type = type;
    p->next = next;

    return p;

}
void param_list_print( struct param_list *a ) {

    if (!a) {
        printf(" ()");
        return;
    }

    printf(" (");
    while (a->next) {
        printf("%s: ", a->name);
        type_print(a->type);
        printf(", ");
        a = a->next;
    }

    printf("%s: ", a->name);
    type_print(a->type);
    printf(")");

    return;

}