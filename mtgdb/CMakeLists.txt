# CMakeList.txt : CMake project for mtgdb, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (mtgdb "src/mtgdb.cpp" "include/mtgdb.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET mtgdb PROPERTY CXX_STANDARD 20)
endif()

#Find vcpkg packages
find_package(CURL REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

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
target_link_libraries(scryfall_api PRIVATE nlohmann_json::nlohmann_json CURL::libcurl)
# Link libraries to executable
target_link_libraries(mtgdb PRIVATE db_interface scryfall_api sqlite3)


# SQL info
configure_file(resources/schema.sql ${CMAKE_BINARY_DIR}/mtgdb/schema.sql COPYONLY)
