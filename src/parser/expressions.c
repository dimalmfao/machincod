#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <args.h>
#include <expressions.h>
#include <errors/error.h>
#include <struct.h>
#include <symbol_table.h>

Expression_t*
expr_create(Token_t **token, enum Type t)
{
    Token_t *tok = *token;

    Expression_t *expr;

    if (token_checks(tok, 7, TOK_INTEGER, TOK_FLOAT, LPAR, SYMBOL, MINUS, TOK_STRING, TOK_CHAR))
    {
        expr = expr_(&tok, t);
    }

    else if (token_check(tok, KEYWORD))
    {
        if (strcmp(tok->value.p, Machincod_ReservedKeywords[KW_TRUE]) == 0)
        {
            expr = xmalloc(sizeof(Expression_t));
            expr_init(expr);
            expr->expr_type = EXPR_BOOL;
            expr->int_value = 1; // true
            expr->type.t = _BOOL;
        }
        else if (strcmp(tok->value.p, Machincod_ReservedKeywords[KW_FALSE]) == 0)
        {
            expr = xmalloc(sizeof(Expression_t));
            expr_init(expr);
            expr->expr_type = EXPR_BOOL;
            expr->int_value = 0; // false
            expr->type.t = _BOOL;
        }
        else
        {
            show_error_source(tok);
            fprintf(stderr,
                "Invalid expression\n");
            cc_exit();
        }

        tok = tok->next;
    }

    else if (tok->type == EOS)
    {
        return NULL;
    }

    else
    {
        show_error_source(tok);
        fprintf(stderr,
            "Invalid expression\n");
        cc_exit();
    }
    
    *token = tok;
    expr = expr_fold(expr);

    type_evaluate(expr, t);
    type_set(expr, type_of_first_symbol(expr));
    type_check(expr);

    return expr;
}

