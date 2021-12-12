/**
 * @file parser.y
 * @author Zack Kollar (SeedyROM) (me@seedyrom.io)
 * @brief 
 * @version 0.1
 * @date 2021-12-10
 * 
 * @copyright Copyright (c) 2021
 * 
 */

%define api.pure full // Make sure the generated parser is reentrant (aka the state is locally defined), bad naming IMO
%define api.push-pull push // Don't circularly rely on the yylex method, push tokens to our parser when needed
%define parse.trace // Helps debug logs
%define parse.error verbose // Helps errors
%define api.token.prefix {T_} // Preface our tokens with a T to make use later simpler
%locations // Use bison locations, flex has a complimentary option we're using called `%option bison-locations`
%header "include/parser.tab.h" // Generate a header file in the include dir with the bison impl

// Include our AST type at the "top" (aka before the defines) that will allow us to pass it to our gen'd params below
// AKA %*-param 
%code top {
  #include "ast.h"
}

// Forward declare the parser error methods, this is just a fact of bison/yacc.
// I can't find another way.
%code {
  extern void yyerror(YYLTYPE* loc, struct ast_node* document, const char* s);
  extern void yywarn (YYLTYPE* loc, struct ast_node* document, const char* s);
}

// Pass data to the lexer and parser, specifically the document we're building
// This is what takes our example from simple to something actually useful in the real world
%lex-param   { struct ast_node* document }
%parse-param { struct ast_node* document }

// Create a bison union to handle the different types our tokens will emit.
%union {
  char* string;
  struct ast_node* node;
}

// Generic tokens
%token EOL
%token <string> IDENT
%token <string> STRING
%token NULL

// AST grammar types, probably all node or struct ast_node* specifically
%type <node> values
%type <node> item
%type <node> value

%%

// The root document, this is where our tree is finally placed for us to use
document : values { ast_node_object_append(document, $1); }

// Everything after this point is test code... PLEASE INGORE

values : 
    item { 
      $$ = ast_node_array();
      ast_node_array_append($$, $1);
    }
  | values item {
      ast_node_array_append($1, $2);
    }

item : 
    value
  | value EOL

value :
    NULL       { $$ = ast_node_null();     }
  |  IDENT     { $$ = ast_node_ident($1);  }
  | STRING     { $$ = ast_node_string($1); }
  

%%

// Handle errors and warns, ignore warns for now

void yyerror(YYLTYPE* loc, struct ast_node *document, const char *s)
{
  fflush(stderr);
  fprintf(stderr, "Parse error: %s\n", s);
}

/* void yywarn(YYLTYPE* loc, const char *s)
{
  fflush(stdout);
  printf("Warning in %s at line %d: %s\n", loc->filename, loc->first_line, s);
} */