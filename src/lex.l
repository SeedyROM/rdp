/**
 * @file lex.l
 * @author Zack Kollar (SeedyROM) (me@seedyrom.io)
 * @brief 
 * @version 0.1
 * @date 2021-12-10
 * 
 * @copyright Copyright (c) 2021
 * 
 */

%option 8bit reentrant bison-bridge bison-locations
%option warn noyywrap nodefault stack yylineno

%option header-file="src/lex.yy.h"

%{
  #include <string.h>

  #include "parser.tab.h"  
%}

blank [ \t]
identifier [a-zA-Z][a-zA-Z_0-9]*


%%

{blank}       ;
\n            { return T_EOL; }
{identifier}  { yylval->s_val = strdup(yytext); return T_IDENT; }

%%