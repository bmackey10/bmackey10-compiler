#include "resolve.h"
#include "../scope.h"

void decl_resolve(struct decl* d, int which) {

    if (!d) {
        return;
    }

    symbol_t curr_decl = top->next ? SYMBOL_LOCAL : SYMBOL_GLOBAL;
    d->symbol = symbol_create(curr_decl, d->type, d->name, which);
    scope_bind(d->name, d->symbol);

    if (d->type->kind == TYPE_ARRAY) {
            expr_resolve(d->type->expr);
        }

    if (d->value) {
        expr_resolve(d->value);
    } else if (d->code) {
        scope_enter();
        param_resolve(d->type->params, 1);
        scope_enter();
        stmt_resolve(d->code, 1);
        scope_exit();
        scope_exit();
        d->symbol->code = 1;
    } else if (!d->code && d->type->kind == TYPE_FUNCTION) {
        d->symbol->code = 0;
    }

    decl_resolve(d->next, 1);

}
int stmt_resolve(struct stmt* s, int which) {

    if (!s) {
        return which;
    }

    switch (s->kind) {
        case STMT_DECL:
            decl_resolve(s->decl, which);
            which++;
            break;
        case STMT_EXPR:
            expr_resolve(s->expr);
            break;
        case STMT_IF_ELSE:
            expr_resolve(s->expr);
            which = stmt_resolve(s->body, which);
            which = stmt_resolve(s->else_body, which);
            break;
        case STMT_FOR:
            expr_resolve(s->init_expr);
            expr_resolve(s->expr);
            expr_resolve(s->next_expr);
            which = stmt_resolve(s->body, which);
            break;
        case STMT_PRINT:
            expr_resolve(s->expr);
            break;
        case STMT_RETURN:
            expr_resolve(s->expr);
            break;
        case STMT_BLOCK:
            scope_enter();
            which = stmt_resolve(s->body, which);
            scope_exit();
        case STMT_LIST:
            break;
    }

    which = stmt_resolve(s->next, which);

    return which;
}

void expr_resolve(struct expr* e) {

    if (!e) {
        return;
    }

    if (e->kind == EXPR_IDENTIFIER) {
        e->symbol = scope_lookup(e->name);
    } else {
        expr_resolve(e->left);
        expr_resolve(e->right);
    }

}

void param_resolve(struct param_list *p, int which) {

    if (!p) {
        return;
    }

    struct param_list *curr = p;

    while (curr) {
        curr->symbol = symbol_create(SYMBOL_PARAM, curr->type, curr->name, which);
        scope_bind(curr->name, curr->symbol);
        curr = curr->next;
        which++;
    }

}