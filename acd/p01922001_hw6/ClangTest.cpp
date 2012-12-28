#include <iostream>

#include "llvm/Support/Host.h"

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/TargetOptions.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Basic/FileManager.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Basic/TargetOptions.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Parse/ParseAST.h"

using namespace clang;

int main( int argc, char **argv ) {

	CompilerInstance ci;

	ci.createDiagnostics( 0, NULL );

	llvm::IntrusiveRefCntPtr<TargetOptions> pto( new TargetOptions() );
	pto->Triple = llvm::sys::getDefaultTargetTriple();
	llvm::IntrusiveRefCntPtr<TargetInfo> pti( 
		TargetInfo::CreateTargetInfo( 
			ci.getDiagnostics(), 
			pto.getPtr()
		)
	);
	ci.setTarget( pti.getPtr() );

	ci.createFileManager();
	ci.createSourceManager( ci.getFileManager() );
	ci.createPreprocessor();

	const FileEntry *pFile = ci.getFileManager().getFile( "Hello.cpp" );
	ci.getSourceManager().createMainFileID( pFile );
	ci.getPreprocessor().EnterMainSourceFile();
	ci.getDiagnosticClient().BeginSourceFile( ci.getLangOpts(), &ci.getPreprocessor() );

	Token tok;

	do {

		ci.getPreprocessor().Lex( tok );

		if( ci.getDiagnostics().hasErrorOccurred() )
			break;

		ci.getPreprocessor().DumpToken( tok );
		std::cout << std::endl;

	} while ( tok.isNot( clang::tok::eof ) );

	ci.getDiagnosticClient().EndSourceFile();

	return 0;
}


