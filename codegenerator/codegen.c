#include "codegen.h"

int label_number = 0;
int string_label_number = 0;
int scratch_regs[7] = {0, 0, 0, 0, 0, 0, 0};
int codegen_error = 0;
char* arg_reg[6] = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
int data_segment = 0;
int func_param_count = 0;

int label_create() {
    return label_number++;
}

const char * label_name(int label) {
    char *str = malloc(sizeof(char) * 10);
    sprintf(str, ".L%d", label);
    return str;
}

char * string_label_name() {
    char *str = malloc(sizeof(char) * 10);
    sprintf(str, ".s%d", string_label_number);
    string_label_number++;
    return str;
}

int scratch_alloc() {

    for (int i = 0; i < 7; i++) {
        if (scratch_regs[i] == 0) {
            scratch_regs[i] = 1;
            return i;
        }
    }

    fprintf(stderr, "ERROR: out of scratch registers.\n");
    codegen_error++;
    return -1;
}

void scratch_free(int r) {
    scratch_regs[r] = 0;
}

const char * scratch_name(int r) {
    char *str = malloc(sizeof(char) * 10);
    if (r == 6) {
        sprintf(str, "%%rbx");
    } else {
        sprintf(str, "%%r1%d", r);
    }
    return str;
}

void pre_func_codegen(struct decl *d) {

    printf("PUSHQ %%rbp\n");
    printf("MOVQ %%rsp, %%rbp\n");

    for (int i = 0; i < d->param_count && i < 6; i++) {
        printf("PUSHQ %s\n", arg_reg[i]);
    }

    printf("SUBQ $%d, %%rsp\n", 8 * d->local_count);
    printf("PUSHQ %%rbx\n");
    printf("PUSHQ %%r12\n");
    printf("PUSHQ %%r13\n");
    printf("PUSHQ %%r14\n");
    printf("PUSHQ %%r15\n");

}

void post_func_codegen(struct decl *d) {

    printf("\n.%s_epilogue:\n", d->name);
    printf("POPQ %%rbx\n");
    printf("POPQ %%r12\n");
    printf("POPQ %%r13\n");
    printf("POPQ %%r14\n");
    printf("POPQ %%r15\n");
    printf("MOVQ %%rbp, %%rsp\n");
    printf("POPQ %%rbp\n");
    printf("RET\n\n");
    
}

void decl_codegen(struct decl *d) {

    if (!d) {
        return;
    }

    if (d->symbol->kind == SYMBOL_GLOBAL) {
        if (d->code) {
            if (data_segment == 1) {
                data_segment = 0;
                printf("\n.text\n");
            }
            func_param_count = d->param_count;
            printf(".global %s\n", d->name);
            printf("%s:\n", d->name);
            pre_func_codegen(d);
            stmt_codegen(d->code, d);
            post_func_codegen(d);
        } else {
            if (data_segment == 0) {
                data_segment = 1;
                printf("\n.data\n");
            }
            printf(".global %s\n", d->name);
            switch (d->type->kind) {
                case TYPE_BOOLEAN:
                    if (d->value) {
                        printf("%s: .quad %d\n", d->name, d->value->boolean_literal);
                    } else {
                        printf("%s: .quad 0\n", d->name);
                    }
                    break;
                case TYPE_CHARACTER:
                    if (d->value) {
                        printf("%s: .quad %d\n", d->name, d->value->char_literal);
                    } else {
                        printf("%s: .quad 0\n", d->name);
                    }
                    break;
                case TYPE_INTEGER:
                    if (d->value) {
                        printf("%s: .quad %d\n", d->name, d->value->integer_literal);
                    } else {
                        printf("%s: .quad 0\n", d->name);
                    }
                    break;
                case TYPE_STRING:
                    printf(".%s: .string %s\n", d->name, get_string(d->value->string_literal));
                    break;
                case TYPE_ARRAY:
                    if (d->type->subtype->kind != TYPE_INTEGER) {
                        fprintf(stderr, "ERROR: arrays with type other than integer not accepted in code generation.\n");
                        codegen_error++;
                    }
                    printf("%s: .quad ", d->name);
                    struct expr *curr = d->value->left;
                    while (curr && curr->right) {
                        printf("%d, ", curr->left->integer_literal);
                        curr = curr->right;
                    }
                    printf("%d\n", curr->left->integer_literal);
                    break;
                case TYPE_FUNCTION: break;
                case TYPE_ERROR: break;
                case TYPE_VOID: break;
                case TYPE_FLOAT: break;
            }
        }
    } else if (d->symbol->kind == SYMBOL_LOCAL) {
        if (!d->value) {
            d->value = expr_create_integer_literal(0);
        }
        expr_codegen(d->value);
        printf("MOVQ %s, %s\n", scratch_name(d->value->reg), symbol_codegen(d->symbol));
        scratch_free(d->value->reg);
    }

    decl_codegen(d->next);

}

