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

#include "ast.h"


int main(int argc, char *argv[])
{
  ast_node *root = ast_node_object();
  ast_node *node = ast_node_null();

  ast_node_object_append(root, node);


  FILE *f = fopen("./test.txt", "r");
  if (f == NULL)
  {
    fprintf(stderr, "Failed to load file...\n");
    return 1;
  }

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
#ifdef __DEBUG__
  yyset_debug(1, scanner); // For Flex (no longer a global, but rather a member of yyguts_t)
  yydebug = 1;             // For Bison (still global, even in a reentrant parser)
#endif

  do
  {
    token = yylex(&yylval, &yyloc, scanner);
    status = yypush_parse(ps, token, &yylval, &yyloc);
  } while (status == YYPUSH_MORE);

  yypstate_delete(ps);

  return 0;
}