Expression_t*
expr_factor(Token_t **token, enum Type t)
{
    Token_t *tok = *token;

    Expression_t *expr = xmalloc(sizeof(Expression_t));
    expr_init(expr);

    if (!token_checks(tok, 7, TOK_INTEGER, TOK_FLOAT, TOK_STRING, TOK_CHAR, LPAR, SYMBOL, MINUS))
    {
        show_error_source(tok);
        fprintf(stderr,
            "Invalid syntax\n");
        free_expression(expr);
        cc_exit();
    }

    expr->token = tok;

    if (tok->type == TOK_INTEGER)
    {
        expr->expr_type = EXPR_INTEGER;
        expr->int_value = tok->value.i;
        expr->type.t = t;

        if (t == _VOID || t == _BOOL)
            expr->type.t = INTEGER;

        tok = tok->next;
    }

    else if (tok->type == TOK_STRING)
    {
        expr->expr_type = EXPR_STRING_LITTERAL;
        expr->string_value = tok->value.p;
        expr->type.t = STRING;
        
        tok = tok->next;
    }

    else if (tok->type == TOK_CHAR)
    {
        expr->expr_type = EXPR_CHAR;
        expr->string_value = tok->value.p;
        expr->type.t = _CHAR;

        tok = tok->next;
    }

    else if (tok->type == TOK_FLOAT)
    {
        expr->expr_type = EXPR_FLOAT;
        expr->double_value = tok->value.d;
        expr->type.t = _FLOAT;
        
        tok = tok->next;
    }

    else if (tok->type == LPAR)
    {
        free(expr);
        tok = tok->next;
        expr = expr_(&tok, t);

        if (!token_check(tok, RPAR))
        {
            show_error_source(tok);
            fprintf(stderr,
                "Missing ')'\n");
            free_expression(expr);
            cc_exit();
        }

        tok = tok->next;
    }

    else if (tok->type == MINUS)
    {
        expr->expr_type = EXPR_UNARY_MINUS;
        tok  = tok->next;
        expr->left = expr_(&tok, t);
    }

    else if (tok->type == SYMBOL)
    {
        Symbol_t *sym;
        if (is_declared_var(symtab_g, tok->value.p, &sym) && !token_check(tok->next, LPAR))
        {
            if (is_type_allowed(sym->_type))
            {
                show_error_source(tok);
                fprintf(stderr,
                    "Invalid type in expression\n");
                free_expression(expr);
                cc_exit();
            }

            if (sym->decl && !sym->decl->is_initialised)
            {
                fprintf(stderr,
                    "%sWarning%s: \n\tUse of uninitialised variable '%s'\n",
                    C_YEL,
                    C_NRM,
                    sym->name);
            }
            expr->string_value = tok->value.p;

            if (token_check(tok->next, LBRACKET))
            {

                expr->expr_type = EXPR_ARRAYA;
                expr->type.t = sym->_type.t;
                expr->sym_value = sym;

                tok = tok->next;
                if (!sym->_type.is_array)
                {
                    show_error_source(tok);
                    fprintf(stderr,
                        "Can't index something that is not an array\n");
                    free_expression(expr);
                    cc_exit();
                }

                tok = tok->next;

                expr->access = expr_create(&tok, INTEGER);

                if (!token_check(tok, RBRACKET))
                {
                    show_error_source(tok);
                    fprintf(stderr, "Error\n");
                    cc_exit();
                }

            }
            else if (token_check(tok->next, DOT))
            {
                tok  = tok->next;

                if (!sym->_type.is_structure)
                {
                    show_error_source(tok);
                    fprintf(stderr,
                        "Can't access member of something that is not a structure\n");
                    cc_exit();
                }

                tok = tok->next;

                expr->expr_type = EXPR_STRUCTA;
                if (!token_check(tok, SYMBOL))
                {
                    show_error_source(tok);
                    fprintf(stderr,
                        "Invalid member for structure...\n");
                    free_expression(expr);
                    cc_exit();
                }

                Statement_t *str = get_struct_by_name(sym->_type.ptr);
                Args_t *member = struct_get_member(str, tok->value.p);
                if (!member)
                {
                    show_error_source(tok);
                    fprintf(stderr,
                        "Structure '%s' has no member '%s'\n",
                        (char*)sym->_type.ptr,
                        tok->value.p);
                }
                expr->string_value = member->name;
                expr->type.t = member->type.t;
                expr->sym_value = sym;

            }
            else
            {
                expr->expr_type = EXPR_SYMBOL;
                expr->sym_value = sym;
                expr->type.t = sym->_type.t;
            }

        }

        else if (is_declared_func(symtab_g, tok->value.p, &sym))
        {
            if (is_type_allowed(sym->_type))
            {
                show_error_source(tok);
                fprintf(stderr,
                    "Invalid type in expression\n");
                free_expression(expr);
                cc_exit();
            }

            free(expr);
            char *name = tok->value.p;
            
            tok = tok->next;
            expr = expr_create_funccall(&tok, name);
            expr->expr_type = EXPR_FUNCCALL;

            *token = tok;
            return expr;
        }

        else
        {
            show_error_source(tok);
            fprintf(stderr,
                "Undeclared variable '%s'\n",
                tok->value.p);
            free_expression(expr);
            cc_exit();
        }


        tok = tok->next;
    }

    *token = tok;
    return expr;
}

Expression_t*
expr_term(Token_t **token, enum Type t)
{
    Token_t *tok = *token;

    Expression_t *node = expr_factor(&tok, t);
    Expression_t *tmp;

    while (token_checks(tok, 3, MUL, DIV, MODULO))
    {
        Expression_t *expr = xmalloc(sizeof(Expression_t));

        expr_init(expr);

        if (tok->type == MUL)
            expr->expr_type = EXPR_MUL;
        else if (tok->type == DIV)
            expr->expr_type = EXPR_DIV;
        else 
            expr->expr_type = EXPR_MOD;


        expr->token = tok;
        expr->type.t = t;

        tok = tok->next;

        Expression_t *factor = expr_factor(&tok, t);

        if (!factor)
        {
            show_error_source(tok);
            fprintf(stderr,
                "Invalid syntax\n");
            cc_exit();
        }

        tmp = node;

        node = expr;

        node->left = factor;
        node->right = tmp;
    }

    *token = tok;

    return node;
}

