#include <cli_handler.h>

int cmd_search(const cxxopts::ParseResult& result) {
    std::cout << "Running search..." << std::endl;
    // your logic here
    return 0;
}

int cmd_random(const cxxopts::ParseResult& result) {
    try
    {
        Scryfall::ScryfallAPI api;
        auto res = api.BasicSearch("random");
        std::cout << res->get_name() << std::endl;
        return 0;
    } catch (...)
    {
        return -1;
    }
}


int cli_handler (int argc, char * argv[])
{
    // Create opts
    PARAM_OPTS.add_options()
        ("command", "Command to be run", cxxopts::value<std::string>()->implicit_value("implicit"))
        ("N,name", "Search for card given a name", cxxopts::value<std::string>())
        ("T,type", "Search for a card given a type", cxxopts::value<std::string>())
    ;

    // Parse the cli options
    PARAM_OPTS.parse_positional({"command"});
    auto cli_opts = PARAM_OPTS.parse(argc, argv);

    return (exec_handlers[cli_opts["command"].as<std::string>()])(cli_opts);
}