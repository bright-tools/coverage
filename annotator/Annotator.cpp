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

#include <sstream>

#include "Annotator.hpp"
#include "clang/Basic/SourceManager.h"
#include "clang/Lex/Lexer.h"
#include "clang/AST/ASTContext.h"

#include "AnnotatorUtils.hpp"

using namespace clang;
using namespace std;

AnnotationGenerator Annotator::annotationGenerator;

Annotator::Annotator(clang::tooling::Replacements *Replace) : Replace(Replace)
{
}

Annotator::~Annotator()
{
}

void Annotator::HandleFlowChange( const clang::ast_matchers::MatchFinder::MatchResult &Result, const clang::Stmt* pStmt )
{
	if( pStmt->getStmtClass() != Stmt::NullStmtClass )
	{
		string annotationText = annotationGenerator.GetAnnotation( Result, pStmt );

#if 0
		pStmt->dump();
#endif

		SourceLocation loc;

		/* Check to see if the note we're looking at is compound */
		if( pStmt->getStmtClass() == Stmt::CompoundStmtClass )
		{
			/* If it is compound, need to make sure that we insert the annotation after any delcaration */
			for( Stmt::const_child_iterator chld = pStmt->child_begin(); chld!= pStmt->child_end(); chld++ ) {
				if(( *chld != NULL ) &&
					( chld->getStmtClass() != Stmt::NullStmtClass ) &&
					( chld->getStmtClass() != Stmt::DeclStmtClass ))
				{
					loc  = chld->getLocStart();
					break;	
				}
			}
		} else {
			loc = pStmt->getLocStart();
		}

		Replace->insert(clang::tooling::Replacement(*Result.SourceManager, loc, 0, annotationText));
	}
}

void Annotator::HandleNonCompound( const clang::ast_matchers::MatchFinder::MatchResult &Result, const clang::Stmt* pStmt, const clang::Stmt* pParent )
{
	SourceLocation loc,sloc;

    llvm::errs() << "Annotator::HandleNonCompound ~ Handling\n";

	// TODO: Is wrapping it into a compound the right thing to do?  Could use comma operator?

	if( pStmt->getStmtClass() != Stmt::NullStmtClass ) {

		llvm::errs() << "Annotator::HandleNonCompound ~ Wrapping into compound\n";

		string stmt_class = pParent->getStmtClassName();
	    string opening = "{ /* coverage: Made " + stmt_class + " compound */\n";
	    string closing = "} /* coverage: Made " + stmt_class + " compound */\n";

		// Find start location and add an replacement to nopening brace
		loc = pStmt->getLocStart();
		Replace->insert(clang::tooling::Replacement(*Result.SourceManager, loc, 0, opening));

		// Non-compound statement is likely to have a semi-colon separator.  Try and find it.
		sloc = findSemiAfterLocation( pStmt->getLocEnd(), *(Result.Context) );

		// If we didn't find a semi-colon separator, just use the end for the node
		if(! sloc.isValid() ) {
			loc = pStmt->getLocEnd();
		}

		// TODO: Need to use the Lexer here rather than just getLocEnd() - not sure why!
		loc = Lexer::getLocForEndOfToken(sloc, 0, (*Result.SourceManager), LangOptions());

		// Add closing brace
		Replace->insert(clang::tooling::Replacement(*Result.SourceManager, loc, 0, closing));
	}

	llvm::errs() << "Annotator::HandleNonCompound ~ Done\n";
}
