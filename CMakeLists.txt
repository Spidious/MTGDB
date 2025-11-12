# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.14)

set(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("mtgdb")

# Link SQLite3
add_library(sqlite3 lib/sqlite3.c)
set_target_properties(sqlite3 PROPERTIES LINKER_LANGUAGE C)
target_include_directories(sqlite3 PUBLIC include)

#Link DB interface
add_library(db_interface lib/db_interface.cpp)
target_include_directories(db_interface PUBLIC include)

#Link Scryfall API Wrapper
add_library(scryfall_api lib/scryfall_api.cpp lib/api_result.cpp)
target_include_directories(scryfall_api PUBLIC include)

#Find vcpkg packages
find_package(CURL REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
#Include vcpkg installed headers (This shouldn't be necessary but for some reason it is the only way to get it to work)
target_include_directories(scryfall_api PUBLIC vcpkg_installed/${VCPKG_TARGET_TRIPLET}/include)
#Link vcpkg libraries
target_link_libraries(scryfall_api PRIVATE CURL::libcurl nlohmann_json::nlohmann_json)

# Include sub-projects.
add_subdirectory("lib")
add_subdirectory ("mtgdb")

option(ENABLE_TESTS "Build tests" OFF)
if(ENABLE_TESTS)
  enable_testing()
  add_subdirectory(test)
endif()