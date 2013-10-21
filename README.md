coverage
========

Utilities to provide code execution coverage metrics for "C"-coded files.
Currently formed of 2 parts:

  * annotator : Annotates "C" code such that coverage of the execution can
                be tracked
  * analyser : Analyses the result of annotated code which has been executed

NOTE: This is a very new project and is not yet in a usable state, as such
there are no pre-built binaries, etc.

annotator
---------

Annotator is build as a clang[1] tool.  clang provides a pre-build parser
for C, Objective C, C++ and Objective C++.  annotator uses the LibTooling
library in order to analyse and "re-write" the source code to include
annotations necessary to track the flow of execution

Getting Going
=============

I'm currently using Visual Studio 10 for development.  I spent a not
insignificant amount of time trying to build LLVM on Windows under MinGW
and despite trying 3 or 4 different versions of GCC, ran into problems at
various stages (including GCC crashing during link).  Anyway, in order to build
the annotator you'll need to build clang - see the instructions at [3].  
Copy the "annotator" subdirectory to "llvm/tools/clang/tools/extra" and then add

    add_subdirectory(coverage)

To the end of llvm/tools/clang/tools/extra/CMakeLists.txt.  Re-run cmake.

References
==========

  * [1] http://clang.llvm.org/
  * [2] http://clang.llvm.org/docs/LibTooling.html
  * [3] http://clang.llvm.org/get_started.html
