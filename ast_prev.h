#include <stdlib.h>  
#include <stdio.h>  
#include <stdbool.h>


typedef struct AST AST;  

struct AST {  
  enum {  
    AST_NUMBER,  
    AST_ID,
    AST_AFF,
    AST_BLOCK,

    AST_ADD,  
    AST_MUL,  
  } tag;  
  union {  
    struct {  
      int number;  
    } AST_NUMBER;  
    struct {  
        char *id;  
    } AST_ID;  
    struct {  
        AST *op1;  
        AST *op2;   
    } AST_AFF;  
    struct {
        AST **insts;
        int count;
    } AST_BLOCK;
    
          
    
    struct {  // you can do this with making op general or make a struct for every operation (AST_ADD, AST_MUL ...)
      char* op;
      AST *left;  
      AST *right;  
    } AST_BINOP; // reuse for both ADD and MUL  
  } data;  
};  

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

AST *ast_new_block(AST **insts, int count) {
    AST *node = malloc(sizeof(AST));
    if (node) {
        node->tag = AST_BLOCK;
        node->data.AST_BLOCK.insts = insts;
        node->data.AST_BLOCK.count = count;
    }
    return node;
}

AST *ast_new_binop(int tag, AST *left, AST *right) {  
    AST *node = malloc(sizeof(AST));  
    if (node) {  
        node->tag = tag;  
        node->data.AST_BINOP.left = left;  
        node->data.AST_BINOP.right = right;  
    }  
    return node;  
}  

// Macro for easier node creation  
#define AST_NEW_NUMBER(n) ast_new_number(n)  
#define AST_NEW_ADD(l, r) ast_new_binop(AST_ADD, l, r)  
#define AST_NEW_MUL(l, r) ast_new_binop(AST_MUL, l, r)  

AST *term;  

void initialize_term() {  
    term = AST_NEW_ADD(  
        AST_NEW_NUMBER(4),  
        AST_NEW_ADD(  
            AST_NEW_MUL(  
                AST_NEW_NUMBER(2),  
                AST_NEW_NUMBER(10)  
            ),  
            AST_NEW_MUL(  
                AST_NEW_NUMBER(3),  
                AST_NEW_ADD(  
                    AST_NEW_NUMBER(5),  
                    AST_NEW_NUMBER(1)  
                )  
            )  
        )  
    );  
}  

void ast_print(AST *node) {  
    if (!node) return;  
    switch (node->tag) {  
        case AST_NUMBER:  
            printf("%d", node->data.AST_NUMBER.number);  
            break;  
        case AST_ADD:  
            printf("(");  
            ast_print(node->data.AST_BINOP.left);  
            printf(" + ");  
            ast_print(node->data.AST_BINOP.right);  
            printf(")");  
            break;  
        case AST_MUL:  
            printf("(");  
            ast_print(node->data.AST_BINOP.left);  
            printf(" * ");  
            ast_print(node->data.AST_BINOP.right);  
            printf(")");  
            break;  
    }  
}  

void print_indent(int indent) {
    for (int i = 0; i < indent; i++) {
        printf("  "); // two spaces per indent level
    }
}


void ast_print_tree(AST *node, const char *prefix, bool is_last);

void ast_print_tree(AST *node, const char *prefix, bool is_last) {
    if (!node) return;

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
            printf("Block\n");
            for (int i = 0; i < node->data.AST_BLOCK.count; ++i) {
                bool is_last_inst = (i == node->data.AST_BLOCK.count - 1);
                char new_prefix[256];
                snprintf(new_prefix, sizeof(new_prefix), "%s%s  ", prefix, is_last ? " " : "|");
                ast_print_tree(node->data.AST_BLOCK.insts[i], new_prefix, is_last_inst);
            }
            break;
        
        default:
            printf("Unknown\n");
            break;
    }
}




