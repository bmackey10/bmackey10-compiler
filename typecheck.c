#include "typecheck.h"

int typecheck_error = 0;

int type_equals(struct type *a, struct type *b) {

    if (!a && !b) {
        return 0;
    } else if (!a || !b) {
        return 1;
    }

    if (a->kind == b->kind) {
        if (a->kind == TYPE_ARRAY) {
            return type_equals(a->subtype, b->subtype);
        } else if (a->kind == TYPE_FUNCTION) {
            if (type_equals(a->subtype, b->subtype) == 0 && param_type_equals(a->params, b->params) == 0) {
                return 0;
            } 
            return 1;
        } else {
            return 0;
        }
    }

    return 1;

}

int param_type_equals(struct param_list *a, struct param_list *b) {
    struct param_list *one = a;
    struct param_list *two = b;

    if (!a && !b) {
        return 0;
    }

    while (one && two) {
        if (type_equals(one->type, two->type) == 1) {
            return 1;
        }

        one = one->next;
        two = two->next;
    }

    if ((one && !two) || (!one && two)) {
        return 1;
    }

    return 0;
}

struct type * type_copy(struct type *c) {

    if (!c) {
        return NULL;
    }

    struct type *copy = type_create(c->kind, type_copy(c->subtype), param_type_copy(c->params));
    copy->expr = c->expr;
    copy->contains_ident = c->contains_ident;

    return copy;

}

struct param_list * param_type_copy(struct param_list *c) {

    if (!c) {
        return NULL;
    }

    struct param_list *copy = param_list_create(c->name, type_copy(c->type), NULL);
    struct param_list *curr = c;
    struct param_list *copy_curr = copy;
    curr = curr->next;

    while (curr) {
        copy_curr->next = param_list_create(curr->name, type_copy(curr->type), NULL);
        copy_curr = copy_curr->next;
        curr = curr->next;
    }

    return copy;
}

void type_delete(struct type *d) {

    if (!d) {
        return;
    }

    param_type_delete(d->params);
    type_delete(d->subtype);
    free(d);
    return;

}

void param_type_delete(struct param_list *d) {

    if (!d) {
        return;
    }

    struct param_list *curr = d;
    struct param_list *next_param = d->next;

    while (curr) {
        type_delete(curr->type);
        free(curr->symbol);
        free(curr);
        curr = next_param;
        if (next_param) {
            next_param = next_param->next;
        }
    }

    return;

}

void param_list_typecheck(char *name, struct param_list *params, struct expr *args) {

    struct param_list *curr_param = params;
    struct expr *curr_arg = args;

    while (curr_param && curr_arg) {
        struct type *arg_type = expr_typecheck(curr_arg->left);
        if (type_equals(curr_param->type, arg_type) == 1) {
            printf("ERROR: argument ");
            type_print(arg_type);
            printf(" (");
            expr_print(NULL, curr_arg->left);
            printf(") does not have correct parameter data type of ");
            type_print(curr_param->type);
            printf(" (%s).\n", curr_param->name);
            typecheck_error = 1;
        }
        type_delete(arg_type);
        curr_param = curr_param->next;
        curr_arg = curr_arg->right;
    }

    if (!curr_param && curr_arg) {
        printf("ERROR: too many arguments for function %s.\n", name);
        typecheck_error = 1;
    } else if (curr_param && !curr_arg) {
        printf("ERROR: too few arguments for function %s.\n", name);
        typecheck_error = 1;
    }

}

struct type * array_typecheck(struct expr *arr) {

    struct type *arr_type = expr_typecheck(arr->left);
    struct expr *curr = arr;
    struct type *curr_type;
    int arr_length = 0;

    while (curr) {
        curr_type = expr_typecheck(curr->left);
        if (type_equals(arr_type, curr_type) == 1) {
            printf("ERROR: array of type ");
            type_print(arr_type);
            printf(" cannot have data of type ");
            type_print(curr_type);
            printf(" (");
            expr_print(NULL,curr->left);
            printf(" ).\n");
            typecheck_error = 1;
        }
        type_delete(curr_type);
        curr = curr->right;
        arr_length++;
    }

    struct type *result = type_create(TYPE_ARRAY, type_copy(arr_type), NULL);
    type_delete(arr_type);
    result->expr = expr_create_integer_literal(arr_length);
    
    return result;
}

struct type * array_subscript_typecheck(struct type *arr_type, struct expr *index) {

