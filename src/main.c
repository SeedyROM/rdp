/**
 * @file main.c
 * @author Zack Kollar (SeedyROM) (me@seedyrom.io)
 * @brief
 * @version 0.1
 * @date 2021-12-10
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <stdio.h>

#include "parser.tab.h"
#include "lex.yy.h"

#include "utarray.h"
#include "ast.h"

// #define DEBUG

void _print_ast_debug_inner(ast_node *node, size_t max_depth, size_t depth)
{
  if (depth >= max_depth)
    return;

  ast_node *child = NULL;

  switch (node->type)
  {
  case ast_OBJECT:
    printf("Object:\n");

    while ((child = (ast_node *)utarray_next((UT_array *)node->value, child)))
    {
      _print_ast_debug_inner(child, max_depth, depth + 1);
    }
    break;

  case ast_ARRAY:
    printf("Array:\n");

    while ((child = (ast_node *)utarray_next((UT_array *)node->value, child)))
    {
      _print_ast_debug_inner(child, max_depth, depth + 1);
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

  default:
    fprintf(stderr, "Invalid node %s\n", node->type);
    exit(1);
    return;
  }
}

void print_ast_debug(ast_node *root)
{
  size_t max_depth = 16;
  size_t depth = 0;
  return _print_ast_debug_inner(root, max_depth, depth);
}

int main(int argc, char *argv[])
{

  // Open a file
  FILE *f = fopen("./test.txt", "r");
  if (f == NULL)
  {
    fprintf(stderr, "Failed to load file...\n");
    return 1;
  }

  // AST
  ast_node *root_node = ast_node_object();

  // Setup the scanner and parser state
  yyscan_t scanner;
  int status, token;
  YYSTYPE yylval;
  YYLTYPE yyloc = {0};

  yypstate *ps = yypstate_new();
  yylex_init(&scanner);

  // Load a test file
  yyset_in(f, scanner);

  // Set debug
#ifdef DEBUG
  yyset_debug(1, scanner); // For Flex (no longer a global, but rather a member of yyguts_t)
  yydebug = 1;             // For Bison (still global, even in a reentrant parser)
#endif

  // Push tokens from yylex into our parser, no globals or pulling from lex
  do
  {
    token = yylex(&yylval, &yyloc, scanner);
    status = yypush_parse(ps, token, &yylval, &yyloc, root_node);
  } while (status == YYPUSH_MORE);

  print_ast_debug(root_node);

  // Free stuff up
  yypstate_delete(ps);
  // TODO: Free ast nodes
  fclose(f);

  return 0;
}