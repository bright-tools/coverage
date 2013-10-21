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
#if !defined( ANNOTATOR_ANNOTATOR_HPP )
#define       ANNOTATOR_ANNOTATOR_HPP

#include "clang/Tooling/Refactoring.h"
#include "clang/Tooling/Tooling.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

#include "AnnotationGenerator.hpp"

class Annotator : public clang::ast_matchers::MatchFinder::MatchCallback
{
public:
    Annotator(clang::tooling::Replacements *Replace);
	~Annotator();

    virtual void run(const clang::ast_matchers::MatchFinder::MatchResult &Result) = 0;

	void HandleNonCompound( const clang::ast_matchers::MatchFinder::MatchResult &Result, const clang::Stmt* pStmt );

	void HandleFlowChange( const clang::ast_matchers::MatchFinder::MatchResult &Result, const clang::Stmt* pStmt );

protected:
	const clang::Stmt* FindLast( const clang::Stmt* pStmt );

    clang::tooling::Replacements *Replace;

	static AnnotationGenerator annotationGenerator;
};

#endif