    struct type *curr_type = arr_type;
    struct expr *curr_expr = index;
    struct type *prev_type;

    while (curr_type && curr_expr) {
        struct type *expr_type = expr_typecheck(curr_expr->left);
        if (expr_type->kind != TYPE_INTEGER) {
            printf("ERROR: cannot subscript an array with ");
            type_print(expr_type);
            printf(" (");
            expr_print(NULL, curr_expr->left);
            printf(").\n");
            typecheck_error = 1;
        }
        type_delete(expr_type);
        prev_type = curr_type;
        curr_type = curr_type->subtype;
        curr_expr = curr_expr->right;
    }

    if (!curr_type && curr_expr) {
        printf("ERROR: cannot subscript ");
        type_print(prev_type);
        printf(" type.\n");
        typecheck_error = 1;
    }

    return type_copy(prev_type);

}

struct type * expr_typecheck(struct expr *e) {

    if (!e) {
        return NULL;
    }

    struct type *left = expr_typecheck(e->left);
    struct type *right = expr_typecheck(e->right);
    struct type *result = NULL;

    switch (e->kind) {
        case EXPR_OR:
        case EXPR_AND:
            if (left->kind == TYPE_ERROR && right->kind == TYPE_ERROR) {
                printf("ERROR: cannot infer types of left and right expressions.\n");
                typecheck_error = 1;
                result = type_create(TYPE_ERROR, NULL, NULL);
            } else if ((left->kind == TYPE_BOOLEAN || left->kind == TYPE_ERROR)  && (right->kind == TYPE_BOOLEAN || right->kind == TYPE_ERROR)) {
                result = type_create(TYPE_BOOLEAN, NULL, NULL);
            } else {
                printf("ERROR: incompatible types ");
                type_print(left);
                printf(" (");
                expr_print(NULL, e->left);
                printf(") and ");
                type_print(right);
                printf(" (");
                expr_print(NULL, e->right);
                printf(") with boolean operator.\n");
                typecheck_error = 1;
                result = type_create(TYPE_ERROR, NULL, NULL);
            }
            break;
        case EXPR_NOT:
            if (left->kind == TYPE_ERROR) {
                printf("ERROR: cannot infer types of left and right expressions.\n");
                typecheck_error = 1;
                result = type_create(TYPE_ERROR, NULL, NULL);
            } else if (left->kind == TYPE_BOOLEAN) {
                result = type_create(TYPE_BOOLEAN, NULL, NULL);
            } else {
                printf("ERROR: incompatible types ");
                type_print(left);
                printf(" (");
                expr_print(NULL, e->left);
                printf(") with boolean operator.\n");
                typecheck_error = 1;
                result = type_create(TYPE_ERROR, NULL, NULL);
            }
            break;
        case EXPR_GREATER:
        case EXPR_LESS:
        case EXPR_GE:
        case EXPR_LE:
            if (left->kind == TYPE_ERROR && right->kind == TYPE_ERROR) {
                printf("ERROR: cannot infer types of left and right expressions.\n");
                typecheck_error = 1;
                result = type_create(TYPE_ERROR, NULL, NULL);
            } else if ((left->kind == TYPE_INTEGER || left->kind == TYPE_ERROR)  && (right->kind == TYPE_INTEGER || right->kind == TYPE_ERROR)) {
                result = type_create(TYPE_BOOLEAN, NULL, NULL);
            } else if ((left->kind == TYPE_FLOAT || left->kind == TYPE_ERROR) && (right->kind == TYPE_FLOAT || right->kind == TYPE_ERROR)) {
                result = type_create(TYPE_BOOLEAN, NULL, NULL);
            } else {
                printf("ERROR: incompatible types ");
                type_print(left);
                printf(" (");
                expr_print(NULL, e->left);
                printf(") and ");
                type_print(right);
                printf(" (");
                expr_print(NULL, e->right);
                printf(") with comparison operator.\n");
                typecheck_error = 1;
                result = type_create(TYPE_ERROR, NULL, NULL);
            }
            break;
        case EXPR_EQUALITY:
        case EXPR_INEQUALITY:
            if (type_equals(left, right) == 1) {
                printf("ERROR: incompatible types ");
                type_print(left);
                printf(" (");
                expr_print(NULL, e->left);
                printf(") and ");
                type_print(right);
                printf(" (");
                expr_print(NULL, e->right);
                printf(") with equality operator.\n");
                typecheck_error = 1;
                result = type_create(TYPE_ERROR, NULL, NULL);
            }
            if (left->kind == TYPE_FUNCTION || left->kind == TYPE_ARRAY || left->kind == TYPE_VOID) {
                printf("ERROR: incompatible types ");
                type_print(left);
                printf(" (");
                expr_print(NULL, e->left);
                printf(") and ");
                type_print(right);
                printf(" (");
                expr_print(NULL,e->right);
                printf(") with equality operator.\n");
                typecheck_error = 1;
                result = type_create(TYPE_ERROR, NULL, NULL);
            } else if (!result) {
                result = type_create(TYPE_BOOLEAN, NULL, NULL);
            }
            break;
        case EXPR_ADD:
        case EXPR_SUBTRACT:
        case EXPR_MULTIPLY:
        case EXPR_DIVIDE:
        case EXPR_MOD:
        case EXPR_EXP:
            if (left->kind == TYPE_ERROR && right->kind == TYPE_ERROR) {
                printf("ERROR: cannot infer types of left and right expressions.\n");
                typecheck_error = 1;
                result = type_create(TYPE_ERROR, NULL, NULL);
            } else if ((left->kind == TYPE_INTEGER || left->kind == TYPE_ERROR)  && (right->kind == TYPE_INTEGER || right->kind == TYPE_ERROR)) {
                result = type_create(TYPE_INTEGER, NULL, NULL);
            } else if ((left->kind == TYPE_FLOAT || left->kind == TYPE_ERROR) && (right->kind == TYPE_FLOAT || right->kind == TYPE_ERROR)) {
                result = type_create(TYPE_FLOAT, NULL, NULL);
            } else {
                printf("ERROR: incompatible types ");
                type_print(left);
                printf(" (");
                expr_print(NULL, e->left);
                printf(") and ");
                type_print(right);
                printf(" (");
                expr_print(NULL, e->right);
                printf(") with arithmetic operator.\n");
                typecheck_error = 1;
                result = type_create(TYPE_ERROR, NULL, NULL);
            }
            break;
        case EXPR_POS:
        case EXPR_NEG:
            if (left->kind == TYPE_ERROR) {
                printf("ERROR: cannot infer types of expression.\n");
                typecheck_error = 1;
                result = type_create(TYPE_ERROR, NULL, NULL);
            } else if (left->kind == TYPE_INTEGER) {
                result = type_create(TYPE_INTEGER, NULL, NULL);
            } else if (left->kind == TYPE_FLOAT) {
                result = type_create(TYPE_FLOAT, NULL, NULL);
            } else {
                printf("ERROR: incompatible type ");
                type_print(left);
                printf(" (");
                expr_print(NULL, e->left);
                printf(") with positive or negative operator.");
                typecheck_error = 1;
                result = type_create(TYPE_ERROR, NULL, NULL);
            }
            break;
            break;
        case EXPR_INC:
        case EXPR_DEC:
            if (left->kind == TYPE_ERROR) {
                printf("ERROR: cannot infer types of left and right expressions.\n");
                typecheck_error = 1;
                result = type_create(TYPE_ERROR, NULL, NULL);
            } else if (left->kind == TYPE_INTEGER) {
                result = type_create(TYPE_INTEGER, NULL, NULL);
            } else if (left->kind == TYPE_FLOAT) {
                result = type_create(TYPE_FLOAT, NULL, NULL);
            } else {
                printf("ERROR: incompatible types ");
                type_print(left);
                printf(" (");
                expr_print(NULL, e->left);
                printf(") and ");
                type_print(right);
                printf(" (");
                expr_print(NULL, e->right);
                printf(") with increment or decrement operator.\n");
                typecheck_error = 1;
                result = type_create(TYPE_ERROR, NULL, NULL);
            }
            break;
        case EXPR_INTEGER:
            result = type_create(TYPE_INTEGER, NULL, NULL);
            break;
        case EXPR_FLOAT:
            result = type_create(TYPE_FLOAT, NULL, NULL);
            break;
        case EXPR_CHAR:
            result = type_create(TYPE_CHARACTER, NULL, NULL);
            break;
        case EXPR_STRING:
            result = type_create(TYPE_STRING, NULL, NULL);
            break;
        case EXPR_BOOL:
            result = type_create(TYPE_BOOLEAN, NULL, NULL);
            break;
        case EXPR_ARR: 
            result = array_typecheck(e->left);
            break;
        case EXPR_FUNC:
            param_list_typecheck(e->left->symbol->name, e->left->symbol->type->params, e->right);
            result = type_copy(e->left->symbol->type->subtype);
            break;
        case EXPR_ARR_SUB: 
            if (left->kind != TYPE_ARRAY) {
                printf("ERROR: cannot subscript a non-array type ");
                type_print(left);
                printf(" (");
                expr_print(NULL, e->left);
                printf(").\n");
                typecheck_error = 1;
                result = type_create(TYPE_ERROR, NULL, NULL);
            } else {
                result = array_subscript_typecheck(left->subtype, e->right);
            }
            break;
        case EXPR_ASSIGN:
            if (e->left->kind != EXPR_IDENTIFIER && e->left->kind != EXPR_ARR_SUB) {
                printf("ERROR: cannot assign a non-variable ");
                type_print(left);
                printf(" (");
                expr_print(NULL, e->left);
                printf(") to a ");
                type_print(right);
                printf(" (");
                expr_print(NULL, e->right);
                printf(").\n");
                typecheck_error = 1;
            }
            if (type_equals(left, right) == 1) {
                printf("ERROR: incompatible types ");
                type_print(left);
                printf(" (");
                expr_print(NULL, e->left);
                printf(") and ");
                type_print(right);
                printf(" (");
                expr_print(NULL, e->right);
                printf(") with assignment operator.\n");
                typecheck_error = 1;
                result = type_create(TYPE_ERROR, NULL, NULL);
            } else {
                result = type_copy(left);
            }
            break;
        case EXPR_IDENTIFIER:
            result = type_copy(e->symbol->type);
            result->contains_ident = 1;
            break;
        case EXPR_LIST: break;
        case EXPR_INDEX: break;
    }


