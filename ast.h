#ifndef AST_H
#define AST_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

/* VLPT type macros */
#define VLPT_FUNC_CALL 1
#define VLPT_ARRAY_REF 2
// Add more as needed


typedef struct AST AST;
typedef struct InstructEntry InstructEntry;

struct InstructEntry {
    struct AST *instruct;
    struct InstructEntry *next;
};

typedef struct Param
{
    struct AST *instruct;
    struct Param *next;
}Param;




struct AST {
    enum {
        AST_NUMBER,
        AST_ID,
        AST_AFF,
        AST_BLOCK,
        AST_ADD, //DEPRECATED 
        AST_MUL, //DEPRECATED 
        AST_VLPT, // variable length parameters tree (like function parameters , switch-cases ....) 
        AST_BINOP,  
        AST_MOINS,
        AST_FUNC,
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
            InstructEntry *instructs;
            int counter;
        } AST_BLOCK;
        struct {
            AST *op;
        } AST_MOINS;
        struct {
            char *op;
            AST *left;
            AST *right;
        } AST_BINOP;
        struct  //variable length parameter tree : blocks | functions calling | switch statements | tab ref 
        {   
            char *id ;
            AST *params;
            int vlpt_type; // 1: function call, 2: array ref, etc.
        } AST_VLPT;//variable length parameter tree : blocks | functions calling | switch statements | tab ref 
        
        
    } data;
};

AST *term;
AST *parent_term;
AST *first_child_term;
AST *second_child_term;

// Function declarations
AST *ast_new_number(int number);
AST *ast_new_id(char *id);
AST *ast_new_aff(AST *id, AST *number);
AST *ast_new_block(struct InstructEntry *instructs);
AST *ast_new_vlpt(char* id,AST *params, int vlpt_type);
AST *ast_new_binop(char* operation, AST *left, AST *right);
AST *ast_new_moins(AST *op);
void add_bloc_instr(AST *instr);
void initialize_term();
void ast_print(AST *node);
void ast_print_tree(AST *node, const char *prefix, bool is_last);

#endif // AST_H