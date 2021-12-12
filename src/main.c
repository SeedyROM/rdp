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

#include "document.h"

int main(int argc, char *argv[])
{
  document *doc = parse_document("./test.txt");
  ast_print_debug(doc);

  // TODO: Free the document, figure out the ICD other shit and recursive free
  return 0;
}