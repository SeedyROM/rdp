/**
 * @file ast.c
 * @author Zack Kollar (SeedyROM) (me@seedyrom.io)
 * @brief
 * @version 0.1
 * @date 2021-12-10
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <stdio.h>

#include "ast.h"

ast_node *ast_node_new(ast_node_type type, ast_node_value *value)
{
  ast_node *node = malloc(sizeof(ast_node));
  if (node == NULL)
  {
    fprintf(stderr, "Failed to allocate ast_node on the heap\n");
    return NULL;
  }

  node->type = type;
  node->value = value;
  return node;
}

ast_node *ast_node_null()
{
  return ast_node_new(ast_NULL, NULL);
}

ast_node *ast_node_bool(int value)
{
  return ast_node_new(ast_BOOL, (ast_node_value *)&value);
}

ast_node *ast_node_object()
{
  UT_array *nodes;
  utarray_new(nodes, &ast_node_icd);

  return ast_node_new(ast_OBJECT, (ast_node_value *)nodes);
}

ast_node *ast_node_array()
{
  UT_array *nodes;
  utarray_new(nodes, &ast_node_icd);

  return ast_node_new(ast_ARRAY, (ast_node_value *)nodes);
}

int ast_node_object_append(ast_node *root, ast_node *node)
{
  if (root->type != ast_OBJECT)
  {
    fprintf(stderr, "Cannot append %s to node of type %s\n", node->type, root->type);
    return 0;
  }

  utarray_push_back((UT_array *)root->value, node);
  return 1;
}

int ast_node_array_append(ast_node *root, ast_node *node)
{
  if (root->type != ast_ARRAY)
  {
    fprintf(stderr, "Cannot append %s to node of type %s\n", node->type, root->type);
    return 0;
  }

  utarray_push_back((UT_array *)root->value, node);
  return 1;
}