    if (((left && left->contains_ident) || (right && right->contains_ident)) && result) {
        result->contains_ident = 1;
    }

    type_delete(left);
    type_delete(right);

    return result;

}

void decl_typecheck(struct decl *d) {

    if (!d) {
        return;
    }

    if (d->type->kind == TYPE_ARRAY) {
        struct type* curr = d->type;
        struct type* t = NULL;
        while (curr->kind == TYPE_ARRAY) {
            if (curr->expr->kind == EXPR_INTEGER && curr->expr->integer_literal == 0) {
                printf("ERROR: the size of %s array cannot be zero.\n", d->name);
                typecheck_error = 1;
            } else if (curr->expr->kind != EXPR_INTEGER && d->symbol->kind == SYMBOL_GLOBAL) {
               printf("ERROR: cannot assign identifier %s using size expression ", d->symbol->name);
               expr_print(NULL, curr->expr);
               printf(" containing identifier (variable value) in global scope.\n");
               typecheck_error = 1;
            }
            t = expr_typecheck(curr->expr);
            if (t->kind != TYPE_INTEGER) {
                printf("ERROR: array size declarations must be an integer, so it cannot be ");
                expr_print(NULL, curr->expr);
                printf(".\n");
                typecheck_error = 1;
            }
            type_delete(t);
            curr = curr->subtype;
        }
    }

    if (d->value) {
        struct type *expr_type = expr_typecheck(d->value);
        if (((d->value->kind != EXPR_INTEGER && d->value->kind != EXPR_FLOAT && d->value->kind != EXPR_CHAR && d->value->kind != EXPR_STRING && d->value->kind != EXPR_BOOL && d->value->kind != EXPR_ARR ) || expr_type->contains_ident) && d->symbol->kind == SYMBOL_GLOBAL) {
            printf("ERROR: cannot assign identifier %s to ", d->symbol->name);
            expr_print(NULL, d->value);
            printf(" containing identifier or using an expression (variable value) in global scope.\n");
            typecheck_error = 1;
        }
        if (d->symbol->kind == SYMBOL_LOCAL && d->type->kind == TYPE_ARRAY && d->value->kind == EXPR_ARR) {
            printf("ERROR: cannot initialize local array %s with array ", d->name);
            expr_print(NULL, d->value);
            printf(".\n");
            typecheck_error = 1;
        }
        if (type_equals(d->type, expr_type)) {
            printf("ERROR: cannot assign ");
            type_print(expr_type);
            printf(" (");
            expr_print(NULL, d->value);
            printf(") to identifier %s with type ", d->symbol->name);
            type_print(d->type);
            printf(".\n");
            typecheck_error = 1;
        }
        if ((d->type->expr && d->type->expr->kind == EXPR_INTEGER) && (expr_type->expr && expr_type->expr->kind == EXPR_INTEGER) && d->type->expr->integer_literal != expr_type->expr->integer_literal) {
            printf("ERROR: the array size ");
            expr_print(NULL, d->type->expr);
            printf(" and the size of the array ");
            expr_print(NULL, expr_type->expr);
            printf(" do not match.\n");
            typecheck_error = 1;
        } else if ((d->type->expr && d->type->expr->kind != EXPR_INTEGER)) {
            printf("Cannot determine if array size expression ");
            expr_print(NULL, d->type->expr);
            printf(" and the size of the array (");
            expr_print(NULL, expr_type->expr);
            printf(") match.\n");
        }
        type_delete(expr_type);
    } else if (d->code) {
        struct type *return_type = stmt_typecheck(d->code);
        if (return_type && type_equals(d->type->subtype, return_type) == 1) {
            printf("ERROR: cannot return a "); 
            type_print(return_type);
            printf(" in a function that returns ");
            type_print(d->type->subtype);
            printf(".\n");
            typecheck_error = 1;
        }
        if (d->type->subtype->kind == TYPE_FUNCTION || d->type->subtype->kind == TYPE_ARRAY) {
            printf("ERROR: return type ");
            type_print(d->type->subtype);
            printf(" is an invalid return type for a function.\n");
            typecheck_error = 1;
        }
        type_delete(return_type);
    }

    decl_typecheck(d->next);

}

