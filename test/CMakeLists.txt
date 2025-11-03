if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET mtgdb PROPERTY CXX_STANDARD 20)
endif()

#enable_testing()

find_package(GTest CONFIG REQUIRED)
add_executable(unit_tests test.cpp)
target_linK_libraries(unit_tests PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main mtgdb_lib)

add_test(NAME all_tests COMMAND unit_tests)

#include(GoogleTest)
#gtest_discover_tests(unit_tests)
