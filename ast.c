#include "ast.h"
short block_counter = 0 ;

AST *ast_new_vlpt(char* id,AST *params, int vlpt_type) {
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
        ast_print_tree(node, "", true);

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

void ast_print(AST *node) {
    if (!node) return;
    switch (node->tag) {
        case AST_NUMBER:
            printf("%d", node->data.AST_NUMBER.number);
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
            printf("VLPT(");
            if (node->data.AST_VLPT.id)
                printf("id: %s, ", node->data.AST_VLPT.id);
            printf("type: ");
            switch (node->data.AST_VLPT.vlpt_type) {
                case VLPT_FUNC_CALL: printf("FUNC_CALL"); break;
                case VLPT_ARRAY_REF: printf("ARRAY_REF"); break;
                default: printf("UNKNOWN"); break;
            }
            printf(", params: ");
            ast_print(node->data.AST_VLPT.params);
            printf(")");
            break;

        default:
            printf("Unknown");
            break;
    }
}

void ast_print_tree(AST *node, const char *prefix, bool is_last) {
    if (!node ) return;

    printf("%s%s- ", prefix, is_last ? "`" : "|");
    switch (node->tag) {

        case AST_NUMBER:
            printf("Number: %d\n", node->data.AST_NUMBER.number);
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
                ast_print_tree(node->data.AST_VLPT.params, new_prefix, true);
            }
            break;

        default:
            printf("Unknown\n");
            break;
    }
    
}
