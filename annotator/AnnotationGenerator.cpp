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

#include "AnnotationGenerator.hpp"

#include "clang/AST/DeclarationName.h"
#include "clang/AST/Decl.h"
#include "clang/AST/ASTContext.h"
#include "clang/Basic/SourceManager.h"


using namespace clang;
using namespace std;

#include <iostream>
#include <sstream>

AnnotationGenerator::AnnotationGenerator()
{

}

std::string AnnotationGenerator::GetAnnotationPrefix()
{
    return "BRIGHT_COVERAGE";
}

#if 0
static const DeclContext *getOutermostFuncOrBlockContext(const Decl *D) {
  const DeclContext *Ret = NULL;
  const DeclContext *DC = D->getDeclContext();
  do  {
    if (DC->getDeclKind() == Decl::FunctionDecl)
    {
      Ret = DC;
      std::cout << "FOUND IT\n";
    } else {
        DC = DC->getParent();
    }
  } while((DC != NULL ) && (DC->getDeclKind() != Decl::TranslationUnit));
  return Ret;
}
#endif

std::string getFuncName( ASTContext& pCtx, const clang::Stmt* pStmt )
{
	std::string ret;
	/* Start the search on the node we were passed */
	const clang::Stmt* node = pStmt;
	const clang::NamedDecl* decl;

	clang::ASTContext::ParentVector ancestors;

	do {
		/* Get parents for the current node of interest */
		ancestors = pCtx.getParents( *node );
	
		/* Examine all the parents */
		for (ASTContext::ParentVector::const_iterator I = ancestors.begin(),
                                                      E = ancestors.end();
			I != E; 
			++I) {

			/* TODO: this won't deal with nodes with multiple parents properly */
			
			/* Set the next parent to look at */	
			node = I->get<Stmt>();
#if 0
			if( node != NULL ) {
				llvm::errs() << node->getStmtClassName() << "\n";
			}
#endif
			/* Check to see if we've found a named decl, and if so nab the name */
			decl = I->get<NamedDecl>();
			if( decl != NULL ) {
				ret = decl->getName();
			}
		}
	} while( ancestors.size() && ( node != NULL ) );

	return ret;
}

std::string AnnotationGenerator::GetAnnotation( const clang::ast_matchers::MatchFinder::MatchResult &Result, const clang::Stmt* pStmt )
{
	stringstream ret;

	SourceLocation startLoc = pStmt->getLocStart();
	int loc = startLoc.getRawEncoding();
	/* TODO: this needs to be stripped down so that it's not an absolute path */
	std::string filen = (*Result.SourceManager).getFilename( startLoc );
	std::string funn = getFuncName( *Result.Context, pStmt );

	ret << GetAnnotationPrefix() << "(\"" << filen << "\",\"" << funn << "\"," << loc << ");";

	return ret.str();
}

std::string AnnotationGenerator::GetFunctionStartAnnotation( const clang::FunctionDecl* const p_fn )
{
    Stmt *FuncBody = p_fn->getBody();

    // Function name
    DeclarationName DeclName = p_fn->getNameInfo().getName();
    string FuncName = DeclName.getAsString();

    string TUName;

#if 0
    TranslationUnitDecl *D = p_fn->getDeclContext()->getTranslationUnitDecl();

    const DeclContext* foo = getOutermostFuncOrBlockContext( (const Decl*)p_fn );
    if(( foo != NULL ) && ( foo->getDeclKind() == Decl::TranslationUnit )) {
        const TranslationUnitDecl const * TU = cast<const TranslationUnitDecl* const>(foo);
        TUName = TU->getNameInfo().getName().getAsString();
    }
#endif

    return GetAnnotationPrefix()+"FUNCTION_START(\"" + TUName + "\",\"" + FuncName + "\");\n";
}

std::string AnnotationGenerator::GetFunctionEndAnnotation( clang::FunctionDecl* p_fn )
{
    return "TODO";
}
