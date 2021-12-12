/**
 * @file document.c
 * @author Zack Kollar (SeedyROM) (me@seedyrom.io)
 * @brief 
 * @version 0.1
 * @date 2021-12-12
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "document.h"

#include "utarray.h"
#include "ast.h"

#include "parser.tab.h"
#include "lex.yy.h"

ast_node *parse_document(const char *file_path)
{
  // Open a file
  FILE *f = fopen(file_path, "r");
  if (f == NULL)
  {
    fprintf(stderr, "Failed to load file...\n");
    exit(1); // TODO: Crash for now
  }

  // AST root node, it will always begin with an object
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

  // Free stuff up
  yypstate_delete(ps);
  // TODO: Free ast nodes
  fclose(f);

  return root_node;
}