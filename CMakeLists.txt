add_executable (gsm
  GSM.cpp
  CodeGen.cpp
  CodeGen.h
  Lexer.cpp
  Lexer.h
  Parser.cpp
  Parser.h
  Sema.cpp
  Sema.h
  AST.h
  )
target_link_libraries(gsm PRIVATE ${llvm_libs})
