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

int main(int argc, char *argv[])
{

  // Open a file
  FILE *f = fopen("./test.txt", "r");
  if (f == NULL)
  {
    fprintf(stderr, "Failed to load file...\n");
    return 1;
  }

  // AST, it will always begin with an object
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

  // Verify that we actually did what we did by eye for now
  ast_print_debug(root_node);

  // Free stuff up
  yypstate_delete(ps);
  // TODO: Free ast nodes
  fclose(f);

  return 0;
}