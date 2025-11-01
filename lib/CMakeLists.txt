add_library(mtgdb_lib
    api_result.cpp
    db_interface.cpp
    scryfall_api.cpp
    sqlite3.c
)

target_include_directories(mtgdb_lib PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(mtgdb_lib PUBLIC
    CURL::libcurl
    nlohmann_json::nlohmann_json
)