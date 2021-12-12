/**
 * @file document.h
 * @author Zack Kollar (SeedyROM) (me@seedyrom.io)
 * @brief 
 * @version 0.1
 * @date 2021-12-12
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "ast.h"

typedef ast_node document;

ast_node *parse_document(const char *file_path);