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

%option header-file="include/lex.yy.h"

%{
  #include <string.h>
  #include <parser.tab.h>  

  extern void yyerror(YYLTYPE* loc, struct ast_node *document, const char *s);
%}

blank [ \t]
identifier [a-zA-Z][a-zA-Z_0-9]*

%x SINGLE_STRING
%x SINGLE_LINE_COMMENT

%%

{blank}            ;
\n                 { return T_EOL; }
{identifier}       { yylval->string = strdup(yytext); return T_IDENT; }

\"                 BEGIN(SINGLE_STRING);
<SINGLE_STRING>{
  \n               {
                      yyerror(yyscanner, NULL, "unexpected EOL for open \"");
                      return 0;
                   }
  <<EOF>>          {
                      yyerror(yyscanner, NULL, "unexpected EOF for open \"");
                      return 0;
                   }
  ([^\\\"]|\\.)*   { 
                      yylval->string = strdup(yytext);
                      return T_STRING; 
                    }
  \"               BEGIN(INITIAL);
  .                ;
}

#                  BEGIN(SINGLE_LINE_COMMENT);
<SINGLE_LINE_COMMENT>{
  \n               BEGIN(INITIAL);
  .                ;
}

%%