	int num_lines = 0, num_chars = 0;

	int yywrap() { return 1; }

%%
username		printf( "%s", getlogin() );
introduction	printf( "Copyright (c) 2013 Intel Corporation\n" );
\n				++num_lines; ++num_chars;
.				++num_chars;
%%

int main() {

	yylex();
	printf( "# of lines = %d, # of chars = %d\n", num_lines, num_chars );
	return 0;
}


