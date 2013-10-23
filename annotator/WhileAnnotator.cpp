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
#include "WhileAnnotator.hpp"
#include "AnnotationGenerator.hpp"

using namespace std;
using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::tooling;


WhileAnnotator::WhileAnnotator(Replacements *Replace) : Annotator(Replace)
{
}


void WhileAnnotator::run(const MatchFinder::MatchResult &Result) 
{
    const WhileStmt *FS;

	llvm::errs() << "WhileAnnotator::run ~ Found 'while' match\n";

    if (FS = Result.Nodes.getNodeAs<clang::WhileStmt>("whileLoop"))
    {
		llvm::errs() << "WhileAnnotator::run ~ Matched statement\n";

		const Stmt* whileBody = FS->getBody();

		if( whileBody->getStmtClass() != Stmt::CompoundStmtClass )
		{
			llvm::errs() << "WhileAnnotator::run ~ Non-compound loop content: " << whileBody->getStmtClassName() << "\n";
			HandleNonCompound( Result, whileBody, FS );
		}

		HandleFlowChange( Result, whileBody );
    }
}

