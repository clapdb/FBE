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

# boost
find_package(Boost REQUIRED)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

# uuid
find_package(LibUUID)

include_directories("modules/memory")


# Modules
add_subdirectory("modules/memory")

# Link libraries
#list(APPEND LINKLIBS cppcommon)

option(FBE_USING_STD_VECTOR "using std vector" OFF)
if (FBE_USING_STD_VECTOR)
  add_definitions(-DUSING_STD_VECTOR)
endif()

option(FBE_USING_SEASTAR_STRING "using seastar string" OFF)
option(FBE_USING_SMALL_STRING "using memory string" OFF)
if (FBE_USING_SEASTAR_STRING)
  add_definitions(-DUSING_SEASTAR_STRING)
elseif(FBE_USING_SMALL_STRING)
  add_definitions(-DUSING_SMALL_STRING)
endif()

option(FBE_USING_SMALL_ARENA_STRING "using memory arena string" OFF)
if (FBE_USING_SMALL_ARENA_STRING)
  add_definitions(-DUSING_SMALL_ARENA_STRING)
endif()

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
target_link_libraries(fbec ${LINKLIBS} optparse)
list(APPEND INSTALL_TARGETS fbec)

# Additional module components: benchmarks, examples, plugins, tests, tools and install
if(NOT FBEC_MODULE)
  include_directories("modules/Catch2/single_include")
  include_directories("modules/rapidjson/include")

  # Modules
  add_subdirectory("modules/Catch2")

  # Proto FBE models
  file(GLOB FBE_FILES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/proto" "proto/*.fbe")
  file(GLOB CPP_ONLY_FBE_FILES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/proto" "proto/cpp_only.fbe")
  file(GLOB FBE_FILE_ARENA RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/proto" "proto/arena_common.fbe")
  file(GLOB FBE_FILE_ARENA_IMPORT RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/proto" "proto/arena.fbe")
  list(REMOVE_ITEM FBE_FILES
    ${FBE_FILE_ARENA}
    ${FBE_FILE_ARENA_IMPORT}
    ${CPP_ONLY_FBE_FILES}
  )
  foreach(FBE_FILE ${FBE_FILES})
    string(REGEX REPLACE "(.*)\\.fbe" "\\1.h" FBE_HEADER ${FBE_FILE})
    set(FBE_INPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/proto")
    set(FBE_INPUT_FILE "${FBE_INPUT_DIR}/${FBE_FILE}")
    set(FBE_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/proto")
    set(FBE_OUTPUT_FILE "${FBE_OUTPUT_DIR}/${FBE_HEADER}")
    set(FBE_TARGET "${FBE_FILE}_TARGET")
    add_custom_command(
      OUTPUT ${FBE_OUTPUT_FILE}
      COMMAND $<TARGET_FILE:fbec> --cpp --final --json --proto --input ${FBE_INPUT_FILE} --output ${FBE_OUTPUT_DIR}
      # COMMAND $<TARGET_FILE:fbec> --csharp --final --json --proto --input ${FBE_INPUT_FILE} --output ${CMAKE_CURRENT_SOURCE_DIR}/projects/CSharp/Proto
      COMMAND $<TARGET_FILE:fbec> --go --final --json --proto --go-module-path fbeproj/proto --input ${FBE_INPUT_FILE} --output ${CMAKE_CURRENT_SOURCE_DIR}/projects/Go/proto
      # COMMAND $<TARGET_FILE:fbec> --java --final --json --proto --input ${FBE_INPUT_FILE} --output ${CMAKE_CURRENT_SOURCE_DIR}/projects/Java/src
      # COMMAND $<TARGET_FILE:fbec> --javascript --final --json --proto --input ${FBE_INPUT_FILE} --output ${CMAKE_CURRENT_SOURCE_DIR}/projects/JavaScript/proto
      # COMMAND $<TARGET_FILE:fbec> --kotlin --final --json --proto --input ${FBE_INPUT_FILE} --output ${CMAKE_CURRENT_SOURCE_DIR}/projects/Kotlin/src
      # COMMAND $<TARGET_FILE:fbec> --python --final --json --proto --input ${FBE_INPUT_FILE} --output ${CMAKE_CURRENT_SOURCE_DIR}/projects/Python/proto
      # COMMAND $<TARGET_FILE:fbec> --ruby --final --json --proto --input ${FBE_INPUT_FILE} --output ${CMAKE_CURRENT_SOURCE_DIR}/projects/Ruby/proto
      # COMMAND $<TARGET_FILE:fbec> --swift --final --json --proto --input ${FBE_INPUT_FILE} --output ${CMAKE_CURRENT_SOURCE_DIR}/projects/Swift/Proto
      DEPENDS ${FBE_INPUT_FILE} COMMENT "Generating FBE proto for ${FBE_INPUT_FILE}..." VERBATIM
    )
    add_custom_target(${FBE_TARGET} DEPENDS ${FBE_OUTPUT_FILE})
    set_target_properties(${FBE_TARGET} PROPERTIES FOLDER "proto")
    list(APPEND PROTO_DEPENDENCIES ${FBE_TARGET})
    list(APPEND PROTO_FILES ${FBE_OUTPUT_FILE})
  endforeach()

  foreach(FBE_FILE ${CPP_ONLY_FBE_FILES})
    string(REGEX REPLACE "(.*)\\.fbe" "\\1.h" FBE_HEADER ${FBE_FILE})
    set(FBE_INPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/proto")
    set(FBE_INPUT_FILE "${FBE_INPUT_DIR}/${FBE_FILE}")
    set(FBE_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/proto")
    set(FBE_OUTPUT_FILE "${FBE_OUTPUT_DIR}/${FBE_HEADER}")
    set(FBE_TARGET "${FBE_FILE}_TARGET")
    add_custom_command(
      OUTPUT ${FBE_OUTPUT_FILE}
      COMMAND $<TARGET_FILE:fbec> --cpp --final --json --proto --input ${FBE_INPUT_FILE} --output ${FBE_OUTPUT_DIR}
      DEPENDS ${FBE_INPUT_FILE} COMMENT "Generating FBE proto for ${FBE_INPUT_FILE}..." VERBATIM
    )
    add_custom_target(${FBE_TARGET} DEPENDS ${FBE_OUTPUT_FILE})
    set_target_properties(${FBE_TARGET} PROPERTIES FOLDER "proto")
    list(APPEND PROTO_DEPENDENCIES ${FBE_TARGET})
    list(APPEND PROTO_FILES ${FBE_OUTPUT_FILE})
  endforeach()

  foreach(FBE_FILE ${FBE_FILE_ARENA} ${FBE_FILE_ARENA_IMPORT})
    string(REGEX REPLACE "(.*)\\.fbe" "\\1.h" FBE_HEADER ${FBE_FILE})
    set(FBE_INPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/proto")
    set(FBE_INPUT_FILE "${FBE_INPUT_DIR}/${FBE_FILE}")
    set(FBE_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/proto")
    set(FBE_OUTPUT_FILE "${FBE_OUTPUT_DIR}/${FBE_HEADER}")
    set(FBE_TARGET "${FBE_FILE}_TARGET")
    add_custom_command(
      OUTPUT ${FBE_OUTPUT_FILE}
      COMMAND $<TARGET_FILE:fbec> --cpp --input ${FBE_INPUT_FILE} --output ${FBE_OUTPUT_DIR}
      DEPENDS ${FBE_INPUT_FILE} COMMENT "Generating FBE proto for ${FBE_INPUT_FILE}..." VERBATIM
    )
    add_custom_target(${FBE_TARGET} DEPENDS ${FBE_OUTPUT_FILE})
    set_target_properties(${FBE_TARGET} PROPERTIES FOLDER "proto")
    list(APPEND PROTO_DEPENDENCIES ${FBE_TARGET})
    list(APPEND PROTO_FILES ${FBE_OUTPUT_FILE})
  endforeach()

  # Proto ptr-based FBE models
  file(GLOB FBE_FILE_PTR_IMPORT RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/ptr_proto" "ptr_proto/ptrpkg.fbe")
  file(GLOB FBE_FILE_PTR_ARENA_IMPORT RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/ptr_proto" "ptr_proto/arena_ptr.fbe")
  file(GLOB FBE_FILE_TEMPLATE_IMPORT RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/ptr_proto" "ptr_proto/template_variant.fbe")
  file(GLOB FBE_FILES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/ptr_proto" "ptr_proto/*.fbe")
  list(REMOVE_ITEM FBE_FILES
    ${FBE_FILE_PTR_IMPORT}
    ${FBE_FILE_PTR_ARENA_IMPORT}
    ${FBE_FILE_TEMPLATE_IMPORT}
  )
  foreach(FBE_FILE ${FBE_FILES})
    string(REGEX REPLACE "(.*)\\.fbe" "\\1.h" FBE_HEADER ${FBE_FILE})
    set(FBE_INPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ptr_proto")
    set(FBE_INPUT_FILE "${FBE_INPUT_DIR}/${FBE_FILE}")
    set(FBE_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/proto")
    set(FBE_OUTPUT_FILE "${FBE_OUTPUT_DIR}/${FBE_HEADER}")
    set(FBE_TARGET "${FBE_FILE}_PTR_TARGET")
    add_custom_command(
      OUTPUT ${FBE_OUTPUT_FILE}
      COMMAND $<TARGET_FILE:fbec> --cpp --ptr --input ${FBE_INPUT_FILE} --output ${FBE_OUTPUT_DIR}
      COMMAND $<TARGET_FILE:fbec> --go --json --proto --go-module-path fbeproj/proto --input ${FBE_INPUT_FILE} --output ${CMAKE_CURRENT_SOURCE_DIR}/projects/Go/proto
      DEPENDS ${FBE_INPUT_FILE} COMMENT "Generating FBE proto for ${FBE_INPUT_FILE}..." VERBATIM
    )
    add_custom_target(${FBE_TARGET} DEPENDS ${FBE_OUTPUT_FILE})
    set_target_properties(${FBE_TARGET} PROPERTIES FOLDER "proto")
    list(APPEND PROTO_DEPENDENCIES ${FBE_TARGET})
    list(APPEND PROTO_FILES ${FBE_OUTPUT_FILE})
  endforeach()

  foreach(FBE_FILE ${FBE_FILE_PTR_IMPORT})
    string(REGEX REPLACE "(.*)\\.fbe" "\\1.h" FBE_HEADER ${FBE_FILE})
    set(FBE_INPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ptr_proto")
    set(FBE_INPUT_FILE "${FBE_INPUT_DIR}/${FBE_FILE}")
    set(FBE_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/proto")
    set(FBE_OUTPUT_FILE "${FBE_OUTPUT_DIR}/${FBE_HEADER}")
    set(FBE_TARGET "${FBE_FILE}_PTR_TARGET")
    add_custom_command(
      OUTPUT ${FBE_OUTPUT_FILE}
      COMMAND $<TARGET_FILE:fbec> --cpp --import-ptr --ptr --input ${FBE_INPUT_FILE} --output ${FBE_OUTPUT_DIR}
      COMMAND $<TARGET_FILE:fbec> --go --json --proto --go-module-path fbeproj/proto --input ${FBE_INPUT_FILE} --output ${CMAKE_CURRENT_SOURCE_DIR}/projects/Go/proto
      DEPENDS ${FBE_INPUT_FILE} COMMENT "Generating FBE proto for ${FBE_INPUT_FILE}..." VERBATIM
    )
    add_custom_target(${FBE_TARGET} DEPENDS ${FBE_OUTPUT_FILE})
    set_target_properties(${FBE_TARGET} PROPERTIES FOLDER "proto")
    list(APPEND PROTO_DEPENDENCIES ${FBE_TARGET})
    list(APPEND PROTO_FILES ${FBE_OUTPUT_FILE})
  endforeach()

  foreach(FBE_FILE ${FBE_FILE_PTR_ARENA} ${FBE_FILE_ARENA_TEMPLATE_IMPORT})
    string(REGEX REPLACE "(.*)\\.fbe" "\\1.h" FBE_HEADER ${FBE_FILE})
    set(FBE_INPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ptr_proto")
    set(FBE_INPUT_FILE "${FBE_INPUT_DIR}/${FBE_FILE}")
    set(FBE_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/proto")
    set(FBE_OUTPUT_FILE "${FBE_OUTPUT_DIR}/${FBE_HEADER}")
    set(FBE_TARGET "${FBE_FILE}_PTR_TARGET")
    add_custom_command(
      OUTPUT ${FBE_OUTPUT_FILE}
      COMMAND $<TARGET_FILE:fbec> --cpp --ptr --input ${FBE_INPUT_FILE} --output ${FBE_OUTPUT_DIR}
      DEPENDS ${FBE_INPUT_FILE} COMMENT "Generating FBE proto for ${FBE_INPUT_FILE}..." VERBATIM
    )
    add_custom_target(${FBE_TARGET} DEPENDS ${FBE_OUTPUT_FILE})
    set_target_properties(${FBE_TARGET} PROPERTIES FOLDER "proto")
    list(APPEND PROTO_DEPENDENCIES ${FBE_TARGET})
    list(APPEND PROTO_FILES ${FBE_OUTPUT_FILE})
  endforeach()

  foreach(FBE_FILE ${FBE_FILE_PTR_ARENA_IMPORT})
    string(REGEX REPLACE "(.*)\\.fbe" "\\1.h" FBE_HEADER ${FBE_FILE})
    set(FBE_INPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ptr_proto")
    set(FBE_INPUT_FILE "${FBE_INPUT_DIR}/${FBE_FILE}")
    set(FBE_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/proto")
    set(FBE_OUTPUT_FILE "${FBE_OUTPUT_DIR}/${FBE_HEADER}")
    set(FBE_TARGET "${FBE_FILE}_PTR_TARGET")
    add_custom_command(
      OUTPUT ${FBE_OUTPUT_FILE}
      COMMAND $<TARGET_FILE:fbec> --cpp --ptr --input ${FBE_INPUT_FILE} --output ${FBE_OUTPUT_DIR}
      DEPENDS ${FBE_INPUT_FILE} COMMENT "Generating FBE proto for ${FBE_INPUT_FILE}..." VERBATIM
    )
    add_custom_target(${FBE_TARGET} DEPENDS ${FBE_OUTPUT_FILE})
    set_target_properties(${FBE_TARGET} PROPERTIES FOLDER "proto")
    list(APPEND PROTO_DEPENDENCIES ${FBE_TARGET})
    list(APPEND PROTO_FILES ${FBE_OUTPUT_FILE})
  endforeach()

  foreach(FBE_FILE ${FBE_FILE_TEMPLATE_IMPORT})
    string(REGEX REPLACE "(.*)\\.fbe" "\\1.h" FBE_HEADER ${FBE_FILE})
    set(FBE_INPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ptr_proto")
    set(FBE_INPUT_FILE "${FBE_INPUT_DIR}/${FBE_FILE}")
    set(FBE_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/proto")
    set(FBE_OUTPUT_FILE "${FBE_OUTPUT_DIR}/${FBE_HEADER}")
    set(FBE_TARGET "${FBE_FILE}_PTR_TARGET")
    add_custom_command(
      OUTPUT ${FBE_OUTPUT_FILE}
      COMMAND $<TARGET_FILE:fbec> --cpp --ptr --input ${FBE_INPUT_FILE} --output ${FBE_OUTPUT_DIR}
      DEPENDS ${FBE_INPUT_FILE} COMMENT "Generating FBE proto for ${FBE_INPUT_FILE}..." VERBATIM
    )
    add_custom_target(${FBE_TARGET} DEPENDS ${FBE_OUTPUT_FILE})
    set_target_properties(${FBE_TARGET} PROPERTIES FOLDER "proto")
    list(APPEND PROTO_DEPENDENCIES ${FBE_TARGET})
    list(APPEND PROTO_FILES ${FBE_OUTPUT_FILE})
  endforeach()

  # Proto library
  file(GLOB_RECURSE PROTO_HEADER_FILES "proto/*.h")
  file(GLOB_RECURSE PROTO_INLINE_FILES "proto/*.inl")
  file(GLOB_RECURSE PROTO_SOURCE_FILES "proto/*.cpp")
  add_library(proto ${PROTO_HEADER_FILES} ${PROTO_INLINE_FILES} ${PROTO_SOURCE_FILES})
  set_target_properties(proto PROPERTIES COMPILE_FLAGS "${PEDANTIC_COMPILE_FLAGS}" FOLDER "libraries")
  target_include_directories(proto PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/proto"  ${memory_path})
  if (APPLE)
      target_link_libraries(proto ${LINKLIBS} arena xxHash::xxHash ${lib_cxxexperimental})
  elseif (UNIX)
      target_link_libraries(proto ${LINKLIBS} arena xxHash::xxHash ${LIBUUID_LIBRARY})
  endif()
  list(APPEND INSTALL_TARGETS proto)
  list(APPEND LINKLIBS proto)

  # Examples
  file(GLOB EXAMPLE_HEADER_FILES "examples/*.h")
  file(GLOB EXAMPLE_INLINE_FILES "examples/*.inl")
  file(GLOB EXAMPLE_SOURCE_FILES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/examples" "examples/*.cpp")
  foreach(EXAMPLE_SOURCE_FILE ${EXAMPLE_SOURCE_FILES})
    string(REGEX REPLACE "(.*)\\.cpp" "\\1" EXAMPLE_NAME ${EXAMPLE_SOURCE_FILE})
    set(EXAMPLE_TARGET "fbe-example-${EXAMPLE_NAME}")
    add_executable(${EXAMPLE_TARGET} ${EXAMPLE_HEADER_FILES} ${EXAMPLE_INLINE_FILES} "examples/${EXAMPLE_SOURCE_FILE}" ${PROTO_FILES})
    add_dependencies(${EXAMPLE_TARGET} ${PROTO_DEPENDENCIES})
    set_target_properties(${EXAMPLE_TARGET} PROPERTIES COMPILE_FLAGS "${PEDANTIC_COMPILE_FLAGS}" FOLDER "examples")
    target_include_directories(${EXAMPLE_TARGET} PUBLIC ${rapidjson})
    if (APPLE)
        target_link_libraries(${EXAMPLE_TARGET} ${LINKLIBS} xxHash::xxHash ${lib_cxxexperimental})
    elseif(UNIX)
        target_link_libraries(${EXAMPLE_TARGET} ${LINKLIBS} xxHash::xxHash)
    endif()
    list(APPEND INSTALL_TARGETS ${EXAMPLE_TARGET})
    list(APPEND INSTALL_TARGETS_PDB ${EXAMPLE_TARGET})
  endforeach()

  add_custom_target(fbec_all DEPENDS fbec ${PROTO_FILES})

  # Benchmarks
  file(GLOB BENCHMARK_HEADER_FILES "performance/*.h")
  file(GLOB BENCHMARK_INLINE_FILES "performance/*.inl")
  file(GLOB BENCHMARK_SOURCE_FILES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/performance" "performance/*.cpp")
  foreach(BENCHMARK_SOURCE_FILE ${BENCHMARK_SOURCE_FILES})
    string(REGEX REPLACE "(.*)\\.cpp" "\\1" BENCHMARK_NAME ${BENCHMARK_SOURCE_FILE})
    set(BENCHMARK_TARGET "fbe-performance-${BENCHMARK_NAME}")
    add_executable(${BENCHMARK_TARGET} ${BENCHMARK_HEADER_FILES} ${BENCHMARK_INLINE_FILES} "performance/${BENCHMARK_SOURCE_FILE}")
    add_dependencies(${BENCHMARK_TARGET} ${PROTO_DEPENDENCIES})
    set_target_properties(${BENCHMARK_TARGET} PROPERTIES COMPILE_FLAGS "${PEDANTIC_COMPILE_FLAGS}" FOLDER "performance")
    target_include_directories(${BENCHMARK_TARGET} PUBLIC ${rapidjson})
    target_link_libraries(${BENCHMARK_TARGET} ${LINKLIBS} cppbenchmark)
    list(APPEND INSTALL_TARGETS ${BENCHMARK_TARGET})
    list(APPEND INSTALL_TARGETS_PDB ${BENCHMARK_TARGET})
  endforeach()

  # Tests
  file(GLOB TESTS_HEADER_FILES "tests/*.h")
  file(GLOB TESTS_INLINE_FILES "tests/*.inl")
  file(GLOB TESTS_SOURCE_FILES "tests/*.cpp")
  add_executable(fbe-tests ${TESTS_HEADER_FILES} ${TESTS_INLINE_FILES} ${TESTS_SOURCE_FILES} ${Catch2} ${PROTO_FILES})
  add_dependencies(fbe-tests ${PROTO_DEPENDENCIES})
  set_target_properties(fbe-tests PROPERTIES COMPILE_FLAGS "${PEDANTIC_COMPILE_FLAGS}" FOLDER "tests")
  target_include_directories(fbe-tests PRIVATE ${Catch2} PUBLIC ${rapidjson} ${memory_path})
  target_link_libraries(fbe-tests ${LINKLIBS} arena xxHash::xxHash)
  list(APPEND INSTALL_TARGETS fbe-tests)
  list(APPEND INSTALL_TARGETS_PDB fbe-tests)

  # CTest
  enable_testing()
  add_test(fbe-tests fbe-tests --durations yes --order lex)

  # Install *.pdb files
  if(MSVC)
    foreach(INSTALL_TARGET_PDB ${INSTALL_TARGETS_PDB})
      install(FILES $<TARGET_PDB_FILE:${INSTALL_TARGET_PDB}> DESTINATION "${PROJECT_SOURCE_DIR}/bin")
    endforeach()
  endif()

  # smoke test
  add_subdirectory(smoke_tests)

endif()

# Install
install(TARGETS ${INSTALL_TARGETS}
  RUNTIME DESTINATION "${PROJECT_SOURCE_DIR}/bin"
  LIBRARY DESTINATION "${PROJECT_SOURCE_DIR}/bin"
  ARCHIVE DESTINATION "${PROJECT_SOURCE_DIR}/bin")
