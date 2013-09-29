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

using namespace clang;
using namespace std;

Annotator::Annotator(Rewriter &R) : TheRewriter(R)
{
}


bool Annotator::VisitFunctionDecl(FunctionDecl *f) {
    // Only function definitions (with bodies), not declarations.
    if (f->hasBody()) {
        Stmt *FuncBody = f->getBody();

        // Type name as string
        QualType QT = f->getResultType();
        string TypeStr = QT.getAsString();

        // Function name
        DeclarationName DeclName = f->getNameInfo().getName();
        string FuncName = DeclName.getAsString();

        // Add comment before
        stringstream SSBefore;
        SSBefore << "\nBRIGHT_COVERAGE_START(\"" << FuncName << "\");\n";
        SourceLocation ST = FuncBody->getLocStart().getLocWithOffset(1);
        TheRewriter.InsertText(ST, SSBefore.str(), true, true);

        // And after
        stringstream SSAfter;
        SSAfter << "\nBRIGHT_COVERAGE_END(\"" << FuncName << "\");\n";
        ST = FuncBody->getLocEnd();
        TheRewriter.InsertText(ST, SSAfter.str(), true, true);
    }

    return true;
}


bool Annotator::VisitStmt(Stmt *s) {
    // Only care about If statements.
    if (isa<IfStmt>(s)) {
        IfStmt *IfStatement = cast<IfStmt>(s);
        Stmt *Then = IfStatement->getThen();

        TheRewriter.InsertText(Then->getLocStart(),
                               "// the 'if' part\n",
                               true, true);

        Stmt *Else = IfStatement->getElse();
        if (Else)
        {
            TheRewriter.InsertText(Else->getLocStart(),
                                   "// the 'else' part\n",
                                   true, true);
        }
    }

    return true;
}

