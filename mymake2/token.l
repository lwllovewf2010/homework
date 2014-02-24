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

#.*																		printf( "TK1_COMMENT> %s\n", yytext );
^{GAP}{ID}{GAP}{EQUAL}{GAP}{ANYSTR}		printf( "TK1_VARIABLE> %s\n", yytext );
^{ID}{GAP}{TGTSEP}{GAP}{ANYSTR}				printf( "TK1_TARGET> %s\n", yytext );
^{TAB}{ANYSTR}												printf( "TK1_RECIPE> %s\n", yytext );
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


