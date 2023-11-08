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

int param_type_check(struct param_list *a, struct param_list *b) {
    struct param_list *one = a;
    struct param_list *two = b;

    if (!a && !b) {
        return 1;
    }

    while (one && two) {
        if (!type_check(one->type, two->type)) {
            return 0;
        }

        one = one->next;
        two = two->next;
    }

    if ((one && !two) || (!one && two)) {
        return 0;
    }

    return 1;
}