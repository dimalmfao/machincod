#ifndef _DECLARATIONS_H
#define _DECLARATIONS_H

#include <args.h>
#include <tokens.h>
#include <expressions.h>
#include <statements.h>

enum DeclType {
    VARIABLE,
    FUNCTION
};

// initialize the declaration type with symbol table

typedef struct decl_ {
    enum DeclType decl_type;
    char *name;
    Expression_t *expr;         // for variable declaration only
    Type_s type;
    struct statement_ *code; // for function only
    Args_t *args;            // for function only   
    struct _symbol *sym;
} Declaration_t;

// dec_create_var token_t **token, char, *name, Declaration_t *decl);
// same with declaration_create_func, but i didnt set any type recognition

Declaration_t *declaration_create_var(Token_t **token, char *name, Type_s type);
Declaration_t *declaration_create_func(Token_t **token, char *name, Declaration_t *decl);

void decl_init(Declaration_t *decl);

void free_declaration(Declaration_t *decl);

#endif 