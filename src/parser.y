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

%code {
	#include "ast.h"

	extern void yyerror(YYLTYPE* loc, const char* s);
	extern void yywarn (YYLTYPE* loc, const char* s);
}

%union {
  char* string;
	struct ast_node* node;
}

%token EOL
%token <string> IDENT

%type <node> values
%type <node> value

%%

values : 
		value { 
			$$ = ast_node_array();
			ast_node_array_append($$, $1);
		}
	| values value {
			ast_node_array_append($$, $1);
		}

value : 
		IDENT 		{ $$ = ast_node_ident($1); }
	| IDENT EOL { $$ = ast_node_ident($1); }

%%

void yyerror(YYLTYPE* loc, const char *s)
{
	fflush(stderr);
	fprintf(stderr, "Parse error: %s\n", s);
}

/* void yywarn(YYLTYPE* loc, const char *s)
{
	fflush(stdout);
	printf("Warning in %s at line %d: %s\n", loc->filename, loc->first_line, s);
} */