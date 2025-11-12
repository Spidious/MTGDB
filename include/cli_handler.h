#ifndef MTGDB_CLI_HANDLER_H
#define MTGDB_CLI_HANDLER_H

#include <cxxopts.h>
#include <functional>
#include <unordered_map>
#include <scryfall_api.h>

// Global Param options varaible
inline cxxopts::Options PARAM_OPTS("MTGDB", "Scryfall Search and Inventory Database Management System");

// CMD handler functions
int cmd_search(const cxxopts::ParseResult& result);
int cmd_random(const cxxopts::ParseResult& result);

// Dispatch map table for commands to be run
inline std::unordered_map<std::string, int(*)(const cxxopts::ParseResult&)> exec_handlers = {
    {"search", cmd_search},
    {"random", cmd_random}
};

// Primary cli handler
int cli_handler (int argc, char * argv[]);



#endif //MTGDB_CLI_HANDLER_H