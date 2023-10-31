#include "symbol.h"

struct symbol * symbol_create( symbol_t kind, struct type *type, char *name ) {

    struct symbol *s = calloc(1, sizeof(struct symbol));
    s->kind = kind;
    s->type = type;
    s->name = name;

    return s;
}