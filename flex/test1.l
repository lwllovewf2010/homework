	int num_lines = 0, num_chars = 0;

	int yywrap() { return 1; }

%%
username			printf( "%s", getlogin() ); return 1;
introduction	printf( "Copyright (c) 2013 Intel Corporation\n" ); return 2;
mytext				printf( "Hello world\n" ); return 4;
EOF           printf( "EOF\n" ); return 99;

\n						++num_lines; ++num_chars;
.							++num_chars;
%%

int main() {

  int tok;

  while( (tok = yylex()) ) {

    if( tok == 99 )
      break;
  }

	printf( "# of lines = %d, # of chars = %d\n", num_lines, num_chars );
	return 0;
}


