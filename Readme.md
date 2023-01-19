# FBE

[![Linux (clang)](https://github.com/stdbio/FBE/actions/workflows/linux-clang.yml/badge.svg)](https://github.com/stdbio/FBE/actions/workflows/linux-clang.yml) [![Linux (gcc)](https://github.com/stdbio/FBE/actions/workflows/linux-gcc.yml/badge.svg)](https://github.com/stdbio/FBE/actions/workflows/linux-gcc.yml)
## Introduction

FBE was originally forked from [Fast Binary Encoding (FBE)](https://github.com/chronoxor/FastBinaryEncoding). After a lot of new features were added, FBE is totally different from the original one.

New features:
- support circular dependency via `--ptr` which will generate ptr-based FBE.
- support `variant`.
- supoort `arena`.
> fbec will generate both arena and non-arena code.
> The generated arena code will be located in files affixed with a `_pmr` suffix,
> and the generated classes and structs will also be in namespaces with a `_pmr` suffix.
> See /proto/* and `tests/test_arena.cpp` for more information.
- fix alignment issues.

The examples containing the above features can be found in /proto/*.fbe `/proto/*.fbe`

## Getting Started

### Build

``` bash
git clone git@github.com:stdbio/FBE.git
cd FBE && git submodule update --init --recursive
sudo pkg-dependency.sh
mkdir build.debug && cd build.debug
cmake .. -DCMAKE_BUILD_TYPE=Debug -GNinja -DCMAKE_EXPORT_COMPILE_COMMANDS=ON && mv compile_commands.json ..
ninja
```

### Test

``` bash
ninja fbe-tests && ./fbe-tests
```

### fbec

`fbec --help` for more information. 

#### Examples

``` bash
# Generate template-based FBE.
fbec --cpp --input=./test.fbe --output=./generated

# Generate ptr-based FBE, which support circular dependency.
# And the imported package is template-based.
fbec --cpp --input=./test.fbe --output=./generated --ptr

# Generate ptr-based FBE, and the imported package is also ptr-based.
fbec --cpp --input=./test.fbe --output=./generated --ptr --import-ptr
```

Below is a cmake example.
``` cmake
add_custom_target(fbec_test_ptr
  COMMAND fbec --cpp --input=./test_ptr.fbe --output=./generated --ptr --import-ptr
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/metas
  DEPENDS "${PROJECT_SOURCE_DIR}/metas/test_ptr.fbe"
  COMMENT "fbec test_ptr.fbe"
  VERBATIM
)
```
