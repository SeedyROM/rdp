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

typedef union
{
  int int_value;
  double double_value;
  char *string_value;
  UT_array *children;
} ast_node_value;

typedef struct _ast_node
{
  ast_node_type type;
  ast_node_value *value;
} ast_node;

static UT_icd ast_node_icd = {sizeof(ast_node), NULL, NULL, NULL};

ast_node *ast_node_new(ast_node_type type, ast_node_value *value);
ast_node *ast_node_null();
ast_node *ast_node_bool(int value);
ast_node *ast_node_object();
ast_node *ast_node_array();

int ast_node_object_append(ast_node *root, ast_node *node);
int ast_node_array_append(ast_node *root, ast_node *node);