void cmp_codegen(struct expr* e, char *type) {

    int true_label = label_create();
    int done_label = label_create();
    expr_codegen(e->left);
    expr_codegen(e->right);
    printf("CMPQ %s, %s\n", scratch_name(e->right->reg), scratch_name(e->left->reg));
    printf("%s %s\n", type, label_name(true_label));
    printf("MOVQ $0, %s\n", scratch_name(e->right->reg));
    printf("JMP %s\n", label_name(done_label));
    printf("%s:\n", label_name(true_label));
    printf("MOVQ $1, %s\n", scratch_name(e->right->reg));
    printf("%s:\n", label_name(done_label));
    e->reg = e->right->reg;
    scratch_free(e->left->reg);

}

void func_call_codegen(struct expr* e) {

    struct expr *name = e->left;
    struct expr *args = e->right;
    struct expr *curr_arg = args;

    e->reg = scratch_alloc();
    
    int num_args = 0;
    while (curr_arg) {
        num_args++;
        expr_codegen(curr_arg->left);
        curr_arg = curr_arg->right;
    }

    if (num_args > 6) {
        fprintf(stderr, "ERROR: functions with more than 6 parameters not accepted in code generation.\n");
        codegen_error++;
    }

    curr_arg = args;

    for (int i = 0; i < 6; i++) {
        if (!curr_arg) {
            break;
        }
        printf("MOVQ %s, %s\n", scratch_name(curr_arg->left->reg), arg_reg[i]);
        scratch_free(curr_arg->left->reg);
        curr_arg = curr_arg->right;
    }

    printf("PUSHQ %%r10\n");
    printf("PUSHQ %%r11\n");
    printf("CALL %s\n", name->name);
    printf("POPQ %%r11\n");
    printf("POPQ %%r10\n");
    printf("MOVQ %%rax, %s\n", scratch_name(e->reg));

}

