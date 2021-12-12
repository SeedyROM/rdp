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

%define api.pure full
%define api.push-pull push
%define parse.trace
%define parse.error verbose
%define api.token.prefix {T_}
%locations
%header "include/parser.tab.h"

%code top {
	#include "ast.h"
}

%code {
	extern void yyerror(YYLTYPE* loc, struct ast_node* document, const char* s);
	extern void yywarn (YYLTYPE* loc, struct ast_node* document, const char* s);
}

%parse-param { struct ast_node* document }

%union {
  char* string;
	struct ast_node* node;
}

%token EOL
%token <string> IDENT
%token <string> STRING
%token NULL

%type <node> values
%type <node> item
%type <node> value

%%

document : values { ast_node_object_append(document, $1); }

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
		NULL 			{ $$ = ast_node_null();     }
	|	IDENT 		{ $$ = ast_node_ident($1);  }
	| STRING 		{ $$ = ast_node_string($1); }
	

%%

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