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

using namespace clang;
using namespace std;

#include <iostream>

AnnotationGenerator::AnnotationGenerator()
{

}

std::string AnnotationGenerator::GetAnnotationPrefix()
{
    return "BRIGHT_COVERAGE_";
}

static const DeclContext *getOutermostFuncOrBlockContext(const Decl *D) {
  const DeclContext *Ret = NULL;
  const DeclContext *DC = D->getDeclContext();
  do  {
    if (DC->getDeclKind() == Decl::TranslationUnit)
    {
      Ret = DC;
      std::cout << "FOUND IT\n";
    } else {
        DC = DC->getParent();
    }
  } while((DC != NULL ) && (DC->getDeclKind() != Decl::TranslationUnit));
  return Ret;
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
