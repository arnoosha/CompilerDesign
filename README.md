# Compiler Design Project

This project is an implementation of a simple expression-oriented compiler built with C++ and LLVM. It demonstrates the main stages of a compiler including lexical analysis, parsing, semantic analysis, and code generation.

## Overview

The compiler takes arithmetic and control-flow expressions as input, parses them into an Abstract Syntax Tree (AST), performs semantic checks, and then generates LLVM Intermediate Representation (IR) code. The project showcases the structure and workflow of a compiler using modern compiler infrastructure.

## Components

- **Lexer**  
  The lexical analyzer (`Lexer.cpp`, `Lexer.h`) tokenizes the input source code into a sequence of tokens such as identifiers, numbers, operators, and keywords:contentReference[oaicite:0]{index=0}:contentReference[oaicite:1]{index=1}.

- **Parser**  
  The parser (`Parser.cpp`, `Parser.h`) constructs an Abstract Syntax Tree (AST) from the token stream. It supports variable declarations, assignments, arithmetic expressions, conditions, loops, and if-elif-else control flow constructs:contentReference[oaicite:2]{index=2}:contentReference[oaicite:3]{index=3}.

- **AST (Abstract Syntax Tree)**  
  The AST is defined in `AST.h` and represents the hierarchical structure of the input program, with node types for expressions, declarations, binary operations, conditions, and control flow:contentReference[oaicite:4]{index=4}.

- **Semantic Analysis**  
  The semantic analyzer (`Sema.cpp`) traverses the AST to detect semantic errors such as undeclared variables, duplicate declarations, invalid assignments, and division by zero:contentReference[oaicite:5]{index=5}.

- **Code Generation**  
  The code generator (`CodeGen.cpp`, `CodeGen.h`) traverses the AST and produces LLVM IR. This IR can be further optimized and executed using LLVM’s toolchain:contentReference[oaicite:6]{index=6}:contentReference[oaicite:7]{index=7}.

- **Driver**  
  The main driver (`GSM.cpp`) integrates all components. It reads input expressions, invokes the lexer and parser, checks for errors, performs semantic analysis, and if successful, generates and outputs LLVM IR:contentReference[oaicite:8]{index=8}.

- **Build Configuration**  
  The project uses CMake (`CMakeLists.txt`) to configure and build the compiler with LLVM libraries:contentReference[oaicite:9]{index=9}.

## Key Features

- Tokenization of input source code with support for operators, keywords, and identifiers.
- Parsing into an AST with support for:
  - Variable declarations and assignments
  - Arithmetic operators (+, -, *, /, %, ^)
  - Compound assignment operators (+=, -=, *=, /=, %=)
  - Conditional operators (==, !=, >=, <=, >, <)
  - Control flow constructs (if, elif, else, loop)
- Semantic checks for scope, redeclaration, and type correctness.
- LLVM IR generation from AST for execution or further compilation.

## Purpose

This project is intended as a practical exploration of compiler design principles. It illustrates the pipeline of a compiler from source code to intermediate representation, and serves as a foundation for extending with more advanced language features or optimization passes.
