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
#include "CaseAnnotator.hpp"
#include "AnnotationGenerator.hpp"

using namespace std;
using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::tooling;


CaseAnnotator::CaseAnnotator(Replacements *Replace) : Annotator(Replace)
{
}


void CaseAnnotator::run(const MatchFinder::MatchResult &Result) 
{
	// TODO: Should this matcher run on "case" rather than the parent "switch"?
    const CaseStmt *FS;

	llvm::errs() << "CaseAnnotator::run ~ Found 'case' match\n";

    if (FS = Result.Nodes.getNodeAs<clang::CaseStmt>("caseStmt"))
    {
		llvm::errs() << "CaseAnnotator::run ~ Matched statement\n";

		const Stmt* caseBody = FS->getSubStmt();
		caseBody->dump();

		HandleFlowChange( Result, caseBody );
    }

	llvm::errs() << "CaseAnnotator::run ~ Done\n";
}

