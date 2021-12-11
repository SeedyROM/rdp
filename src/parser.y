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


%code {
	extern void yyerror(YYLTYPE* loc, const char* s);
	extern void yywarn (YYLTYPE* loc, const char* s);
}

%union {
  char* s_val;
}

%token EOL
%token <s_val> IDENT

%%

values : 
		value
	| values value

value : 
		IDENT 		{ printf("Identifier: %s\n", $1); }
	| IDENT EOL { printf("Identifier: %s\n", $1); }

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