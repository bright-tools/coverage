set(LLVM_LINK_COMPONENTS support)
set(LLVM_USED_LIBS clangTooling clangBasic clangAST)

add_clang_executable(coverage
  AnnotatorUtils.cpp
  Annotator.cpp
  ForAnnotator.cpp
  IfAnnotator.cpp
  AnnotationGenerator.cpp
  coverage-annotator.cpp
  )
target_link_libraries(coverage
  clangTooling
  clangBasic
  clangASTMatchers
  )

