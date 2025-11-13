#include <cli_handler.h>

std::vector<Scryfall::SearchItem> search_parse_args(std::string arg)
{
    std::vector<Scryfall::SearchItem> ss;
    // Tokenize the data
    // char* token_value = strtok(arg.data(), ",");

    // Compile search fields into a list separated by |
    std::string search_fields;
    for (auto field : Scryfall::SearchFields)
    {
        search_fields += field += "|";
    }
    search_fields.pop_back();

    // Compile search ops into a list separated by |
    std::string search_ops;
    for (auto op : Scryfall::SearchOps)
    {
        search_ops += op += "|";
    }
    search_ops.pop_back();

    // Building regex pattern
    std::string rx = std::format("(?:({})({})(\".+\"|[^\",]+))", search_fields, search_ops);

    // Apply regex pattern and iterate through each match
    std::regex pattern(rx);
    for (std::sregex_iterator iter(arg.begin(), arg.end(), pattern), end; iter != end; ++iter)
    {
        // append matched string into the back of the vector
        std::smatch match = *iter;
        ss.push_back({match[1], match[2], match[3]});
    }

    // Return the final vector
    return ss;
}

int cmd_search(const cxxopts::ParseResult& result) {
    try
    {
        Scryfall::ScryfallAPI api;
        std::string query = api.parse_query(search_parse_args(result["query"].as<std::string>()));
        auto res = api.AdvancedSearch(query);
        auto list_res = Scryfall::ScryCast<Scryfall::ScryfallObject, Scryfall::ScryList>(res);

        for (int i = 0; i < list_res->size(); i++)
        {
            std::cout << "[" << i << "] " << list_res->at(i)->get_attr("id", std::string("")) << " -- " << list_res->at(i)->get_name() << endl;
        }

        return 0;
    }
    catch (...)
    {
        return -1;
    }
}

int cmd_random(const cxxopts::ParseResult& result) {
    try
    {
        // Create API object and call the "random" endpoint
        Scryfall::ScryfallAPI api;
        auto res = api.BasicSearch("random");

        // Check that object is not an error
        if (res->get_object_type() == "error")
        {
            auto err_res = Scryfall::ScryCast<Scryfall::ScryfallObject, Scryfall::ScryError>(res);
            std::cout << "[ Error ] " << err_res->what() << std::endl;
            return -1;
        }

        // Print out the name of the random card
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
        ("command", "Command to be run", cxxopts::value<std::string>())
        ("query", "query associated to the command", cxxopts::value<std::string>()->default_value(""))
        ("N,name", "Search for card given a name", cxxopts::value<std::string>())
        ("T,type", "Search for a card given a type", cxxopts::value<std::string>())
    ;

    // Parse the cli options
    PARAM_OPTS.parse_positional({"command", "query"});
    auto cli_opts = PARAM_OPTS.parse(argc, argv);

    return (exec_handlers[cli_opts["command"].as<std::string>()])(cli_opts);
}