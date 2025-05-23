#include "ast.h"
short block_counter = 0 ;

AST *ast_new_vlpt(char* id,ParamEntry *params, int vlpt_type) {
    AST *node = malloc(sizeof(AST));
    if (node) {
        node->tag = AST_VLPT;
        node->data.AST_VLPT.params = params;
        node->data.AST_VLPT.vlpt_type = vlpt_type;
        node->data.AST_VLPT.id = id;
    }
    return node;
}


AST *ast_new_number(int number) {
    AST *node = malloc(sizeof(AST));
    if (node) {
        node->tag = AST_NUMBER;
        node->data.AST_NUMBER.number = number;
    }
    return node;
}
AST *ast_new_tab(DimEntry *dims, AST *id) {
    AST *node = malloc(sizeof(AST));
    if (node) {
        node->tag = AST_TAB;
        node->data.AST_TAB.dims = dims;
        node->data.AST_TAB.id = id;
    }
    return node;
}
AST *ast_new_id(char *id) {
    AST *node = malloc(sizeof(AST));
    if (node) {
        node->tag = AST_ID;
        node->data.AST_ID.id = id;
    }
    return node;
}

AST *ast_new_aff(AST *id, AST *number) {
    AST *node = malloc(sizeof(AST));
    if (node) {
        node->tag = AST_AFF;
        node->data.AST_AFF.op1 = id;
        node->data.AST_AFF.op2 = number;
    }
    return node;
}
AST *ast_new_if(AST *cond, AST *then_branch, AST *else_branch) {
    AST *node = malloc(sizeof(AST));
    if (node) {
        node->tag = AST_IF;
        node->data.AST_IF.cond = cond;
        node->data.AST_IF.then_branch = then_branch;
        node->data.AST_IF.else_branch = else_branch;
    }
    return node;
}
AST *ast_new_block(InstructEntry *instructs) {
    AST *node = malloc(sizeof(AST));
    if (node) {
        node->tag = AST_BLOCK;
        node->data.AST_BLOCK.instructs = instructs;
        node->data.AST_BLOCK.counter = 0; // Initialize counter
        printf("Created AST_BLOCK with instructs: %p\n", (void *)instructs);
    } else {
        printf("Error: malloc failed in ast_new_block\n");
    }
    return node;
}

AST *ast_new_binop(char* operation, AST *left, AST *right) {
    AST *node = malloc(sizeof(AST));
    if (node) {
        node->tag = AST_BINOP;
        node->data.AST_BINOP.left = left;
        node->data.AST_BINOP.right = right;
        node->data.AST_BINOP.op = operation; // Not used for simple ADD/MUL/etc.
        printf("just created new_binop\n");
        ast_print(node);

    }
    return node;
}
CaseEntry *case_entry_new(AST *value, AST *body, CaseEntry *next) {
    CaseEntry *entry = malloc(sizeof(CaseEntry));
    if (entry) {
        entry->value = value;
        entry->body = body;
        entry->next = next;
    }
    return entry;
}
AST *ast_new_for(AST *init, AST *cond, AST *post, AST *body) {
    AST *node = malloc(sizeof(AST));
    if (node) {
        node->tag = AST_FOR;
        node->data.AST_FOR.init = init;
        node->data.AST_FOR.cond = cond;
        node->data.AST_FOR.post = post;
        node->data.AST_FOR.body = body;
    }
    return node;
}
AST *ast_new_while(AST *cond, AST *body) {
    AST *node = malloc(sizeof(AST));
    if (node) {
        node->tag = AST_WHILE;
        node->data.AST_WHILE.cond = cond;
        node->data.AST_WHILE.body = body;
    }
    return node;
}
AST *ast_new_switch(AST *expr, CaseEntry *cases, AST *default_case) {
    AST *node = malloc(sizeof(AST));
    if (node) {
        node->tag = AST_SWITCH;
        node->data.AST_SWITCH.expr = expr;
        node->data.AST_SWITCH.cases = cases;
        node->data.AST_SWITCH.default_case = default_case;
    }
    return node;
}
AST *ast_new_moins(AST *op) {
    AST *node = malloc(sizeof(AST));
    if (node) {
        node->tag = AST_MOINS;
        node->data.AST_MOINS.op = op;
    }
    return node;
}
AST *ast_new_break(void) {
    AST *node = malloc(sizeof(AST));
    if (node) node->tag = AST_BREAK;
    return node;
}