Expression_t*
expr_(Token_t **token, enum Type t)
{
    Token_t *tok = *token;

    static bool is_cond = false;

    Expression_t *node = expr_term(&tok, t);
    Expression_t *tmp;

    if (token_checks(tok, 6, CMP, DIFF, OP_LOWER, OP_LOWER_EQ, OP_GREATER, OP_GREATER_EQ))
    {
        if (is_cond)
        {
            show_error_source(tok);
            fprintf(stderr,
                "Invalid syntax here\n");
            cc_exit();
        }
        is_cond = true;
        tmp = expr_create_cond_left(&tok, node, t);
        is_cond = false;
        *token = tok;
        return tmp;
    }

    while (token_checks(tok, 2, PLUS, MINUS))
    {
        Expression_t *expr = xmalloc(sizeof(Expression_t));

        expr_init(expr);

        if (tok->type == PLUS)
            expr->expr_type = EXPR_PLUS;
        else
            expr->expr_type = EXPR_MINUS;
        

        expr->token = tok;
        expr->type.t = t;

        tok = tok->next;
        Expression_t *term = expr_term(&tok, t);
    
        if (!term)
        {
            show_error_source(tok);
            fprintf(stderr,
                "Invalid syntax error\n");
            cc_exit();
        }

        tmp = node;
        node = expr;

        node->left = term;
        node->right = tmp;
    }

    *token = tok;
    return node;
}

Expression_t*
expr_create_funccall(Token_t **token, char *name)
{
    Token_t *tok = *token;
    Token_t *next_token = tok->next;

    Expression_t *expr = xmalloc(sizeof(Expression_t));

    expr_init(expr);

    expr->expr_type = EXPR_FUNCCALL;
    expr->string_value = name;

    Symbol_t *sym;

    expr->token = tok;

    if (token_check(next_token, RPAR))
    {
        sym = find_matching_function(name, NULL);
        if (!sym)
        {
            show_error_source(next_token);
            fprintf(stderr, 
                "Can't find a function with matching prototype\n");
            free(expr);
            cc_exit();
        }
        expr->sym_value = sym;
        expr->type = sym->_type;
        
        next_token = next_token->next;
        *token = next_token;
        return expr;
    }
    
    else
    {
        expr->args = get_args(&next_token, _VOID);
        if (!token_check(next_token, RPAR))
        {
            show_error_source(next_token);
            fprintf(stderr,
                "Missing ')'\n");
            free_args(expr->args);
            free(expr);
            cc_exit();
        }

        next_token = next_token->next;
    }

    Args_t *c_args = expr->args;

    sym = find_matching_function(name, c_args);
    if (!sym)
    {
        show_error_source(next_token);
        fprintf(stderr, 
            "Can't find a function with matching prototype\n");
        free(expr);
        cc_exit();
    }

    expr->sym_value = sym;
    expr->type = sym->_type;

    *token = next_token;
    return expr;
}

Expression_t*
expr_create_cond(Token_t **token, enum Type t)
{
    Token_t *tok = *token;

    Expression_t *expr = expr_create(&tok, t);
    if (type_evaluate(expr, t).t != _BOOL)
    {
        show_error_source(tok);
        fprintf(stderr,
            "A boolean was expected in condition\n");
        free_expression(expr);
        cc_exit();
    }

    *token = tok;
    return expr;
}

Expression_t*
expr_create_cond_left(Token_t **token, Expression_t *left, enum Type t)
{
    Token_t *tok = *token;

    Expression_t *expr = xmalloc(sizeof(Expression_t));

    expr_init(expr);

    expr->expr_type = EXPR_COND;

    expr->left = left;

    Type_s type = type_of_first_symbol(expr->left);

    if (tok->type == CMP) 
        expr->cond_type = EXPR_CMP;
    else if (tok->type == OP_GREATER) 
        expr->cond_type = EXPR_GREATER;
    else if (tok->type == OP_LOWER) 
        expr->cond_type = EXPR_LOWER;
    else if (tok->type == DIFF) 
        expr->cond_type = EXPR_DIFF;
    else if (tok->type == OP_GREATER_EQ) 
        expr->cond_type = EXPR_GREATER_EQ;
    else if (tok->type == OP_LOWER_EQ)
        expr->cond_type = EXPR_LOWER_EQ;

    tok = tok->next;
    expr->right = expr_create(&tok, type.t);

    type_set(expr, type);

    if ((type_evaluate(expr->left, type.t).t != type_evaluate(expr->right, type.t).t))
    {
        show_error_source(tok);
        fprintf(stderr, 
            "Invalid comparison between '%s' and '%s'\n",
            type_name(type_evaluate(expr->left, type.t).t),
            type_name(type_evaluate(expr->right, type.t).t));
        free_expression(expr);
        cc_exit();
    }

    if (type_evaluate(expr->left, type.t).is_array && ((expr->left->expr_type != EXPR_ARRAYA) || (expr->left->expr_type != EXPR_ARRAYA)))
    {
        show_error_source(tok);
        fprintf(stderr, 
            "Invalid comparison between '%s' and '%s'\n",
            type_name(type_evaluate(expr->left, type.t).t),
            type_name(type_evaluate(expr->right, type.t).t));
        free_expression(expr);
        cc_exit();
    }


    *token = tok;
    return expr;
}

