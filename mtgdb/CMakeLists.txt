# CMakeList.txt : CMake project for mtgdb, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (mtgdb "src/mtgdb.cpp" "include/mtgdb.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET mtgdb PROPERTY CXX_STANDARD 20)
endif()

# Link SQLite3 
add_library(sqlite3 lib/sqlite3.c)
set_target_properties(sqlite3 PROPERTIES LINKER_LANGUAGE C)
target_include_directories(sqlite3 PUBLIC include)
#Link DB interface
add_library(db_interface lib/db_interface.cpp)
target_include_directories(db_interface PUBLIC include)
#Link Scryfall API Wrapper
add_library(scryfall_api lib/scryfall_api.cpp)
target_include_directories(scryfall_api PUBLIC include)
# Link libraries to executable
target_link_libraries(mtgdb PRIVATE db_interface scryfall_api sqlite3)

# Get CURL
set(CURL_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/mtgdb/include/curl")
set(CURL_LIBRARY "${CMAKE_SOURCE_DIR}/mtgdb/lib/libcurl.a")
find_package(CURL REQUIRED)
target_link_libraries(mtgdb PRIVATE CURL::libcurl)

# SQL info
configure_file(resources/schema.sql ${CMAKE_BINARY_DIR}/mtgdb/schema.sql COPYONLY)