void expr_codegen(struct expr *e) {

    if (!e) {
        return;
    }

    switch (e->kind) {
        case EXPR_ADD:
            expr_codegen(e->left);
            expr_codegen(e->right);
            printf("ADDQ %s, %s\n", scratch_name(e->left->reg), scratch_name(e->right->reg));
            e->reg = e->right->reg;
            scratch_free(e->left->reg);
            break;
        case EXPR_SUBTRACT:
            expr_codegen(e->left);
            expr_codegen(e->right);
            printf("SUBQ %s, %s\n", scratch_name(e->right->reg), scratch_name(e->left->reg));
            e->reg = e->left->reg;
            scratch_free(e->right->reg);
            break;
        case EXPR_MULTIPLY:
            expr_codegen(e->left);
            expr_codegen(e->right);
            printf("MOVQ %s, %%rax\n", scratch_name(e->left->reg));
            printf("IMULQ %s\n", scratch_name(e->right->reg));
            printf("MOVQ %%rax, %s\n", scratch_name(e->right->reg));
            e->reg = e->right->reg;
            scratch_free(e->left->reg);
            break;
        case EXPR_DIVIDE:
            expr_codegen(e->left);
            expr_codegen(e->right);
            printf("MOVQ %s, %%rax\n", scratch_name(e->left->reg));
            printf("CQO\n");
            printf("IDIVQ %s\n", scratch_name(e->right->reg));
            printf("MOVQ %%rax, %s\n", scratch_name(e->right->reg));
            e->reg = e->right->reg;
            scratch_free(e->left->reg);
            break;
        case EXPR_MOD:
            expr_codegen(e->left);
            expr_codegen(e->right);
            printf("MOVQ %s, %%rax\n", scratch_name(e->left->reg));
            printf("CQO\n");
            printf("IDIVQ %s\n", scratch_name(e->right->reg));
            printf("MOVQ %%rdx, %s\n", scratch_name(e->right->reg));
            e->reg = e->right->reg;
            scratch_free(e->left->reg);
            break;
        case EXPR_EXP:
            {
                struct expr* call = expr_create_func_call(expr_create_name("integer_power"), expr_create_list(e->left, expr_create_list(e->right, NULL)));
                expr_codegen(call);
                e->reg = call->reg;
            }
            break;
        case EXPR_OR:
            expr_codegen(e->left);
            expr_codegen(e->right);
            printf("ORQ %s, %s\n", scratch_name(e->left->reg), scratch_name(e->right->reg));
            e->reg = e->right->reg;
            scratch_free(e->left->reg);
            break;
        case EXPR_AND:
            expr_codegen(e->left);
            expr_codegen(e->right);
            printf("ANDQ %s, %s\n", scratch_name(e->left->reg), scratch_name(e->right->reg));
            e->reg = e->right->reg;
            scratch_free(e->left->reg);
            break;
        case EXPR_NOT:
            expr_codegen(e->left);
            printf("NOTQ %s\n", scratch_name(e->left->reg));
            e->reg = e->left->reg;
            break;
        case EXPR_GREATER:
            cmp_codegen(e, "JG");
            break;
        case EXPR_LESS:
            cmp_codegen(e, "JL");
            break;
        case EXPR_GE:
            cmp_codegen(e, "JGE");
            break;
        case EXPR_LE:
            cmp_codegen(e, "JLE");
            break;
        case EXPR_EQUALITY: // need to do some work here still for comparing strings
            cmp_codegen(e, "JE");
            break;
        case EXPR_INEQUALITY:
            cmp_codegen(e, "JNE");
            break;
        case EXPR_POS:
            expr_codegen(e->left);
            e->reg = e->left->reg;
            break;
        case EXPR_NEG:
            expr_codegen(e->left);
            printf("NEGQ %s\n", scratch_name(e->left->reg));
            e->reg = e->left->reg;
            break;
        case EXPR_INC:
            expr_codegen(e->left);
            e->reg = scratch_alloc();
            printf("MOVQ %s, %s\n", scratch_name(e->left->reg), scratch_name(e->reg));
            printf("INCQ %s\n", scratch_name(e->left->reg));
            printf("MOVQ %s, %s\n", scratch_name(e->left->reg), symbol_codegen(e->left->symbol));
            scratch_free(e->left->reg);
            break;
        case EXPR_DEC:
            expr_codegen(e->left);
            e->reg = scratch_alloc();
            printf("DECQ %s\n", scratch_name(e->left->reg));
            printf("MOVQ %s, %s\n", scratch_name(e->left->reg), symbol_codegen(e->left->symbol));
            scratch_free(e->left->reg);
            break;
        case EXPR_FUNC:
            func_call_codegen(e);
            break;
        case EXPR_ARR_SUB:
            {
                struct expr *index = e->right;
                if (index->right) {
                    fprintf(stderr, "ERROR: arrays with more than 1 dimension not accepted in code generation.\n");
                    codegen_error++;
                }
                expr_codegen(e->right->left);
                expr_codegen(e->left);
                e->reg = scratch_alloc();
                printf("MOVQ (%s, %s, 8), %s\n", scratch_name(e->left->reg), scratch_name(e->right->left->reg), scratch_name(e->reg));
                scratch_free(e->right->reg);
                scratch_free(e->left->reg);
            }
            break;
        case EXPR_ASSIGN:
            expr_codegen(e->right);
            e->reg = scratch_alloc();
            if (e->left->kind == EXPR_ARR_SUB) {
                if (e->left->right->right) {
                    fprintf(stderr, "ERROR: arrays with more than 1 dimension not accepted in code generation.\n");
                    codegen_error++;
                }
                expr_codegen(e->left->right->left);
                expr_codegen(e->left->left);
                printf("LEAQ (%s, %s, 8), %s\n", scratch_name(e->left->left->reg), scratch_name(e->left->right->left->reg), scratch_name(e->reg));
                printf("MOVQ %s, (%s)\n", scratch_name(e->right->reg), scratch_name(e->reg));
            } else {
                printf("MOVQ %s, %s\n", scratch_name(e->right->reg), symbol_codegen(e->left->symbol));
            }
            printf("MOVQ %s, %s\n", scratch_name(e->right->reg), scratch_name(e->reg));
            scratch_free(e->right->reg);
            break;
        case EXPR_INTEGER: 
            e->reg = scratch_alloc();
            printf("MOVQ $%d, %s\n", e->integer_literal, scratch_name(e->reg));
            break;
        case EXPR_FLOAT:
            fprintf(stderr, "ERROR: floats not accepted in code generation.\n");
            codegen_error++;
            break;
        case EXPR_IDENTIFIER:
            e->reg = scratch_alloc();
            if (e->symbol->kind == SYMBOL_GLOBAL && e->symbol->type->kind == TYPE_ARRAY) {
                printf("LEAQ %s, %s\n", symbol_codegen(e->symbol), scratch_name(e->reg));
            } else if (e->symbol->kind == SYMBOL_GLOBAL && e->symbol->type->kind == TYPE_STRING) {
                printf("MOVQ $.%s, %s\n", symbol_codegen(e->symbol), scratch_name(e->reg));
            } else {
                printf("MOVQ %s, %s\n", symbol_codegen(e->symbol), scratch_name(e->reg));
            }
            break;
        case EXPR_CHAR:
            e->reg = scratch_alloc();
            printf("MOVQ $%d, %s\n", e->char_literal, scratch_name(e->reg));
            break;
        case EXPR_STRING:
            e->reg = scratch_alloc();
            char *string_label = string_label_name();
            printf("\n.data\n%s: .string %s\n\n.text\n", string_label, get_string(e->string_literal));
            printf("MOVQ $%s, %s\n", string_label, scratch_name(e->reg));
            break;
        case EXPR_BOOL:
            e->reg = scratch_alloc();
            printf("MOVQ $%d, %s\n", e->boolean_literal, scratch_name(e->reg));
            break;
        case EXPR_LIST: break;
        case EXPR_INDEX: break;
        case EXPR_ARR: break;
    }

}

