#include "clang/Frontend/CompilerInstance.h"

using namespace clang;

int main( int argc, char **argv ) {

	CompilerInstance ci;

	ci.createDiagnostics( 0, NULL );
	ci.createFileManager();
	ci.createSourceManager( ci.getFileManager() );
	ci.createPreprocessor();

	const FileEntry *pFile = ci.getFileManager().getFile( "hello.cpp" );

	ci.getSourceManager().createMainFileID( pFile );

#if 0
	ci.getPreprocessor().EnterMainSourceFile();
	ci.getDiagnosticClient().BeginSourceFile( co.getLangOpts(), &ci.getProcessor() );

	Token tok;

	do {

		ci.getProcessor().Lex( tok );

		if( ci.getDiagnostics().hasErrorOccured() )
			break;

		ci.getProcessor().dumpToken( tok );
		std::cerr << std::endl;

	} while ( tok.isNot( clang::tok::eof ) );

	ci.getDiahnosticClient().EndSourceFile();
#endif

	return 0;
}


