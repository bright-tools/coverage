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
#include "coverage-annotator-cl.hpp"

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

#if defined _WIN32
// TODO: This is Windows only code ...
#include "Shlwapi.h"
#include <algorithm>

// "Borrowed" from http://stackoverflow.com/questions/7724011/in-c-whats-the-fastest-way-to-replace-all-occurrences-of-a-substring-within
std::string
replaceAll( std::string const& original,
            std::string const& before,
            std::string const& after )
{
    std::string retval;
    std::string::const_iterator end     = original.end();
    std::string::const_iterator current = original.begin();
    std::string::const_iterator next    =
            std::search( current, end, before.begin(), before.end() );
    while ( next != end ) {
        retval.append( current, next );
        retval.append( after );
        current = next + before.size();
        next = std::search( current, end, before.begin(), before.end() );
    }
    retval.append( current, next );
    return retval;
}

std::string getFileName( const std::string pFn )
{
	if( ProjectRoot.length() ) 
	{
		// TODO: Clean up variable names, etc.
		char szOut[MAX_PATH] = "";
		char can[MAX_PATH] = "";
		TCHAR** lppPart={NULL};
		char buffer_1[MAX_PATH];
		string path;

		GetFullPathName(ProjectRoot.c_str(),
					 MAX_PATH,
					 can,
					 lppPart);

		PathRelativePathTo(szOut,
						   can,
						   FILE_ATTRIBUTE_DIRECTORY,
						   pFn.c_str(),
						   FILE_ATTRIBUTE_NORMAL);

		PathCanonicalize(buffer_1,szOut);

		path = buffer_1;

		// Need to escape out all those pesky directory separators
		path = replaceAll( path, "\\", "\\\\" );

		return path;
	} else {
		return pFn;
	}
}
// !TODO: Windows only code
#else
#error Really sorry - don't have path handling coded for your platform yet :-(
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

std::string AnnotationGenerator::GetAnnotation( const clang::ast_matchers::MatchFinder::MatchResult &Result, const clang::Stmt* pStmt, const bool pUseComma )
{
	stringstream ret;

	SourceLocation startLoc = pStmt->getLocStart();
	int loc = startLoc.getRawEncoding();
	/* TODO: this needs to be stripped down so that it's not an absolute path */
	std::string filen = getFileName( (*Result.SourceManager).getFilename( startLoc ) );
	std::string funn = getFuncName( *Result.Context, pStmt );

	ret << GetAnnotationPrefix() << "(\"" << filen << "\",\"" << funn << "\"," << loc << ")";
	if( pUseComma ) 
	{
		ret << ",";
	} else {
		ret << ";";
	}
	ret << "\n";

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
