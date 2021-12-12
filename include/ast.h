/**
 * @file ast.h
 * @author Zack Kollar (SeedyROM) (me@seedyrom.io)
 * @brief
 * @version 0.1
 * @date 2021-12-10
 *
 * @copyright Copyright (c) 2021
 *
 */

#pragma once

#include "utarray.h"

/**
 * @brief Node type enum
 * 
 */
typedef enum
{
  ast_NULL,
  ast_BOOL,
  ast_IDENT,
  ast_STRING,
  ast_NUMBER,
  ast_ARRAY,
  ast_OBJECT
} ast_node_type;

/**
 * @brief Node value enum
 * 
 */
typedef union
{
  int int_value;
  double double_value;
  char *string_value;
  UT_array *children;
} ast_node_value;

/**
 * @brief An AST node in our document
 * 
 */
typedef struct ast_node
{
  ast_node_type type;
  ast_node_value *value;
} ast_node;

/**
 * @brief The init, copy and destructor for our UT_array of children
 * 
 */
static UT_icd ast_node_icd = {sizeof(ast_node), NULL, NULL, NULL};

// Create a new node of any type, see docs in impl
ast_node *ast_node_new(ast_node_type type, ast_node_value *value);

// Node creation helpers
ast_node *ast_node_null();
ast_node *ast_node_bool(int value);
ast_node *ast_node_ident(char *value);
ast_node *ast_node_string(char *value);
ast_node *ast_node_object();
ast_node *ast_node_array();

// Recursive node helpers
int ast_node_object_append(struct ast_node *root, ast_node *node);
int ast_node_array_append(struct ast_node *root, ast_node *node);

// Debug print helpers
void __ast_print_debug_indent(size_t depth);
void __ast_print_debug_inner(ast_node *node, size_t max_depth, size_t depth);
void ast_print_debug(ast_node *root);
