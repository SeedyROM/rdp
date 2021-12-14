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
#include <string.h>

#include "ast.h"

/**
 * @brief Used to generate nodes of any type, used by all general helper methods
 * 
 * @param type 
 * @param value 
 * @return ast_node* 
 */
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

/**
 * @brief Create a null (empty) node
 * 
 * @return ast_node* 
 */
ast_node *ast_node_null()
{
  return ast_node_new(ast_NULL, NULL);
}

/**
 * @brief Create a bool node
 * 
 * @param value 
 * @return ast_node* 
 */
ast_node *ast_node_bool(int value)
{
  return ast_node_new(ast_BOOL, (ast_node_value *)&value);
}

/**
 * @brief Create a identifier node
 * 
 * @param value 
 * @return ast_node* 
 */
ast_node *ast_node_ident(char *value)
{
  return ast_node_new(ast_IDENT, (ast_node_value *)value); // Maybe don't dupe it?
}

/**
 * @brief Create a string node
 * 
 * @param value 
 * @return ast_node* 
 */
ast_node *ast_node_string(char *value)
{
  return ast_node_new(ast_STRING, (ast_node_value *)value); // Maybe don't dupe it?
}

/**
 * @brief Create an object node
 * 
 * @return ast_node* 
 */
ast_node *ast_node_object()
{
  UT_array *nodes;
  utarray_new(nodes, &ast_node_icd);

  return ast_node_new(ast_OBJECT, (ast_node_value *)nodes);
}

/**
 * @brief Create an array node 
 * 
 * @return ast_node* 
 */
ast_node *ast_node_array()
{
  UT_array *nodes;
  utarray_new(nodes, &ast_node_icd);

  return ast_node_new(ast_ARRAY, (ast_node_value *)nodes);
}

/**
 * @brief Append a node to an object, typically going to be a kv_pair
 * 
 * @param root 
 * @param node 
 * @return int 
 */
int ast_node_object_append(struct ast_node *root, ast_node *node)
{
  if (root->type != ast_OBJECT && node->type != ast_KEY_PAIR)
  {
    fprintf(stderr, "Cannot append %s to node of type %s\n", node->type, root->type);
    return 0;
  }

  utarray_push_back((UT_array *)root->value, node);
  return 1;
}

/**
 * @brief Create a key pair to be used in an object
 * 
 * @param key 
 * @param value 
 * @return ast_node* 
 */
ast_node *ast_node_key_pair(char *key, ast_node *value)
{
  ast_key_pair *key_pair = malloc(sizeof(ast_key_pair));
  if (key_pair == NULL)
  {
    fprintf(stderr, "Failed to allocate ast_node on the heap\n");
    return NULL;
  }

  key_pair->key = key;
  key_pair->value = value;

  return ast_node_new(ast_KEY_PAIR, (ast_node_value*)key_pair);
}

/**
 * @brief Append a node to an array
 * 
 * @param root 
 * @param node 
 * @return int 
 */
int ast_node_array_append(struct ast_node *root, ast_node *node)
{
  if (root->type != ast_ARRAY)
  {
    fprintf(stderr, "Cannot append %s to node of type %s\n", node->type, root->type);
    return 0;
  }

  utarray_push_back((UT_array *)root->value, node);
  return 1;
}

inline void __ast_print_debug_indent(size_t depth)
{
  printf("%0*s", depth * 2, ""); // This simplifies the printf callback and prints * N many spaces, 2 for now
}

void __ast_print_debug_inner(ast_node *node, size_t max_depth, size_t depth)
{
  // Stop recursing at a specified depth
  if (depth >= max_depth)
    return;

  // Used to handle object and array recursive calls
  ast_node *child = NULL;

  // Help with prettier printing
  __ast_print_debug_indent(depth);

  // Print info based on the node type
  switch (node->type)
  {
  case ast_OBJECT:
    printf("Object:\n");

    while ((child = (ast_node *)utarray_next((UT_array *)node->value, child)))
    {
      __ast_print_debug_inner(child, max_depth, depth + 1);
    }
    break;

  case ast_KEY_PAIR:
    ast_key_pair* pair = (ast_key_pair*)node->value;
    printf("Key: %s\n", pair->key);
    __ast_print_debug_indent(depth);
    printf("Value: \n");
    __ast_print_debug_inner(pair->value, max_depth, depth + 1);
    break;

  case ast_ARRAY:
    printf("Array:\n");

    while ((child = (ast_node *)utarray_next((UT_array *)node->value, child)))
    {
      __ast_print_debug_inner(child, max_depth, depth + 1);
    }
    break;

  case ast_NULL:
    printf("Null\n");
    break;

  case ast_BOOL:
    printf("Bool: %d\n", node->value);
    break;

  case ast_IDENT:
    printf("Ident: %s\n", node->value);
    break;

  case ast_STRING:
    printf("String: \"%s\"\n", node->value);
    break;

  default:
    fprintf(stderr, "Invalid node %s\n", node->type);
    exit(1);
    return;
  }
}

/**
 * @brief Debug print an ast for sanity's sake
 * 
 * @param root 
 */
void ast_print_debug(ast_node *root)
{
  size_t max_depth = 32; // TODO: This should be configurable
  size_t depth = 0;
  return __ast_print_debug_inner(root, max_depth, depth);
}