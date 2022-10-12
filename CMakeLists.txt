cmake_minimum_required(VERSION 3.20)

# Global properties
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Project name
project(FBE)

## set to c++20
set(CMAKE_CXX_STANDARD 20)
set(COMMON_WARNING_FLAGS "${COMMON_WARNING_FLAGS} -Wcomment")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_WARNING_FLAGS} -frtti -fexceptions -fsized-deallocation")
set(CMAKE_CXX_EXTENSIONS ON)

# Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
  set(DOXYGEN "doxygen")
  if(NOT TARGET ${DOXYGEN})
    add_custom_command(OUTPUT "Doxyfile" COMMAND ${DOXYGEN_EXECUTABLE} "Doxyfile" WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/documents")
    add_custom_target(${DOXYGEN} DEPENDS "Doxyfile")
    set_target_properties(${DOXYGEN} PROPERTIES FOLDER "doxygen")
  endif()
endif()

# CMake module path
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Compiler features
include(SetCompilerFeatures)
include(SetCompilerWarnings)
include(SetPlatformFeatures)
include(SystemInformation)

# sanitizer
set(SANITIZE_PARAM "-fsanitize=address -fsanitize=undefined -fno-sanitize=vptr -fno-omit-frame-pointer")  # param from seastar.pc
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${SANITIZE_PARAM}")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${SANITIZE_PARAM}")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} ${SANITIZE_PARAM}")
set(CMAKE_MODULE_LINKER_FLAGS_DEBUG "${CMAKE_MODULE_LINKER_FLAGS_DEBUG} ${SANITIZE_PARAM}")

# External packages
find_package(BISON)
find_package(FLEX)
find_package(xxHash REQUIRED)

# Modules
add_subdirectory("modules/Catch2")
add_subdirectory("modules/memory")
add_subdirectory("modules/cpp-optparse")

# Link libraries
list(APPEND LINKLIBS cppcommon)

# System directories
#include_directories("${CMAKE_CURRENT_SOURCE_DIR}/CppCommon")
include_directories(SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/modules")

# Lexer & parser
if(MSVC)
  flex_target(lexer "${CMAKE_CURRENT_SOURCE_DIR}/source/fbe.l" "${CMAKE_CURRENT_SOURCE_DIR}/source/fbe-lexer.cpp" COMPILE_FLAGS "--verbose --yylineno --wincompat")
else()
  flex_target(lexer "${CMAKE_CURRENT_SOURCE_DIR}/source/fbe.l" "${CMAKE_CURRENT_SOURCE_DIR}/source/fbe-lexer.cpp" COMPILE_FLAGS "--verbose --yylineno")
endif()
bison_target(parser "${CMAKE_CURRENT_SOURCE_DIR}/source/fbe.y" "${CMAKE_CURRENT_SOURCE_DIR}/source/fbe-parser.cpp" COMPILE_FLAGS "-Wno-conflicts-rr -Wno-conflicts-sr")
add_flex_bison_dependency(lexer parser)

# Compiler
file(GLOB_RECURSE EXE_HEADER_FILES "include/*.h" "source/*.h")
file(GLOB_RECURSE EXE_INLINE_FILES "include/*.inl" "source/*.inl")
file(GLOB_RECURSE EXE_SOURCE_FILES "include/*.cpp" "source/*.cpp")
list(APPEND EXE_SOURCE_FILES ${FLEX_lexer_OUTPUTS})
list(APPEND EXE_SOURCE_FILES ${BISON_parser_OUTPUTS})
add_executable(fbec ${EXE_HEADER_FILES} ${EXE_INLINE_FILES} ${EXE_SOURCE_FILES})

if(MSVC)
  # C4005: 'identifier' : macro redefinition
  # C4065: switch statement contains 'default' but no 'case' labels
  # C4127: conditional expression is constant
  # C4244: 'conversion' conversion from 'type1' to 'type2', possible loss of data
  # C4505: 'function' : unreferenced local function has been removed
  # C4702: unreachable code
  set_target_properties(fbec PROPERTIES COMPILE_FLAGS "${PEDANTIC_COMPILE_FLAGS} /wd4005 /wd4065 /wd4127 /wd4244 /wd4505 /wd4702" FOLDER "compiler")
else()
  set_target_properties(fbec PROPERTIES COMPILE_FLAGS "${PEDANTIC_COMPILE_FLAGS} -Wno-register -Wno-sign-compare" FOLDER "compiler")
endif()
target_include_directories(fbec PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(fbec ${LINKLIBS} cpp-optparse)
list(APPEND INSTALL_TARGETS fbec)
