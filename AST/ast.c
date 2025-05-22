#include <stdio.h>  
#include <stdlib.h>  
#include "ast.h"  

// Definition of AST struct  
struct AST {  
    enum {  
        AST_NUMBER,  
        AST_ADD,  
        AST_MUL  
    } tag;  
    union {  
        struct {  
            int number;  
        } AST_NUMBER;  
        struct {  
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

AST *ast_new_binop(int tag, AST *left, AST *right) {  
    AST *node = malloc(sizeof(AST));  
    if (node) {  
        node->tag = tag;  
        node->data.AST_BINOP.left = left;  
        node->data.AST_BINOP.right = right;  
    }  
    return node;  
}  

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

int main(){
    initialize_term();
    ast_print(term);
}