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

#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Lex/Lexer.h"
#include "clang/Tooling/CompilationDatabase.h"
#include "clang/Tooling/Refactoring.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/ADT/OwningPtr.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/Signals.h"
#include "llvm/Support/raw_ostream.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Rewrite/Frontend/Rewriters.h"


#include "clang/Frontend/TextDiagnosticPrinter.h"

#include "ForAnnotator.hpp"
#include "WhileAnnotator.hpp"
#include "IfAnnotator.hpp"
#include "CaseAnnotator.hpp"
#include "coverage-annotator-cl.hpp"

#include <string>


using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::tooling;
using namespace llvm;

namespace {
class ToolTemplateCallback : public MatchFinder::MatchCallback {
 public:
  ToolTemplateCallback(Replacements *Replace) : Replace(Replace) {}

  virtual void run(const MatchFinder::MatchResult &Result) {
//  TODO: This routine will get called for each thing that the matchers find.
//  At this point, you can examine the match, and do whatever you want,
//  including replacing the matched text with other text
  (void) Replace; // This to prevent an "unused member variable" warning;
  }

 private:
  Replacements *Replace;
};
} // end anonymous namespace

// Set up the command line options
cl::opt<std::string> BuildPath(
  cl::Positional,
  cl::desc("<build-path>"));

cl::opt<std::string> ProjectRoot(
  "r",
  cl::desc("Set project root directory"),
  cl::value_desc("filename"));

cl::opt<std::string> SourcePaths(
  cl::Positional,
  cl::desc("<source>"),
  cl::Required);


StatementMatcher LoopMatcher = forStmt().bind("forLoop");
StatementMatcher IfMatcher = ifStmt().bind("ifStmt");
StatementMatcher WhileMatcher = whileStmt().bind("whileLoop");
StatementMatcher CaseMatcher = caseStmt().bind("caseStmt");
// TODO: do .. while
// TODO: goto
// TODO: while ...
// TODO: function decl

int main(int argc, const char **argv) {
  llvm::sys::PrintStackTraceOnErrorSignal();
  llvm::OwningPtr<CompilationDatabase> Compilations(
        FixedCompilationDatabase::loadFromCommandLine(argc, argv));
  cl::ParseCommandLineOptions(argc, argv);
  if (!Compilations) {  // Couldn't find a compilation DB from the command line
    std::string ErrorMessage;
    Compilations.reset(
      !BuildPath.empty() ?
        CompilationDatabase::autoDetectFromDirectory(BuildPath, ErrorMessage) :
        CompilationDatabase::autoDetectFromSource(SourcePaths, ErrorMessage)
      );

//  Still no compilation DB? - bail.
    if (!Compilations)
    {
      llvm::report_fatal_error(ErrorMessage);
    }
  }
  RefactoringTool Tool(*Compilations, SourcePaths);
  ast_matchers::MatchFinder Finder;
  //ToolTemplateCallback Callback(&Tool.getReplacements());

  ForAnnotator forAnnotator(&Tool.getReplacements());
  Finder.addMatcher(LoopMatcher, &forAnnotator);
  IfAnnotator ifAnnotator(&Tool.getReplacements());
  Finder.addMatcher(IfMatcher, &ifAnnotator);
  WhileAnnotator whileAnnotator(&Tool.getReplacements());
  Finder.addMatcher(WhileMatcher, &whileAnnotator);
  CaseAnnotator caseAnnotator(&Tool.getReplacements());
  Finder.addMatcher(CaseMatcher, &caseAnnotator);

  int Result = Tool.run(newFrontendActionFactory(&Finder));
  if( !Result ) {
    LangOptions DefaultLangOptions;
    IntrusiveRefCntPtr<DiagnosticOptions> DiagOpts = new DiagnosticOptions();
    TextDiagnosticPrinter DiagnosticPrinter(llvm::errs(), &*DiagOpts);
    DiagnosticsEngine Diagnostics(
        IntrusiveRefCntPtr<DiagnosticIDs>(new DiagnosticIDs()),
        &*DiagOpts, &DiagnosticPrinter, false);
    SourceManager Sources(Diagnostics, Tool.getFiles());
    Rewriter Rewrite(Sources, DefaultLangOptions);

    const FileEntry *FileIn = Sources.getFileManager().getFile(SourcePaths);
    FileID f = Sources.createMainFileID(FileIn);

    if (!tooling::applyAllReplacements(Tool.getReplacements(),Rewrite)) {
      llvm::errs() << "Skipped some replacements.\n";
    }

    const RewriteBuffer *RewriteBuf = Rewrite.getRewriteBufferFor(f);

    if( RewriteBuf == NULL ) 
	{
		/* No re-writes?  Use file verbatim */
		llvm::errs() << "Couldn't get rewrite buffer.\n";
		const MemoryBuffer* verbatimBuffer = Sources.getFileManager().getBufferForFile(FileIn);
		llvm::outs() << std::string(verbatimBuffer->getBufferStart(), verbatimBuffer->getBufferEnd());
    } 
	else 
	{
      llvm::outs() << std::string(RewriteBuf->begin(), RewriteBuf->end());
	}
  }
  return Result;
}
