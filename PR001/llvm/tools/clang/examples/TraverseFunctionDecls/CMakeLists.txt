set(MODULE TRUE)

set( LLVM_LINK_COMPONENTS support mc)

add_clang_library(TraverseFunctionDecls TraverseFunctionDecls.cpp)

add_dependencies(TraverseFunctionDecls
  ClangAttrClasses
  ClangAttrList
  ClangCommentNodes
  ClangDeclNodes
  ClangDiagnosticCommon
  ClangStmtNodes
  )

target_link_libraries(TraverseFunctionDecls
  clangFrontend
  clangAST
  )

set_target_properties(TraverseFunctionDecls
  PROPERTIES
  LINKER_LANGUAGE CXX
  PREFIX "")
