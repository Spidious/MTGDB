# CMakeList.txt : CMake project for mtgdb, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (mtgdb "src/mtgdb.cpp" "include/mtgdb.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET mtgdb PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
add_library(sqlite3 lib/sqlite3.c)
target_include_directories(sqlite3 PUBLIC include)
target_link_libraries(mtgdb PRIVATE sqlite3)