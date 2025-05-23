#include <stdio.h>
#include "graphgen.h"
#include "ast.h"

// Unique node ID generator for DOT output
static int dot_node_counter = 0;
static int get_next_dot_id() {
    return dot_node_counter++;
}

void ast_to_dot_rec(FILE *f, AST *node, int parent_id, const char *edge_label);

void ast_to_dot(FILE *f, AST *root) {
    dot_node_counter = 0;
    fprintf(f, "digraph AST {\n");
    if (root)
        ast_to_dot_rec(f, root, -1, NULL);
    fprintf(f, "}\n");

}

void ast_to_dot_rec(FILE *f, AST *node, int parent_id, const char *edge_label) {
    if (!node) return;
    int my_id = get_next_dot_id();
    printf(" getting to node ");ast_print(node);

    // Print this node
    switch (node->tag) {
        case AST_NUMBER:
            fprintf(f, "  n%d [label=\"%d\", shape=box];\n", my_id, node->data.AST_NUMBER.number);
            break;
        case AST_ID:
            fprintf(f, "  n%d [label=\"%s\", shape=ellipse];\n", my_id, node->data.AST_ID.id);
            break;
        case AST_AFF:
            fprintf(f, "  n%d [label=\":=\", shape=diamond];\n", my_id);
            break;
        case AST_BINOP:
            fprintf(f, "  n%d [label=\"%s\", shape=diamond];\n", my_id, node->data.AST_BINOP.op ? node->data.AST_BINOP.op : "?");
            break;
        case AST_MOINS:
            fprintf(f, "  n%d [label=\"-\", shape=diamond];\n", my_id);
            break;
        case AST_FOR:
            fprintf(f, "  n%d [label=\"for\", shape=box];\n", my_id);
            break;
        case AST_WHILE:
            fprintf(f, "  n%d [label=\"while\", shape=box];\n", my_id);
            break;
        case AST_IF:
            fprintf(f, "  n%d [label=\"if\", shape=box];\n", my_id);
            break;
        case AST_BLOCK:
            fprintf(f, "  n%d [label=\"block\", shape=box3d];\n", my_id);
            break;
        case AST_VLPT:
            fprintf(f, "  n%d [label=\"call: %s\", shape=box];\n", my_id, node->data.AST_VLPT.id ? node->data.AST_VLPT.id : "");
            break;
        case AST_TAB:
            fprintf(f, "  n%d [label=\"arrayref\", shape=box];\n", my_id);
            break;
        case AST_BREAK:
            fprintf(f, "  n%d [label=\"break\", shape=octagon];\n", my_id);
            break;
        case AST_RETURN:
            fprintf(f, "  n%d [label=\"return\", shape=octagon];\n", my_id);
            break;
        case AST_SWITCH:
            fprintf(f, "  n%d [label=\"switch\", shape=box];\n", my_id);
            break;
        default:
            fprintf(f, "  n%d [label=\"?\", shape=plaintext];\n", my_id);
            break;
    }

    // Print edge from parent to this node
    if (parent_id >= 0) {
        if (edge_label)
            fprintf(f, "  n%d -> n%d [label=\"%s\"];\n", parent_id, my_id, edge_label);
        else
            fprintf(f, "  n%d -> n%d;\n", parent_id, my_id);
    }

    // Recurse on children
    switch (node->tag) {
        case AST_NUMBER:
        case AST_ID:
        case AST_BREAK:
            break;
        case AST_RETURN:
            ast_to_dot_rec(f, node->data.AST_RETURN.expr, my_id, "expr");
            break;
        case AST_AFF:
            ast_to_dot_rec(f, node->data.AST_AFF.op1, my_id, "lhs");
            ast_to_dot_rec(f, node->data.AST_AFF.op2, my_id, "rhs");
            break;
        case AST_BINOP:
            ast_to_dot_rec(f, node->data.AST_BINOP.left, my_id, "left");
            ast_to_dot_rec(f, node->data.AST_BINOP.right, my_id, "right");
            break;
        case AST_MOINS:
            ast_to_dot_rec(f, node->data.AST_MOINS.op, my_id, "op");
            break;
        case AST_FOR:
            ast_to_dot_rec(f, node->data.AST_FOR.init, my_id, "init");
            ast_to_dot_rec(f, node->data.AST_FOR.cond, my_id, "cond");
            ast_to_dot_rec(f, node->data.AST_FOR.post, my_id, "post");
            ast_to_dot_rec(f, node->data.AST_FOR.body, my_id, "body");
            break;
        case AST_WHILE:
            ast_to_dot_rec(f, node->data.AST_WHILE.cond, my_id, "cond");
            ast_to_dot_rec(f, node->data.AST_WHILE.body, my_id, "body");
            break;
        case AST_IF:
            ast_to_dot_rec(f, node->data.AST_IF.cond, my_id, "cond");
            ast_to_dot_rec(f, node->data.AST_IF.then_branch, my_id, "then");
            ast_to_dot_rec(f, node->data.AST_IF.else_branch, my_id, "else");
            break;
        case AST_BLOCK: {
            InstructEntry *entry = node->data.AST_BLOCK.instructs;
            int idx = 0;
            while (entry) {
                char label[16];
                snprintf(label, sizeof(label), "stmt%d", idx++);
                ast_to_dot_rec(f, entry->instruct, my_id, label);
                entry = entry->next;
            }
            break;
        }
        case AST_VLPT: {
            ParamEntry *p = node->data.AST_VLPT.params;
            int idx = 0;
            while (p != NULL) {
                printf("param : ");ast_print(p->param);
                char label[16];
                snprintf(label, sizeof(label), "arg%d", idx++);
                ast_to_dot_rec(f, p->param, my_id, label);
                p = p->next;
            }
            break;
        }
        case AST_TAB: {
            ast_to_dot_rec(f, node->data.AST_TAB.id, my_id, "id");
            DimEntry *dim = node->data.AST_TAB.dims;
            int idx = 0;
            while (dim) {
                char label[16];
                snprintf(label, sizeof(label), "dim%d", idx++);
                ast_to_dot_rec(f, dim->dim, my_id, label);
                dim = dim->next;
            }
            break;
        }
        case AST_SWITCH: {
            ast_to_dot_rec(f, node->data.AST_SWITCH.expr, my_id, "expr");
            CaseEntry *c = node->data.AST_SWITCH.cases;
            int idx = 0;
            while (c) {
                char label[16];
                snprintf(label, sizeof(label), "case%d", idx++);
                int case_id = get_next_dot_id();
                fprintf(f, "  n%d [label=\"case\", shape=box];\n", case_id);
                fprintf(f, "  n%d -> n%d [label=\"%s\"];\n", my_id, case_id, label);
                ast_to_dot_rec(f, c->value, case_id, "value");
                ast_to_dot_rec(f, c->body, case_id, "body");
                c = c->next;
            }
            if (node->data.AST_SWITCH.default_case) {
                int def_id = get_next_dot_id();
                fprintf(f, "  n%d [label=\"default\", shape=box];\n", def_id);
                fprintf(f, "  n%d -> n%d [label=\"default\"];\n", my_id, def_id);
                ast_to_dot_rec(f, node->data.AST_SWITCH.default_case, def_id, "body");
            }
            break;
        }
        default:
            break;
    }
}