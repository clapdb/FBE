# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

FBE is a Fast Binary Encoding compiler and serialization library, originally forked from FastBinaryEncoding but significantly evolved with new features. It's primarily a C++ project that generates serialization code for multiple target languages.

### Key Features
- **Circular dependency support** via `--ptr` flag generating pointer-based FBE
- **Variant support** for flexible data types
- **Arena allocator support** generating both regular and PMR-based code
- **Multi-language code generation**: C++, C#, Go, Java, JavaScript, Kotlin, Python, Ruby, Swift
- **Template-based and pointer-based serialization modes**

## Architecture

### Core Components
- **`fbec` compiler**: Main executable that processes `.fbe` schema files and generates serialization code
- **Schema Language**: FBE schema files (`.fbe`) define data structures using enums, flags, structs with various field types
- **Generated Code**: Serialization/deserialization classes for target languages
- **Support Modules**:
  - `arena`: Memory arena allocator (modules/arena/)
  - `containa`: High-performance containers (modules/containa/)
  - `smallstring`: Optimized small string implementation (modules/smallstring/)

### Directory Structure
- `source/`: Compiler source code (lexer, parser, generators)
- `include/`: Header files for compiler components
- `proto/`: Template-based FBE schema examples and generated code
- `ptr_proto/`: Pointer-based FBE schema examples
- `tests/`: Test suite using Catch2
- `modules/`: Git submodules for supporting libraries
- `projects/`: Generated code for different target languages

### Build System
The project uses CMake with complex code generation pipelines:
- Generates lexer/parser from flex/bison sources
- Automatically processes `.fbe` files to generate serialization code
- Supports multiple build configurations (Debug with sanitizers)
- Uses Ninja as the preferred generator

## Build Commands

### Initial Setup
```bash
git clone <repo-url>
cd FBE && git submodule update --init --recursive
sudo ./pkg-dependency.sh  # Install system dependencies
```

### Debug Build
```bash
mkdir build.debug && cd build.debug
cmake .. -DCMAKE_BUILD_TYPE=Debug -GNinja -DCMAKE_EXPORT_COMPILE_COMMANDS=ON && mv compile_commands.json ..
ninja
```

### Release Build
```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -GNinja
ninja
```

### Testing
```bash
ninja fbe-tests && ./fbe-tests
```

### Running Single Tests
```bash
./fbe-tests "[test-pattern]"  # Catch2 syntax for filtering tests
```

## FBE Compiler Usage

### Basic Template-based Generation
```bash
fbec --cpp --input=./schema.fbe --output=./generated
```

### Pointer-based Generation (supports circular dependencies)
```bash
fbec --cpp --ptr --input=./schema.fbe --output=./generated
```

### Multi-language Generation
```bash
fbec --cpp --go --final --json --proto --input=./schema.fbe --output=./generated
```

### Common Flags
- `--cpp`, `--csharp`, `--go`, etc.: Target language
- `--ptr`: Generate pointer-based FBE (supports circular dependencies)
- `--import-ptr`: Imported packages are also pointer-based
- `--final`: Generate compact final model (loses versioning capability)
- `--json`: Generate JSON support
- `--proto`: Generate protocol support

## Important Build Notes

- **Sanitizers enabled in Debug**: AddressSanitizer and UndefinedBehaviorSanitizer are enabled for Debug builds
- **Generated code dependency**: Many targets depend on proto code generation from `.fbe` files
- **Compiler toolchain**: Requires Clang (preferred) or GCC with C++20 support
- **System dependencies**: Install via `pkg-dependency.sh` which supports Fedora, Ubuntu/Debian, and macOS

## Configuration Options

### CMake Options
- `FBE_USING_SEASTAR_STRING=ON`: Use Seastar string implementation
- `FBE_USING_SMALL_STRING=ON`: Use small string optimization
- `FBE_USING_SMALL_ARENA_STRING=ON`: Use small arena string optimization

### Build Types
- **Debug**: Includes sanitizers, debug symbols, optimizations disabled
- **Release**: Full optimizations, no debug symbols or sanitizers

## Schema Development

When working with FBE schemas:
- Template-based schemas go in `proto/` directory
- Pointer-based schemas go in `ptr_proto/` directory
- Schema changes automatically trigger code regeneration via CMake
- Generated code appears in `proto/` with appropriate file extensions
- Arena support generates additional `_pmr` suffixed files and namespaces

## Testing Strategy

- **Unit tests** in `tests/` directory using Catch2
- **Smoke tests** in `smoke_tests/` directory
- **Arena-specific tests** in `tests/test_arena.cpp`
- **Serialization tests** covering template, final, JSON, and pointer modes
- Run with `ninja fbe-tests && ./fbe-tests --durations yes --order lex`