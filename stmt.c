#include "stmt.h"

struct stmt * stmt_create( stmt_t kind, struct decl *decl, struct expr *init_expr, struct expr *expr, struct expr *next_expr, struct stmt *body, struct stmt *else_body, struct stmt *next ) {

    struct stmt *s = calloc(1, sizeof(struct stmt));
    s->kind = kind;
    s->decl = decl;
    s->init_expr = init_expr;
    s->expr = expr;
    s->next_expr = next_expr;
    s->body = body;
    s->else_body = else_body;
    s->next = next;

    return s;
}

struct stmt * stmt_create_if( struct expr* cond, struct stmt *if_block, struct stmt *else_block ) {

    struct stmt* i = stmt_create(STMT_IF_ELSE, NULL, NULL, cond, NULL, if_block, else_block, NULL);
    return i;

}

struct stmt * stmt_create_list(struct stmt* current, struct stmt* next) {
    
    current->next = next;
    return current;

}

struct stmt * stmt_create_block(struct stmt* body) {

    struct stmt* b = stmt_create(STMT_BLOCK, NULL, NULL, NULL, NULL, body, NULL, NULL);
    return b;

}

struct stmt * stmt_create_for(struct expr* init, struct expr* condition, struct expr* next, struct stmt* body) {
    
    struct stmt* f = stmt_create(STMT_FOR, NULL, init, condition, next, body, NULL, NULL);
    return f;

}

void stmt_print_list(struct stmt *s, int indent) {

    if (!s) {
        return;
    }
    struct stmt* curr = s;

    while (curr->next) {
        stmt_print(curr, indent);
        curr = curr->next;
    }

    stmt_print(curr, indent);
    return;

}

void stmt_print( struct stmt *s, int indent ) {

    switch (s->kind) {
        case STMT_DECL:
            decl_print(s->decl, indent);
            printf("\n");
            break;
        case STMT_EXPR:
            expr_print(s->expr);
            printf(";\n");
            break;
        case STMT_IF_ELSE:
            printf("if ("); 
            expr_print(s->expr);
            printf(")\n");
            stmt_print(s->body, indent);

            if (s->else_body) {
                printf("else\n");
                stmt_print(s->else_body, indent);
            }

            break;
        case STMT_FOR:
            printf("for ("); 
            expr_print(s->init_expr); 
            printf("; ");
            expr_print(s->expr);
            printf("; ");
            expr_print(s->next_expr);
            printf(")\n");
            stmt_print(s->body, indent);
            break;
        case STMT_PRINT:
            printf("print ");
            expr_print(s->expr);
            printf(";\n");
            break;
        case STMT_RETURN:
            printf("return ");
            expr_print(s->expr);
            printf(";\n");
            break;
        case STMT_BLOCK:
            printf("{\n");
            stmt_print_list(s->body, indent + 1);
            printf("}\n");
            break;
        case STMT_LIST:
            stmt_print_list(s, indent);
            break;
    }
}