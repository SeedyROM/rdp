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

/* 
  Alright let's describe the options here:
    - 8bit: read only 1 byte at a time, no unicode here sorry
    - reetrant: ignore global state and generate yylex methods with state than can be passed in AKA: Not global
    - bison-*: is just to link our tools, bridge is to help with params and location help... well..
    - warn: Give us warnings about broken rules in our tokenizer
    - noyywrap: As far as I know, don't handle recursion automatically?
    - nodefault: Do not define a default rule, so don't match EVERYTHING automatically
    - stack: Use a stack based tokenizer to simplify errors? This is up to debate since the docs scare me
    - yylineno: This is supposed to pass the current position in the stream to bison, 
                but I don't know if it actually does shit now
*/
%option 8bit reentrant bison-bridge bison-locations
%option warn noyywrap nodefault stack yylineno

/* Generate a header for use with libs and our Makefile */
%option header-file="include/lex.yy.h"

%{
  #include <string.h>
  #include <parser.tab.h>  

  extern void yyerror(YYLTYPE* loc, struct ast_node *document, const char *s);
%}

/* General rules for our tokenizer */
blank [ \t]
identifier [a-zA-Z][a-zA-Z_0-9]*
null "null"

/* Custom tokenizer states */
%x SINGLE_STRING
%x SINGLE_LINE_COMMENT

%%

{blank}            ;
\n                 { return T_EOL; }
{null}             { return T_NULL; }
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