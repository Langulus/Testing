cmake_minimum_required(VERSION 3.28)
project(LangulusTesting
    VERSION         1.0.0
    DESCRIPTION     "Langulus testing utility, using the Catch2 library"
    HOMEPAGE_URL    https://langulus.com
)

# Check if this project is built as standalone, or a part of something else 
if (PROJECT_IS_TOP_LEVEL OR NOT LANGULUS)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
    include(LangulusUtilities.cmake)
	if (NOT TARGET LangulusLogger)
		fetch_langulus_module(Logger GIT_TAG 0d1d5d22e54f9ef8b371975f43638a35fe23c301)
	endif()
endif()

# Configure Catch2															
fetch_external_module(
    Catch2
    GIT_REPOSITORY  https://github.com/catchorg/Catch2.git
    GIT_TAG         ee1450f268dfd5c13aa8670ba97e93cabaf2e15d #v2.x
)

# Build and install Tester library											
add_library(LangulusTesting
	STATIC		$<TARGET_OBJECTS:LangulusLogger>
				source/Main.cpp
)

target_compile_definitions(LangulusTesting
    PRIVATE     LANGULUS_EXPORT_ALL
)

target_include_directories(LangulusTesting
    PUBLIC      $<TARGET_PROPERTY:LangulusLogger,INTERFACE_INCLUDE_DIRECTORIES>
				include
)

target_link_libraries(LangulusTesting
    PUBLIC      LangulusCore
				Catch2
)