AST *ast_new_return(AST *expr) {
    AST *node = malloc(sizeof(AST));
    if (node) {
        node->tag = AST_RETURN;
        node->data.AST_RETURN.expr = expr;
    }
    return node;
}
/*
void add_bloc_instr(AST *instr) {
    InstructEntry *instr_entry = malloc(sizeof(InstructEntry));
    curr_bloc_instructs = get_bloc_instructs();
    instr_entry->instruct = curr_bloc_instructs->next;
    curr_bloc_instructs->next = instr_entry; 
}*/

void initialize_term() {
    // Example initialization
}
void ast_print_param_list(ParamEntry *param) {
    int count = 0;
    while (param) {
        if (!param->param) {
            printf("(NULL param)");
        } else {
            ast_print(param->param);
        }
        param = param->next;
        if (++count > 100) {
            printf("... (param list too long or circular)\n");
            break;
        }
    }
}

void ast_print(AST *node) {
    if (!node) return;
    switch (node->tag) {
        case AST_NUMBER:
            printf("%d", node->data.AST_NUMBER.number);
            break;
        case AST_BREAK:
            printf("break;");
            break;
        case AST_RETURN:
            printf("return");
            if (node->data.AST_RETURN.expr) {
                printf(" ");
                ast_print(node->data.AST_RETURN.expr);
            }
            printf(";");
            break;
        case AST_ID:
            printf("%s", node->data.AST_ID.id);
            break;

        case AST_AFF:
            printf("aff (");
            ast_print(node->data.AST_AFF.op1);
            printf(" = ");
            ast_print(node->data.AST_AFF.op2);
            printf(")\n");
            break;

        case AST_BLOCK:
            printf("{\n");
            InstructEntry *entry = node->data.AST_BLOCK.instructs;
            while (entry) {
                printf("  ");
                ast_print(entry->instruct);
                entry = entry->next;
                printf("\n");
            }
            printf("}\n");
            break;
        case AST_FOR:
            printf("for (");
            ast_print(node->data.AST_FOR.init);
            printf("; ");
            ast_print(node->data.AST_FOR.cond);
            printf("; ");
            ast_print(node->data.AST_FOR.post);
            printf(") ");
            ast_print(node->data.AST_FOR.body);
            printf("\n");
            break;
        case AST_ADD:
            printf("(");
            ast_print(node->data.AST_BINOP.left);
            printf(" + ");
            ast_print(node->data.AST_BINOP.right);
            printf(")\n");
            break;

        case AST_MUL:
            printf("(");
            ast_print(node->data.AST_BINOP.left);
            printf(" * ");
            ast_print(node->data.AST_BINOP.right);
            printf(")\n");
            break;

        case AST_BINOP:
            printf("(");
            ast_print(node->data.AST_BINOP.left);
            if (node->data.AST_BINOP.op)
                printf(" %s ", node->data.AST_BINOP.op);
            else
                printf(" ? ");
            ast_print(node->data.AST_BINOP.right);
            printf(")\n");
            break;

        case AST_VLPT:
            if (node->data.AST_VLPT.vlpt_type == VLPT_FUNC_CALL) {
                printf("VLPT(id: %s, type: FUNC_CALL, params: ", node->data.AST_VLPT.id);
                ParamEntry *p = node->data.AST_VLPT.params;
                /*
                while (p) {
                    ast_print(p->param);
                    if (p->next) printf(", ");
                    p = p->next;
                }*/
                printf(")\n");
            }
            break;

        case AST_TAB:
            printf("ArrayRef(");
            ast_print(node->data.AST_TAB.id);
            printf("[");
            DimEntry *dim = node->data.AST_TAB.dims;
            while (dim) {
                ast_print(dim->dim); // print the AST for the dimension
                if (dim->next) printf("][");
                dim = dim->next;
            }
            printf("])");
            break;
        case AST_IF:
            printf("if (");
            ast_print(node->data.AST_IF.cond);
            printf(") ");
            ast_print(node->data.AST_IF.then_branch);
            if (node->data.AST_IF.else_branch) {
                printf(" else ");
                ast_print(node->data.AST_IF.else_branch);
            }
            printf("\n");
            break;
        case AST_SWITCH:
            printf("switch (");
            ast_print(node->data.AST_SWITCH.expr);
            printf(") {\n");
            CaseEntry *c = node->data.AST_SWITCH.cases;
            while (c) {
                printf("  case ");
                ast_print(c->value);
                printf(": ");
                ast_print(c->body);
                printf("\n");
                c = c->next;
            }
            if (node->data.AST_SWITCH.default_case) {
                printf("  default: ");
                ast_print(node->data.AST_SWITCH.default_case);
                printf("\n");
            }
            printf("}\n");
            break;
        case AST_WHILE:
            printf("while (");
            ast_print(node->data.AST_WHILE.cond);
            printf(") ");
            ast_print(node->data.AST_WHILE.body);
            printf("\n");
            break;
        case AST_MOINS:
            printf("(-");
            ast_print(node->data.AST_MOINS.op);
            printf(")");
            break;
        default:
            printf("Unknown");
            break;
    }
}
void ast_print_tree_param_list(ParamEntry *param, const char *prefix, bool is_last) {
    int count = 0;
    while (param) {
        ast_print_tree(param->param, prefix, param->next == NULL);
        param = param->next;
        count++;
    }
}
void ast_print_tree(AST *node, const char *prefix, bool is_last) {
    if (!node ) return;

    printf("%s%s- ", prefix, is_last ? "`" : "|");
    switch (node->tag) {

        case AST_NUMBER:
            printf("Number: %d\n", node->data.AST_NUMBER.number);
            break;
        case AST_BREAK:
            printf("Break\n");
            break;
        case AST_RETURN:
            printf("Return\n");
            if (node->data.AST_RETURN.expr) {
                char new_prefix[256];
                snprintf(new_prefix, sizeof(new_prefix), "%s%s  ", prefix, is_last ? " " : "|");
                ast_print_tree(node->data.AST_RETURN.expr, new_prefix, true);
            }
            break;
        case AST_ADD:
            printf("Add\n");
            {
                char new_prefix[256];
                snprintf(new_prefix, sizeof(new_prefix), "%s%s  ", prefix, is_last ? " " : "|");
                ast_print_tree(node->data.AST_BINOP.left, new_prefix, false);
                ast_print_tree(node->data.AST_BINOP.right, new_prefix, true);
            }
            break;

        case AST_MUL:
            printf("Multiply\n");
            {
                char new_prefix[256];
                snprintf(new_prefix, sizeof(new_prefix), "%s%s  ", prefix, is_last ? " " : "|");
                ast_print_tree(node->data.AST_BINOP.left, new_prefix, false);
                ast_print_tree(node->data.AST_BINOP.right, new_prefix, true);
            }
            break;

        case AST_BINOP:
            printf("BinOp");
            if (node->data.AST_BINOP.op)
                printf(" [%s]", node->data.AST_BINOP.op);
            printf("\n");
            {
                char new_prefix[256];
                snprintf(new_prefix, sizeof(new_prefix), "%s%s  ", prefix, is_last ? " " : "|");
                ast_print_tree(node->data.AST_BINOP.left, new_prefix, false);
                ast_print_tree(node->data.AST_BINOP.right, new_prefix, true);
            }
            break;
        case AST_FOR:
            printf("For\n");
            {
                char new_prefix[256];
                snprintf(new_prefix, sizeof(new_prefix), "%s%s  ", prefix, is_last ? " " : "|");
                printf("%s|- Init: ", new_prefix);
                ast_print_tree(node->data.AST_FOR.init, new_prefix, false);
                printf("%s|- Cond: ", new_prefix);
                ast_print_tree(node->data.AST_FOR.cond, new_prefix, false);
                printf("%s|- Post: ", new_prefix);
                ast_print_tree(node->data.AST_FOR.post, new_prefix, false);
                printf("%s`- Body: ", new_prefix);
                ast_print_tree(node->data.AST_FOR.body, new_prefix, true);
            }
            break;
        case AST_ID:
            printf("Id: %s\n", node->data.AST_ID.id);
            break;

        case AST_AFF:
            printf("Assign\n");
            {
                char new_prefix[256];
                snprintf(new_prefix, sizeof(new_prefix), "%s%s  ", prefix, is_last ? " " : "|");
                ast_print_tree(node->data.AST_AFF.op1, new_prefix, false);
                ast_print_tree(node->data.AST_AFF.op2, new_prefix, true);
            }
            break;
        case AST_IF:
            printf("If\n");
            {
                char new_prefix[256];
                snprintf(new_prefix, sizeof(new_prefix), "%s%s  ", prefix, is_last ? " " : "|");
                ast_print_tree(node->data.AST_IF.cond, new_prefix, false);
                ast_print_tree(node->data.AST_IF.then_branch, new_prefix, node->data.AST_IF.else_branch == NULL);
                if (node->data.AST_IF.else_branch) {
                    ast_print_tree(node->data.AST_IF.else_branch, new_prefix, true);
                }
            }
            break;
        case AST_SWITCH:
            printf("Switch\n");
            {
                char new_prefix[256];
                snprintf(new_prefix, sizeof(new_prefix), "%s%s  ", prefix, is_last ? " " : "|");
                printf("%s|- Expr: ", new_prefix);
                ast_print(node->data.AST_SWITCH.expr);
                printf("\n");
                CaseEntry *c = node->data.AST_SWITCH.cases;
                int idx = 0;
                while (c) {
                    printf("%s|- Case[%d]: ", new_prefix, idx++);
                    ast_print(c->value);
                    printf(" -> ");
                    ast_print_tree(c->body, new_prefix, false);
                    c = c->next;
                }
                if (node->data.AST_SWITCH.default_case) {
                    printf("%s`- Default: ", new_prefix);
                    ast_print_tree(node->data.AST_SWITCH.default_case, new_prefix, true);
                }
            }
            break;
        case AST_BLOCK:
            {
                printf("Block -- %d\n", node->data.AST_BLOCK.counter);
                InstructEntry *entry = node->data.AST_BLOCK.instructs;

                if (!entry) {
                    printf("  (Empty Block)\n");
                    break;
                }

                char new_prefix[256];
                int is_last_inst;
                while (entry) {
                    if (!entry->instruct) {
                        printf("  (Null Instruction)\n");
                    } else {
                        is_last_inst = (entry->next == NULL);
                        snprintf(new_prefix, sizeof(new_prefix), "%s%s  ", prefix, is_last ? " " : "|");
                        ast_print_tree(entry->instruct, new_prefix, is_last_inst);
                    }
                    entry = entry->next;
                }
            }
            break;
        case AST_WHILE:
            printf("While\n");
            {
                char new_prefix[256];
                snprintf(new_prefix, sizeof(new_prefix), "%s%s  ", prefix, is_last ? " " : "|");
                printf("%s|- Cond: ", new_prefix);
                ast_print_tree(node->data.AST_WHILE.cond, new_prefix, false);
                printf("%s`- Body: ", new_prefix);
                ast_print_tree(node->data.AST_WHILE.body, new_prefix, true);
            }
            break;
        case AST_VLPT:
            printf("VLPT [");
            if (node->data.AST_VLPT.id)
                printf("id: %s, ", node->data.AST_VLPT.id);
            printf("type: ");
            switch (node->data.AST_VLPT.vlpt_type) {
                case VLPT_FUNC_CALL: printf("FUNC_CALL"); break;
                case VLPT_ARRAY_REF: printf("ARRAY_REF"); break;
                default: printf("UNKNOWN"); break;
            }
            printf("]\n");
            {
                char new_prefix[256];
                snprintf(new_prefix, sizeof(new_prefix), "%s%s  ", prefix, is_last ? " " : "|");
                //ast_print_tree_param_list(node->data.AST_VLPT.params, new_prefix, true);
            }
            break;
        case AST_MOINS:
            printf("Minus\n");
            {
                char new_prefix[256];
                snprintf(new_prefix, sizeof(new_prefix), "%s%s  ", prefix, is_last ? " " : "|");
                ast_print_tree(node->data.AST_MOINS.op, new_prefix, true);
            }
            break;
        case AST_TAB:
            printf("ArrayRef\n");
            {
                char new_prefix[256];
                snprintf(new_prefix, sizeof(new_prefix), "%s%s  ", prefix, is_last ? " " : "|");
                ast_print_tree(node->data.AST_TAB.id, new_prefix, false);
                DimEntry *dim = node->data.AST_TAB.dims;
                int dim_idx = 0;
                while (dim) {
                    printf("%s%s- Dim[%d]: ", new_prefix, (dim->next == NULL) ? "`" : "|", dim_idx);
                    // Print the actual expression for the dimension
                    ast_print(dim->dim);
                    printf("\n");
                    // No recursive call to ast_print or ast_print_tree here
                    dim = dim->next;
                    dim_idx++;
                }
            }
            break;

        default:
            printf("Unknown\n");
            break;
    }
    
}
