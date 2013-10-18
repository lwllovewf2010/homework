%{
#include <iostream>
#include <fstream>

using namespace std;
%}

DIGIT		[0-9]
ID			[a-zA-Z_][a-zA-Z_0-9]*
GAP			[ \t]*
GAP1		[ \t]+
TGTSEP		:[^=]
TAB			[\t]+
ANYSTR		.*
NEWLINE		[\r\n]
EQUAL		[:?+]*[=]+

%option noyywrap

%%

".DEFAULT:".*              printf( "TK_DOT_DEFAULT> %s\n", yytext );
".DEFAULT_GOAL:".*         printf( "TK_DOT_DEFAULT_GOAL> %s\n", yytext );
".DELETE_ON_ERROR:".*      printf( "TK_DOT_DELETE_ON_ERROR> %s\n", yytext );
".EXPORT_ALL_VARIABLE:".*  printf( "TK_DOT_EXPORT_ALL_VARIABLE> %s\n", yytext );
".FEATURES:".*             printf( "TK_DOT_FEATURES> %s\n", yytext );
".IGNORE:".*               printf( "TK_DOT_IGNORE> %s\n", yytext );
".INCLUDE_DIRS:".*         printf( "TK_DOT_INCLUDE_DIRS> %s\n", yytext );
".INTERMEDIATE:".*         printf( "TK_DOT_INTERMEDIATE> %s\n", yytext );
".LIBPATTERNS:".*          printf( "TK_DOT_LIBPATTERNS> %s\n", yytext );
".LOW_RESOLUTION_TIME:".*  printf( "TK_DOT_LOW_RESOLUTION_TIME> %s\n", yytext );
".NOTPARALLEL:".*          printf( "TK_DOT_NOTPARALLEL> %s\n", yytext );
".ONESHELL:".*             printf( "TK_DOT_ONESHELL> %s\n", yytext );
".PHONY:".*                printf( "TK_DOT_PHONY> %s\n", yytext );
".POSIX:".*                printf( "TK_DOT_POSIX> %s\n", yytext );
".PRECIOUS:".*             printf( "TK_DOT_PRECIOUS> %s\n", yytext );
".RECIPEPREFIX:".*         printf( "TK_DOT_RECIPEPREFIX> %s\n", yytext );
".SECONDARY:".*            printf( "TK_DOT_SECONDARY> %s\n", yytext );
".SECONDEXPANSION:".*      printf( "TK_DOT_SECONDEXPANSION> %s\n", yytext );
".SHELLFLAGS:".*           printf( "TK_DOT_SHELLFLAGS> %s\n", yytext );
".SILENT:".*               printf( "TK_DOT_SILENT> %s\n", yytext );
".SUFFIXES:".*             printf( "TK_DOT_SUFFIXES> %s\n", yytext );
".VARIABLES:".*            printf( "TK_DOT_VARIABLES> %s\n", yytext );

"vpath:".*                 printf( "TK_VPATH_NOCAP> %s\n", yytext );
"VPATH:".*                 printf( "TK_VPATH_CAP> %s\n", yytext );

"include".*                printf( "TK_INCLUDE> %s\n", yytext );
"ifdef".*                  printf( "TK_IFDEF> %s\n", yytext );
"ifeq".*                   printf( "TK_IFEQUA> %s\n", yytext );
"ifndef".*                 printf( "TK_IFNDEF> %s\n", yytext );
"ifneq".*                  printf( "TK_IFNEQUA> %s\n", yytext );
"if".*                     printf( "TK_IF> %s\n", yytext );
"else".*                   printf( "TK_ELSE> %s\n", yytext );
"endef".*                  printf( "TK_ENDEF> %s\n", yytext );
"endif".*                  printf( "TK_ENDIF> %s\n", yytext );

#.*										printf( "TK1_COMMENT> %s\n", yytext );
^{GAP}{ID}{GAP}{EQUAL}{GAP}{ANYSTR}		printf( "TK1_VARIABLE> %s\n", yytext );
^{ID}{GAP}{TGTSEP}{GAP}{ANYSTR}			printf( "TK1_TARGET> %s\n", yytext );
^{TAB}{ANYSTR}							printf( "TK1_RECIPE> %s\n", yytext );
\n
.										printf( "\tChar> %s\n", yytext );

%%


int main( int argc, char **argv ) {

    FlexLexer *lexer;
	filebuf rfile;

	if( argc != 2 ) {

		cerr << "At least 2 input arguments\n";
		return -1;
	}

	// Open file
	rfile.open( argv[ 1 ], std::ios::in );
	if( rfile.is_open() == false ) {

		cerr << "Cannot open file\n";
		return -1;
	}

	istream is( &rfile );

	// Create new object
	lexer = new yyFlexLexer( &is );

    // Execute
    while( lexer->yylex() != 0 );

    // Return
    return 0;
}


