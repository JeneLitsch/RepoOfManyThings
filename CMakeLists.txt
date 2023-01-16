cmake_minimum_required (VERSION 3.8)
     
project (repo_of_many_things)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 20)
add_compile_options(
	-O2
	-W 
	-Wall 
	-Wextra 
	-pedantic 
	-Wpedantic 
	-pedantic-errors 
	-Wunused 
	-Wwrite-strings 
	-Wconversion 
	-Wunreachable-code 
	-Wuninitialized 
	-Wold-style-cast 
	-Walloca 
	-Wparentheses 
	-Wno-unknown-pragmas 
)

add_subdirectory(visit_erased)