Expression_t*
expr_fold(Expression_t *expr)
{
    if (expr->expr_type == EXPR_COND)
        return expr;

    if (expr->left)
        expr->left = expr_fold(expr->left);

    if (expr->right)
        expr->right = expr_fold(expr->right);

    if (expr->expr_type == EXPR_UNARY_MINUS && expr->left->expr_type == EXPR_INTEGER)
    {
        Expression_t *e = xmalloc(sizeof(Expression_t));
        expr_init(e);
        e->expr_type = EXPR_INTEGER;
        e->type = expr->left->type;
        e->int_value = expr->left->int_value * (-1);
        free_expression(expr->left);
        free(expr);
        return e;
    }

    else if ((expr->left != NULL && expr->left->expr_type == EXPR_INTEGER) && (expr->right != NULL && expr->right->expr_type == EXPR_INTEGER))
    {
        
        Expression_t *e = xmalloc(sizeof(Expression_t));
        expr_init(e);
    
        e->expr_type = EXPR_INTEGER;
        e->type = expr->left->type;


        switch (expr->expr_type)
        {
            case EXPR_PLUS:
                e->int_value = expr->right->int_value + expr->left->int_value;
                break;
            case EXPR_MINUS:
                e->int_value = expr->right->int_value - expr->left->int_value;
                break;
            case EXPR_DIV:
                if (expr->left->int_value == 0)
                {
                    fprintf(stderr, 
                        "Error: \n\tDivision by 0!\n");
                    free_expression(expr);
                    free(e);
                    cc_exit();
                }
                e->int_value = expr->right->int_value / expr->left->int_value;
                break;
            case EXPR_MUL:
                e->int_value = expr->right->int_value * expr->left->int_value;
                break;
            case EXPR_MOD:
                e->int_value = expr->right->int_value % expr->left->int_value;
                break;
            default:
                fprintf(stderr,
                    "Error\n\tUnknow error\n");
                free_expression(expr);
                free(e);
                cc_exit();
                break;
        }

        free_expression(expr->left); expr->left = NULL;
        free_expression(expr->right); expr->right = NULL;
        free(expr);

        return e;

    }
    else
    {
        return expr;
    }
}

void
expr_init(Expression_t *expr)
{
    expr->left = NULL;
    expr->right = NULL;
    expr->args = NULL;
    expr->access = NULL;
    expr->string_value = NULL;
    expr->sym_value = NULL;
    expr->token = NULL;
    expr->sym = NULL;
    expr->reg = UINT_MAX;
    expr->int_value = 0;
    expr->type.is_structure = false;
    expr->type.is_array = false;
}

bool
is_type_allowed(Type_s type)
{
    return (type.t != INTEGER) && (type.t != _BYTE) && (type.t != _CHAR) && (type.t != STRING) && (type.t != STRUCTURE) && (type.t != _FLOAT) && (type.t != _BOOL);
}

void
free_expression(Expression_t *expr)
{

    if (!expr)
        return;

    if (expr->args && expr->expr_type == EXPR_FUNCCALL)
    {
        free_args(expr->args);
        expr->args = NULL;
    }

    if (expr->right)
        free_expression(expr->right);

    if (expr->left)
        free_expression(expr->left);

    if (expr->access)
        free_expression(expr->access);


    if (expr->expr_type == EXPR_STRING_LITTERAL)
        free(expr->string_value);
    
    free(expr);
}