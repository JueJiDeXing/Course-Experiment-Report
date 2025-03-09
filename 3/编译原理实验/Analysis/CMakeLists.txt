cmake_minimum_required(VERSION 3.28)
project(Analysis)

set(CMAKE_CXX_STANDARD 17)

add_executable(ex1 Ex1/ex1.cpp
        Ex1/Grammar.h
        Ex1/Grammar.cpp
        CodeHelper/Generator.h
        CodeHelper/Header.h
        CodeHelper/SpecialString.h
        CodeHelper/SpecialString.cpp
        CodeHelper/Printer.h
        CodeHelper/Printer.cpp
        CodeHelper/Utils.h
        CodeHelper/Utils.cpp
)

add_executable(ex2 Ex2/ex2.cpp
        Ex2/WordFactor.h
        Ex2/TokenType.h
        Ex2/Token.h
        Ex2/State.h
        Ex2/WordFactor.cpp
        CodeHelper/Generator.h
        CodeHelper/Header.h
        CodeHelper/SpecialString.h
        CodeHelper/SpecialString.cpp
        CodeHelper/Printer.h
        CodeHelper/Printer.cpp
        CodeHelper/Utils.h
        CodeHelper/Utils.cpp
)
add_executable(ex3 Ex3/ex3.cpp
        Ex3/LLFactor.h
        Ex1/Grammar.h
        Ex1/Grammar.cpp
        CodeHelper/Generator.h
        CodeHelper/Header.h
        CodeHelper/SpecialString.h
        CodeHelper/SpecialString.cpp
        CodeHelper/Printer.h
        CodeHelper/Printer.cpp
        CodeHelper/Utils.h
        CodeHelper/Utils.cpp
)
add_executable(ex4 Ex4/ex4.cpp
        Ex1/Grammar.h
        Ex1/Grammar.cpp
        Ex2/TokenType.h
        Ex2/Token.h
        Ex2/WordFactor.h
        Ex2/WordFactor.cpp
        Ex3/LLFactor.h
        Ex4/Recursive.h
        CodeHelper/Generator.h
        CodeHelper/Header.h
        CodeHelper/SpecialString.h
        CodeHelper/SpecialString.cpp
        CodeHelper/Printer.h
        CodeHelper/Printer.cpp
        CodeHelper/Utils.h
        CodeHelper/Utils.cpp
)


add_executable(ex5
        Ex1/Grammar.h
        Ex1/Grammar.cpp
        Ex2/WordFactor.h
        Ex2/WordFactor.cpp
        Ex5/ex5.cpp
        Ex5/ex5Util.h
        Ex5/I.h
        Ex5/Production.h
        Ex5/DfaFactor.h
        Ex5/ExtendGrammar.h
        Ex5/SheetFactor.h
        Ex5/TreeNode.h
        Ex5/GrammarTreeFactor.h
        CodeHelper/Printer.h
        Ex5/Semantic.h
        Ex5/CalAttribute.h
        CodeHelper/Generator.h
        CodeHelper/Header.h
        CodeHelper/SpecialString.h
        CodeHelper/SpecialString.cpp
        CodeHelper/Printer.h
        CodeHelper/Printer.cpp
        CodeHelper/Utils.h
        CodeHelper/Utils.cpp
)


add_executable(ex6
        Ex6/ex6.cpp
        Ex6/Recursive.h
        Ex6/TreeNode.h
        Ex1/Grammar.h
        Ex1/Grammar.cpp
        Ex2/WordFactor.h
        Ex2/TokenType.h
        Ex2/Token.h
        Ex2/State.h
        Ex2/WordFactor.cpp
        CodeHelper/Generator.h
        CodeHelper/Header.h
        CodeHelper/SpecialString.h
        CodeHelper/SpecialString.cpp
        CodeHelper/Printer.h
        CodeHelper/Printer.cpp
        CodeHelper/Utils.h
        CodeHelper/Utils.cpp
        Ex6/TreeNode.h
        Ex6/Vn_class.h
        Ex6/ex6Util.cpp
        Ex6/ex6Util.h
        Ex6/Recursive_.h
)

add_executable(t t.cpp)