void stmt_codegen(struct stmt *s, struct decl *d) {

    if (!s) {
        return;
    }

    switch(s->kind) {
        case STMT_DECL:
            data_segment = 0;
            decl_codegen(s->decl);
            break;
        case STMT_EXPR:
            expr_codegen(s->expr);
            scratch_free(s->expr->reg);
            break;
        case STMT_IF_ELSE: 
            {
                expr_codegen(s->expr);
                int else_label = label_create();
                int b = label_create();
                printf("CMP $0, %s\n", scratch_name(s->expr->reg));
                printf("JE %s\n", label_name(else_label));
                scratch_free(s->expr->reg);
                stmt_codegen(s->body, d);
                printf("JMP %s\n", label_name(b));
                printf("%s:\n", label_name(else_label));
                stmt_codegen(s->else_body, d);
                printf("JMP %s\n", label_name(b));
                printf("%s:\n", label_name(b));
                break;
            }
        case STMT_FOR:
            {
                if (s->init_expr) {
                    expr_codegen(s->init_expr);
                    scratch_free(s->init_expr->reg);
                }
                int t = label_create();
                int b = label_create();
                printf("%s:\n", label_name(t));
                if (!s->expr) {
                    s->expr = expr_create_boolean_literal("true");
                }
                expr_codegen(s->expr);
                printf("CMP $0, %s\n", scratch_name(s->expr->reg));
                printf("JE %s\n", label_name(b));
                scratch_free(s->expr->reg);
                stmt_codegen(s->body, d);
                if (s->next_expr) {
                    expr_codegen(s->next_expr);
                    scratch_free(s->next_expr->reg);
                }
                printf("JMP %s\n", label_name(t));
                printf("%s:\n", label_name(b));
                break;
            }
        case STMT_PRINT:
            if (s->expr) {
                struct expr *curr = s->expr;
                while (curr) {
                    struct type* t = expr_typecheck(curr->left);

                    struct expr* call;
                    if (t->kind == TYPE_INTEGER) {
                        call = expr_create_func_call(expr_create_name("print_integer"), expr_create_list(curr->left, NULL));
                    } else if (t->kind == TYPE_STRING) {
                        call = expr_create_func_call(expr_create_name("print_string"), expr_create_list(curr->left, NULL));
                    } else if (t->kind == TYPE_CHARACTER) {
                        call = expr_create_func_call(expr_create_name("print_character"), expr_create_list(curr->left, NULL));
                    } else if (t->kind == TYPE_BOOLEAN) {
                        call = expr_create_func_call(expr_create_name("print_boolean"), expr_create_list(curr->left, NULL));
                    }
                    expr_codegen(call);
                    scratch_free(call->reg);
                    curr = curr->right;
                }
            }
            break;
        case STMT_RETURN:
            if (s->expr) {
                expr_codegen(s->expr);
                printf("MOVQ %s, %%rax\n", scratch_name(s->expr->reg));
            }
            printf("JMP .%s_epilogue\n", d->name);
            if (s->expr) {
                scratch_free(s->expr->reg);
            }
            break;
        case STMT_BLOCK:
            stmt_codegen(s->body, d);
            break;
        case STMT_LIST: break;
    }

    stmt_codegen(s->next, d);

}

const char * symbol_codegen(struct symbol *s) {
    char *str = malloc(sizeof(char) * 10);

    switch(s->kind) {
        case SYMBOL_GLOBAL:
            return s->name;
        case SYMBOL_LOCAL:
            sprintf(str, "%d(%%rbp)", -8 * (s->which + 1 + func_param_count));
            return str;
        case SYMBOL_PARAM:
            sprintf(str, "%d(%%rbp)", -8 * (s->which + 1));
            return str;
    }

    return NULL;
}