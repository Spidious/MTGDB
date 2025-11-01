if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET mtgdb PROPERTY CXX_STANDARD 20)
endif()

enable_testing()

find_package(GTest CONFIG REQUIRED)
add_executable(unit_tests test.cpp)
target_linK_libraries(unit_tests PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main mtgdb_lib)

add_test(NAME all_tests COMMAND unit_tests)

#include(FetchContent)
#FetchContent_Declare(
#        googletest
#        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
#)

# For Windows: Prevent overriding the parent project's compiler/linker settings
#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
#FetchContent_MakeAvailable(googletest)