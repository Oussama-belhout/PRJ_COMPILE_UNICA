#ifndef GRAPHGEN_H
#define GRAPHGEN_H

#include <stdio.h>
#include "ast.h"

// Writes the AST as a DOT graph to the given file.
void ast_to_dot(FILE *f, AST *root);
void transpile(FuncEntry *functions);
#endif // GRAPHGEN_H