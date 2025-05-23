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

typedef struct ParamEntry
{
    struct AST *param;
    struct ParamEntry *next;

}ParamEntry;

typedef struct DimEntry
{
    AST* dim;
    struct DimEntry *next;

}DimEntry;

typedef struct CaseEntry {
    AST *value;           // Expression for the case (usually a constant)
    AST *body;            // Instruction or block for this case
    struct CaseEntry *next;
} CaseEntry;



struct AST {
    enum {
        AST_NUMBER,
        AST_WHILE,
        AST_SWITCH,
        AST_ID,
        AST_AFF,
        AST_BLOCK,
        AST_ADD, //DEPRECATED 
        AST_MUL, //DEPRECATED 
        AST_VLPT, // variable length parameters tree (like function parameters , switch-cases ....) 
        AST_BINOP,  
        AST_TAB,
        AST_MOINS,
        AST_FOR,
        AST_IF,
        AST_BREAK,
        AST_RETURN,
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
            AST *init;        // initial assignment
            AST *cond;        // loop condition
            AST *post;        // post-condition (increment)
            AST *body;        // loop body
        } AST_FOR;
        struct { } AST_BREAK; // rien à stocker
        struct { AST *expr; } AST_RETURN; //
        struct { // If/Else
            AST *cond;
            AST *then_branch;
            AST *else_branch; // NULL if no else
        } AST_IF;
        struct {
            AST *op1;
            AST *op2;
        } AST_AFF;
        struct { // Switch-case
            AST *expr;         // Expression to match
            CaseEntry *cases;  // Linked list of cases
            AST *default_case; // Optional default case (can be NULL)
        } AST_SWITCH;
        struct {
            InstructEntry *instructs;
            int counter;
        } AST_BLOCK;
        struct {
            AST *cond;   // loop condition
            AST *body;   // loop body
        } AST_WHILE;
        struct {
            AST *op;
        } AST_MOINS;
        struct { // Array reference
            DimEntry *dims;
            AST *id;
        } AST_TAB;
        struct {
            char *op;
            AST *left;
            AST *right;
        } AST_BINOP;
        struct  //variable length parameter tree : blocks | functions calling | switch statements | tab ref 
        {   
            char *id ;
            ParamEntry *params;
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
AST *ast_new_vlpt(char* id,ParamEntry *params, int vlpt_type);
AST *ast_new_binop(char* operation, AST *left, AST *right);
AST *ast_new_moins(AST *op);
AST *ast_new_tab(DimEntry *dims, AST *id);
AST *ast_new_if(AST *cond, AST *then_branch, AST *else_branch);
AST *ast_new_switch(AST *expr, CaseEntry *cases, AST *default_case);
AST *ast_new_for(AST *init, AST *cond, AST *post, AST *body);
AST *ast_new_while(AST *cond, AST *body);
AST *ast_new_break(void);
AST *ast_new_return(AST *expr);
CaseEntry *case_entry_new(AST *value, AST *body, CaseEntry *next);
void add_bloc_instr(AST *instr);
void initialize_term();
void ast_print(AST *node);
void ast_print_tree(AST *node, const char *prefix, bool is_last);

#endif // AST_H