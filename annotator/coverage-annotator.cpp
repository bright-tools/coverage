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

 This project was kick-started by the public domain example provided by
 Eli Bendersky (eliben@gmail.com)
 at http://eli.thegreenplace.net/2012/06/08/basic-source-to-source-transformation-with-clang/
 Grateful thanks are due to Eli for sharing his knowledge
*/
#include <cstdio>
#include <string>

#include "clang/AST/ASTConsumer.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/FileManager.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Basic/TargetOptions.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Parse/ParseAST.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/raw_ostream.h"

using namespace clang;
using namespace std;

#include "Annotator.hpp"

// Implementation of the ASTConsumer interface for reading an AST produced
// by the Clang parser.
class MyASTConsumer : public ASTConsumer
{
public:
    MyASTConsumer(Rewriter &R)
        : Visitor(R)
    {}

    // Override the method that gets called for each parsed top-level
    // declaration.
    virtual bool HandleTopLevelDecl(DeclGroupRef DR) {
        for (DeclGroupRef::iterator b = DR.begin(), e = DR.end();
             b != e; ++b)
            // Traverse the declaration using our AST visitor.
            Visitor.TraverseDecl(*b);
        return true;
    }

private:
    Annotator Visitor;
};


int main(int argc, char *argv[])
{
    if (argc != 2) {
        llvm::errs() << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    // CompilerInstance will hold the instance of the Clang compiler for us,
    // managing the various objects needed to run the compiler.
    CompilerInstance TheCompInst;
    TheCompInst.createDiagnostics( (DiagnosticConsumer *)NULL, false);

    // Initialize target info with the default triple for our platform.
    TargetOptions TO;
    TO.Triple = llvm::sys::getDefaultTargetTriple();
    TargetInfo *TI = TargetInfo::CreateTargetInfo(
        TheCompInst.getDiagnostics(), &TO);
    TheCompInst.setTarget(TI);

    TheCompInst.createFileManager();
    FileManager &FileMgr = TheCompInst.getFileManager();
    TheCompInst.createSourceManager(FileMgr);
    SourceManager &SourceMgr = TheCompInst.getSourceManager();
    TheCompInst.createPreprocessor();
    TheCompInst.createASTContext();

    // A Rewriter helps us manage the code rewriting task.
    Rewriter TheRewriter;
    TheRewriter.setSourceMgr(SourceMgr, TheCompInst.getLangOpts());

    // Set the main file handled by the source manager to the input file.
    const FileEntry *FileIn = FileMgr.getFile(argv[1]);
    SourceMgr.createMainFileID(FileIn);
    TheCompInst.getDiagnosticClient().BeginSourceFile(
        TheCompInst.getLangOpts(),
        &TheCompInst.getPreprocessor());

    // Create an AST consumer instance which is going to get called by
    // ParseAST.
    MyASTConsumer TheConsumer(TheRewriter);

    // Parse the file to AST, registering our consumer as the AST consumer.
    ParseAST(TheCompInst.getPreprocessor(), &TheConsumer,
             TheCompInst.getASTContext());

    // At this point the rewriter's buffer should be full with the rewritten
    // file contents.
    const RewriteBuffer *RewriteBuf =
        TheRewriter.getRewriteBufferFor(SourceMgr.getMainFileID());
    llvm::outs() << string(RewriteBuf->begin(), RewriteBuf->end());

    return 0;
}

