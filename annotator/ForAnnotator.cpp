/*
   Copyright 2013 John Bailey

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#if 0
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Rewrite/Frontend/Rewriters.h"
#include "clang/Tooling/Refactoring.h"
#include "clang/Tooling/Tooling.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#endif


#include "clang/Basic/SourceManager.h"

#include <sstream>

#include "ForAnnotator.hpp"
#include "AnnotationGenerator.hpp"

using namespace std;
using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::tooling;


ForAnnotator::ForAnnotator(Replacements *Replace) : Annotator(Replace)
{
}


void ForAnnotator::run(const MatchFinder::MatchResult &Result) 
{
    const ForStmt *FS;

	llvm::errs() << "ForAnnotator::run ~ Found 'for' match\n";

    if (FS = Result.Nodes.getNodeAs<clang::ForStmt>("forLoop"))
    {
		llvm::errs() << "ForAnnotator::run ~ Matched statement\n";

		
		const Stmt* forBody = FS->getBody();

		if( forBody->getStmtClass() != Stmt::CompoundStmtClass )
		{
			llvm::errs() << "ForAnnotator::run ~ Non-compound 'then': " << forBody->getStmtClassName() << "\n";
			HandleNonCompound( Result, forBody, FS );
		}

		HandleFlowChange( Result, forBody );

		const Stmt* forInc = FS->getInc();

		if(( forInc != NULL ) &&
		   ( forInc->getStmtClass() != Stmt::NullStmtClass ))
		{
			llvm::errs() << "ForAnnotator::run ~ Found increment/decrement term\n";
			HandleFlowChange( Result, forInc, true );
		}
#if 0
		for( Stmt::const_child_iterator x = FS->child_begin(); x!= FS->child_end(); x++ ) {
			if(( *x != NULL ) &&
				( x->getStmtClass() != Stmt::NullStmtClass )) {
				x->dump();
				SourceLocation loc = x->getLocEnd().getLocWithOffset(1);
				Replace->insert(Replacement(*Result.SourceManager, loc, 0, "formoxy"));
			}
			//break;
		}

#endif
		/* Get the location just after the for loop and instrument that.
		   This is so that we can detect that the for loop exited. */
		//SourceLocation loc = FS->getLocEnd().getLocWithOffset(1);
        //Replace->insert(Replacement(*Result.SourceManager, loc, 0, "moxy"));
    }
}

