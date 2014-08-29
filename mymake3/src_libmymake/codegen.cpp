#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/PassManager.h"

#include <mymake.h>
#include <ast.h>

using namespace mymake;
using namespace llvm;


IRBuilder<> Builder( getGlobalContext() );


Value *TargetAst::codegen() {

#if 0
	Value *L = new Value( ConstantFP::get( getGlobalContext(), APFloat( 98.00 ) );
	Value *R = ConstantFP::get( getGlobalContext(), APFloat( 23.00 ) );

  errs() << "CODE GEN TEST\n";
	return Builder.CreateFAdd( L, R, "addtmp" );
#endif
  return NULL;
}