struct type * stmt_typecheck(struct stmt *s) {

    if (!s) {
        return NULL;
    }

    struct type *t;
    struct expr *curr;
    struct type *return_type = NULL;

    switch(s->kind) {
        case STMT_DECL: 
            decl_typecheck(s->decl);
            break;
        case STMT_EXPR:
            t = expr_typecheck(s->expr);
            type_delete(t);
            break;
        case STMT_IF_ELSE:
            t = expr_typecheck(s->expr);
            if (t->kind != TYPE_BOOLEAN) {
                printf("ERROR: if expression type ");
                type_print(t);
                printf(" (");
                expr_print(NULL, s->expr);
                printf(") is an invalid type for this statement.\n");
                typecheck_error = 1;
            }
            type_delete(t);
            stmt_typecheck(s->body);
            stmt_typecheck(s->else_body);
            break; 
        case STMT_FOR:
            t = expr_typecheck(s->init_expr);
            if (t && t->kind != TYPE_INTEGER && t->kind != TYPE_FLOAT) {
                printf("ERROR: for initial expression type ");
                type_print(t);
                printf(" (");
                expr_print(NULL, s->expr);
                printf(") is an invalid type for this statement.\n");
                typecheck_error = 1;
            }
            type_delete(t);
            t = expr_typecheck(s->expr);
            if (t && t->kind != TYPE_BOOLEAN) {
                printf("ERROR: for expression type ");
                type_print(t);
                printf(" (");
                expr_print(NULL, s->expr);
                printf(") is an invalid type for this statement.\n");
                typecheck_error = 1;
            }
            type_delete(t);
            t = expr_typecheck(s->next_expr);
            if (t && t->kind != TYPE_INTEGER && t->kind != TYPE_FLOAT) {
                printf("ERROR: for next expression type ");
                type_print(t);
                printf(" (");
                expr_print(NULL, s->expr);
                printf(") is an invalid type for this statement.\n");
                typecheck_error = 1;
            }
            type_delete(t);
            stmt_typecheck(s->body);
            break;
        case STMT_PRINT:
            curr = s->expr;
            while (curr) {
                t = expr_typecheck(curr->left);
                curr = curr->right;
                type_delete(t);
            }
            break;
        case STMT_RETURN: 
            t = expr_typecheck(s->expr);
            if (!t) {
                t = type_create(TYPE_VOID, NULL, NULL);
            }
            return_type = t;
            break;
        case STMT_BLOCK:
            stmt_typecheck(s->body);
            break;
        case STMT_LIST: break;
    }

    struct type *next_return_type = stmt_typecheck(s->next);
    
    if (next_return_type) {
        return_type = next_return_type;
    }

    return return_type;
}