cmake_minimum_required(VERSION 3.30)
project(LangulusTesting
   VERSION         1.0.0
   DESCRIPTION     "Langulus testing utility, using the Catch2 library"
   HOMEPAGE_URL    https://langulus.com
)

# Check if this project is built as standalone, or a part of something else 
if (PROJECT_IS_TOP_LEVEL OR NOT LANGULUS)
   set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
   set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
   set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
   include(LangulusUtilities.cmake)
   if (NOT TARGET LangulusLogger)
      fetch_langulus_module(Logger GIT_TAG wip)
   endif()
endif()

# Configure doctest															
fetch_external_module(
    doctest
    GIT_REPOSITORY  https://github.com/doctest/doctest.git
    GIT_TAG         tags/v2.5.3
    GIT_SHALLOW     TRUE
)

# Build and install Tester library											
add_library(LangulusTesting STATIC source/Main.cpp)

target_include_directories(LangulusTesting PUBLIC include)

# Each test inherits the framework build environment, as well as doctest
target_link_libraries(LangulusTesting
   PUBLIC   LangulusCore
            doctest
)

target_compile_definitions(LangulusTesting
   PUBLIC   LANGULUS_OPTION_TESTING
            DOCTEST_CONFIG_SUPER_FAST_ASSERTS
)
