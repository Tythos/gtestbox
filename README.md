Reusing C++ projects is a bad enough experience. When it comes to the complexity overhead introduced by integrations with things like unit testing frameworks, the nightmare can get considerably worse. This goes rapidly downhill when you try and consider how to do so in a way that is platform-neutral and doesn't require too many special configurations for package installation, etc.

We consider an approach here that combines a cmake+submodule approach to writing transportable C++ packages with the potent googletest framework. In addition to writing against a static library build, we want to outline a transparent way in which additional tests can be introduced and scaffolded to support CI, easy "spinup" cost for new developers, and standard test reports.

## The Test Target

We'll start by defining a basic C++ project that builds into a static library. At the top level, this will include a namespace-organized class with a basic "Person" model and behavior. We'll also provide an implementation to "hide" the source for our library in the usual ".hpp"-vs-".cpp" dichotemy.

> _I am using a convention here that is not strictly necessary for this testing setup. Specifically, I am mapping namespaces to project, file, and symbol levels to provide a clear python-like mapping between logical and physical (that is, in-code and on-filesystem) representations. It's convenient but not required._

```cpp
/**
 * person.hpp
 */

#pragma once

#include <string>
#include <iostream>

namespace gtestbox {
    namespace person {
        class Person {
        private:
        protected:
        public:
            std::string name;
            int age;
            Person();
            void sayHello(std::string msg);
        };
    }
}
```

```cpp
/**
 * person.cpp
 */

#include "person.hpp"

gtestbox::person::Person::Person() :
    name("unknown"),
    age(0) {}

void gtestbox::person::Person::sayHello(std::string msg) {
    std::cout << this->name << ", age " << this->age << ", says '" << msg << "'" << std::endl;
}
```

The corresponding CMakeLists.txt file is fairly straightforward; we define a few project settings and the static library we want to build from our source.

```cmake
# define project settings
cmake_minimum_required(VERSION 3.14)
project(gtestbox)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_library(${PROJECT_NAME} STATIC
    person.cpp
)
```

If you build this, you should see a static library (e.g., `.lib` on Windows/MSVC) show up in your build artifacts folder for the default configuration.

```sh
> cmake -S . -B build
> cmake --build build
> ls build/Debug
```

## Hooking and Configuring Dependencies

We will avoid differentiating between development and non-development dependencies for the time being, because it lets us avoid concerns about platform-specific installation and exposure of binaries, headers, and libraries. Submodules are a great way to do this for open source code where you can directly include the tools you need within your project configuration by way of Git.

Let's consume the gtest suite by adding a submodule from the google project:

```sh
> git submodule add https://github.com/google/googletest.git
> git submodule update --init --recursive
```

We now need to hook and configure this dependency within our cmake file.

```cmake
# hook and configure dependencies
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(googletest EXCLUDE_FROM_ALL)
enable_testing()
```

This is a one-time setup within `CMakeLists.txt`, after which we can add in each test module one by one.

## Adding, Linking, and Cataloging Tests

Create a "tests/" folder within your project. I find it is helpful to transparently indicate that test files (or modules, as it were) are not necessarily part of your project source. This also helps make it clear (to both developers and CMake) that you are building/linking these tests against the static library and not the source itself.

We'll include a few basic tests against our default constructor values as a demonstration. From a `tests/test_defaults.cpp` file, we'll include the gtest and library headers, then use the gtest macros to define a method with several assertions against a default Person object. Finally, we'll close out with a `main()` entry point that uses some nice gtest self-discovery logic. If we use our namespace organization scheme, the contents might look something like this:

```cpp
/**
 * tests/test_defaults.cpp
 */

#include "person.hpp"
#include "gtest/gtest.h"

namespace gtestbox {
    namespace tests {
        namespace test_hello {
            TEST(TestDefaults, BasicAssertions) {
                gtestbox::person::Person p;
                EXPECT_STREQ(p.name.c_str(), "unknown");
                EXPECT_EQ(p.age, 0);
            }
        }
    }
}

int main(int nArgs, char** vArgs) {
    ::testing::InitGoogleTest(&nArgs, vArgs);
    return RUN_ALL_TESTS();
}
```

But the real lift comes in our `CMakeTests.txt` file, where we define a new compile target for each test module we've created. We need to add these files as an executable for their own project; include relevant directories so they can "discover" the library headers (since we haven't installed them yet); link against the library and other artifacts from the gtest project; and finally add the test so CMake recognizes its relationship to project commands.

```cmake
# add; link; and catalog test_hello tests
add_executable(test_defaults tests/test_defaults.cpp)
target_include_directories(test_defaults PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(test_defaults gtest gtest_main ${PROJECT_NAME})
add_test(NAME test_defaults COMMAND test_defaults)
```

## Putting It All Together

To summarize: The CMake project builds both a static library and associated unit tests from the "tests/" folder. Once build, you can run these tests (linked against that library) from the "build/" folder, where the executables should have been generated. (In the following example, we assume Windows/MSVC build configuration when resolving the test executable path.) We've now reached a point where we can see the end result with a simple series of standard CMake commands:

```sh
> cmake -S . -B build
> cmake --build build
> build\\Debug\\test_defaults.txt
```

If all goes well, you should see something like the following:

![Successful test results](https://dev-to-uploads.s3.amazonaws.com/uploads/articles/pnew2hqkzf2mhjik72ce.png)

## Next Steps

Adding additional tests as you flesh out your package is now trivial. I recommend a one-to-one corresponding test module for each top-level module; in addition to simplifying naming, this makes it clear what namespace each file is testing. The `CMakeLists.txt` modifications will effectively be a copy-paste of the final "add; link and catalog" lines from our final example.

More sophisticated usage will eventually require you define install procedures (or at least configurations) for binaries, headers, and libraries across platforms, especially if you don't want to share all of your source directly. This can be accomplished through a combination of user-defined `.cmake` toolchain file paths and the corresponding `install()` etc. calls from your `CMakeLists.txt` file. Perhaps we'll cover that in a sequel.

In the meantime, I think you'll find this approach fairly robust across well-organized C++ projects, even when multiple levels of submodule dependencies are introduced.

## Related Resources

* [GitHub Project](https://github.com/Tythos/gtestbox)

* [Dev.to Article](https://dev.to/tythos/basic-c-unit-testing-with-gtest-cmake-and-submodules-4767)

* [GTest CMake Guide](https://google.github.io/googletest/quickstart-cmake.html)

* [Previous CMake+Submodule Article](https://dev.to/tythos/felt-cute-might-git-rm-rf-45i0)
