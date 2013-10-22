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

#include "clang/Basic/SourceManager.h"

#include <sstream>

#include "IfAnnotator.hpp"
#include "AnnotationGenerator.hpp"

using namespace std;
using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::tooling;


IfAnnotator::IfAnnotator(Replacements *Replace) : Annotator(Replace)
{
}


void IfAnnotator::run(const MatchFinder::MatchResult &Result) 
{
    const IfStmt *FS;

	llvm::errs() << "IfAnnotator::run ~ Found 'if' match\n";

    if (FS = Result.Nodes.getNodeAs<clang::IfStmt>("ifStmt"))
    {
		llvm::errs() << "IfAnnotator::run ~ Matched statement\n";

		const Stmt* thenBody = FS->getThen();

		if( thenBody->getStmtClass() != Stmt::CompoundStmtClass )
		{
			llvm::errs() << "IfAnnotator::run ~ Non-compound 'then': " << thenBody->getStmtClassName() << "\n";
			HandleNonCompound( Result, thenBody, FS );
		}

		HandleFlowChange( Result, thenBody );

		const Stmt* elseBody = FS->getElse();

		/* Check for an "else" part */
		if( elseBody != NULL ) {

			/* If the "else" is non-compound and it's not a chained "if" */
			if(( elseBody->getStmtClass() != Stmt::CompoundStmtClass ) &&
			   ( elseBody->getStmtClass() != Stmt::IfStmtClass ))
			{
				llvm::errs() << "IfAnnotator::run ~ Non-compound 'else': " << elseBody->getStmtClassName() << "\n";
				elseBody->dump();
				HandleNonCompound( Result, elseBody, FS );
			}

			/* If the else will be handled by another handler (e.g. it's an "if", which will be handled by another 
			   call to this callback), then we don't want to handle it as a flow change.  If it's not, we do. */
			if( ! HandlerExistsFor( elseBody->getStmtClass() ))
			{
				HandleFlowChange( Result, elseBody );
			}
		}

    }

	llvm::errs() << "IfAnnotator::run ~ Done\n";
}

