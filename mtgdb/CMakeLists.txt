# CMakeList.txt : CMake project for mtgdb, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (mtgdb "src/mtgdb.cpp" "include/mtgdb.h" "lib/db_interface.cpp" "include/db_interface.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET mtgdb PROPERTY CXX_STANDARD 20)
endif()

# Target libraries
add_library(sqlite3 lib/sqlite3.c "lib/db_interface.cpp" "include/db_interface.h")
target_include_directories(sqlite3 PUBLIC include)
target_link_libraries(mtgdb PRIVATE sqlite3)

# SQL info
configure_file(resources/schema.sql ${CMAKE_BINARY_DIR}/mtgdb/schema.sql COPYONLY)
