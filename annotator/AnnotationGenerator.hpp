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
#if !defined( ANNOTATOR_ANNOTATOR_GENERATOR_HPP )
#define       ANNOTATOR_ANNOTATOR_GENERATOR_HPP

#include "clang/AST/ASTConsumer.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

#include <string>

class AnnotationGenerator
{
    public:
        AnnotationGenerator();

		std::string GetAnnotation( const clang::ast_matchers::MatchFinder::MatchResult &Result, const clang::Stmt* pStmt );

	protected:
        static std::string GetFunctionStartAnnotation( const clang::FunctionDecl* const p_fn );
        static std::string GetFunctionEndAnnotation( clang::FunctionDecl* p_fn );
        static std::string GetAnnotationPrefix();

};

#endif
