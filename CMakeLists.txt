cmake_minimum_required(VERSION 3.16.3)
project(project_0)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(project-2
       Automaton.h
	BlockCommentAutomaton.h
	IDAutomaton.h
	LineCommentAutomaton.h
	MatcherAutomaton.h
	StringAutomaton.h
       Lexer.h
       Token.h
       main.cpp)


