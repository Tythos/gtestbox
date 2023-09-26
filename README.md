# gtestbox

Basic scaffold / example of streamlined gtest integration via CMake for unit testing of C++ libraries via submodule dependencies.

## setup

After git-cloning, initialize submodule dependencies:

```sh
> git submodule update --init --recursive
```

## building

Straight CMake build should be sufficient:

```sh
> cmake -S . -B build
> cmake --build build
```

## testing

The CMake project builds both a static library and associated unit tests from the "tests/" folder. Once build, you can run these tests (linked against that library) from the "build/" folder, where the executables should have been generated. (In the following example, we assume Windows/MSVC build configuration when resolving the test executable path.)

```sh
> build\\Debug\\test_hello.exe
```
