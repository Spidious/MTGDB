# CMakeList.txt : CMake project for mtgdb, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (mtgdb "mtgdb.cpp" "../include/mtgdb.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET mtgdb PROPERTY CXX_STANDARD 20)
endif()

#target_include_directories(scryfall_api PUBLIC ../vcpkg_installed/x64-linux/include)
# Link libraries to executable

target_link_libraries(mtgdb PRIVATE db_interface scryfall_api sqlite3)

# SQL info
configure_file(../resources/schema.sql ${CMAKE_BINARY_DIR}/mtgdb/schema.sql COPYONLY)
