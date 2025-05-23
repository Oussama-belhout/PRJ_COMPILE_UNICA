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
            fprintf(f, "  n%d [label=\"%d\", shape=ecllipse];\n", my_id, node->data.AST_NUMBER.number);
            break;
        case AST_ID:
            fprintf(f, "  n%d [label=\"%s\", shape=ellipse];\n", my_id, node->data.AST_ID.id);
            break;
        case AST_AFF:
            // Changed shape from diamond to ellipse and label from ":=" to ":=" as per spec [cite: 183, 281, 283]
            fprintf(f, "  n%d [label=\":=\", shape=ellipse];\n", my_id);
            break;
        case AST_BINOP:
            // Changed shape from diamond to ellipse for arithmetic/logical expressions [cite: 187, 285]
            fprintf(f, "  n%d [label=\"%s\", shape=ellipse];\n", my_id, node->data.AST_BINOP.op ? node->data.AST_BINOP.op : "?");
            break;
        case AST_MOINS:
            // Changed shape from diamond to ellipse for unary minus [cite: 187, 285]
            fprintf(f, "  n%d [label=\"-\", shape=ellipse];\n", my_id);
            break;
        case AST_FOR:
            // Changed label to "FOR" and shape to diamond as per spec [cite: 209, 311]
            fprintf(f, "  n%d [label=\"FOR\", shape=diamond];\n", my_id);
            break;
        case AST_WHILE:
            // Changed label to "WHILE" and shape to diamond as per spec [cite: 211, 314]
            fprintf(f, "  n%d [label=\"WHILE\", shape=diamond];\n", my_id);
            break;
        case AST_IF:
            // Changed label to "IF" and shape to diamond as per spec [cite: 201, 303, 304]
            fprintf(f, "  n%d [label=\"IF\", shape=diamond];\n", my_id);
            break;
        case AST_BLOCK:
            // Changed label to "BLOC" and shape to ellipse as per spec [cite: 179, 278]
            fprintf(f, "  n%d [label=\"BLOC\", shape=ellipse];\n", my_id);
            break;
        case AST_VLPT:
            // Changed label to function name and shape to septagon as per spec [cite: 216, 217, 319, 320]
            fprintf(f, "  n%d [label=\"%s\", shape=septagon];\n", my_id, node->data.AST_VLPT.id ? node->data.AST_VLPT.id : "");
            break;
        case AST_TAB:
            // Changed label to "TAB" and shape to ellipse as per spec [cite: 192, 292] (assuming ellipse for data structures)
            fprintf(f, "  n%d [label=\"TAB\", shape=ellipse];\n", my_id);
            break;
        case AST_BREAK:
            // Changed label to "BREAK" and shape to box as per spec [cite: 214, 316, 317]
            fprintf(f, "  n%d [label=\"BREAK\", shape=box];\n", my_id);
            break;
        case AST_RETURN:
            if (node->data.AST_RETURN.expr != NULL)
            {
                // Changed label to "RETURN" and shape to trapezium and color to blue as per spec [cite: 198, 298]
                fprintf(f, "  n%d [label=\"RETURN\", shape=trapezium color=blue];\n", my_id);
            } 
            break;
        case AST_SWITCH:
            // Changed label to "SWITCH" and shape to diamond (consistent with other control flow) [cite: 206, 309]
            fprintf(f, "  n%d [label=\"SWITCH\", shape=diamond];\n", my_id);
            break;
        case AST_FUNC_DEC:/*
            // Changed label to "function name, type" and shape to invtrapezium and color to blue as per spec [cite: 176, 177, 273, 274]
            // This requires AST_FUNC_DEC to hold function name and type
            // Assuming node->data.AST_FUNC_DEC.id and node->data.AST_FUNC_DEC.type exist
            fprintf(f, "  n%d [label=\"%s, %s\", shape=invtrapezium color=blue];\n", my_id, node->data.AST_FUNC_DEC.id, node->data.AST_FUNC_DEC.type);
            break;*/
        case AST_FUNC:
            // AST_FUNC itself doesn't have a specific node representation based on the description
            // Its children (id and body) will be handled by recursive calls
            // This case might need adjustment depending on how AST_FUNC_DEC and AST_FUNC are intended to work together.
            // For now, it just passes calls to its children, which is fine for structure.
            //ast_to_dot_rec(f, node->data.AST_FUNC.id, my_id, "name");
            fprintf(f, "  n%d [label=\"%s, %s\", shape=invtrapezium, color=blue];\n", my_id,
                node->data.AST_FUNC.id && node->data.AST_FUNC.id->data.AST_ID.id ? node->data.AST_FUNC.id->data.AST_ID.id : "",
                node->data.AST_FUNC.type ? node->data.AST_FUNC.type : "");            
                ast_to_dot_rec(f, node->data.AST_FUNC.body, my_id, "body");
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
            if (node->data.AST_RETURN.expr != NULL) { // Only recurse if a value is returned [cite: 200, 300]
                ast_to_dot_rec(f, node->data.AST_RETURN.expr, my_id, "expr");
            }
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
            ast_to_dot_rec(f, node->data.AST_IF.else_branch, my_id, "else"); // Optional, will be NULL if no else [cite: 205, 307]
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
            ast_to_dot_rec(f, node->data.AST_TAB.id, my_id, "name"); // Changed "id" to "name" as per example [cite: 293]
            DimEntry *dim = node->data.AST_TAB.dims;
            int idx = 0;
            while (dim) {
                char label[16];
                snprintf(label, sizeof(label), "idx%d", idx++); // Changed "dim" to "idx" for clarity
                ast_to_dot_rec(f, dim->dim, my_id, label);
                dim = dim->next;
            }
            break;
        }
        case AST_SWITCH: {
            ast_to_dot_rec(f, node->data.AST_SWITCH.expr, my_id, "expr"); // First child is expression [cite: 207, 309]
            CaseEntry *c = node->data.AST_SWITCH.cases;
            int idx = 0;
            while (c) {
                char label[16];
                snprintf(label, sizeof(label), "case%d", idx++); // Subsequent children are cases [cite: 207, 309]
                int case_id = get_next_dot_id();
                // Cases themselves could be represented as nodes, or their value and body as children of the switch
                // The current implementation creates a "case" node, which is acceptable if not explicitly forbidden
                fprintf(f, "  n%d [label=\"case\", shape=box];\n", case_id);
                fprintf(f, "  n%d -> n%d [label=\"%s\"];\n", my_id, case_id, label);
                ast_to_dot_rec(f, c->value, case_id, "value");
                ast_to_dot_rec(f, c->body, case_id, "body");
                c = c->next;
            }
            if (node->data.AST_SWITCH.default_case) {
                int def_id = get_next_dot_id();
                // Last child is default case [cite: 208, 310]
                fprintf(f, "  n%d [label=\"default\", shape=box];\n", def_id);
                fprintf(f, "  n%d -> n%d [label=\"default\"];\n", my_id, def_id);
                ast_to_dot_rec(f, node->data.AST_SWITCH.default_case, def_id, "body");
            }
            break;
        }
        case AST_FUNC_DEC:
            // For a function declaration, its children would typically be parameters and the function body.
            // Assuming AST_FUNC_DEC stores a list of parameters and a body, similar to AST_FUNC.
            // This part needs to be adapted based on the actual AST structure for function declarations.
            // Example: iterating through parameters and then the body.
            // For instance, if there's a param_list and body:
            // ast_to_dot_rec(f, node->data.AST_FUNC_DEC.param_list, my_id, "params");
            // ast_to_dot_rec(f, node->data.AST_FUNC_DEC.body, my_id, "body");
            break;
        case AST_FUNC:
            // Children already handled above in the node printing section.
            break;
        default:
            break;
    }
}