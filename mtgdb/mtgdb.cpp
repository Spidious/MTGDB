// mtgdb.cpp : Defines the entry point for the application.
//

#include "mtgdb.h"

using namespace std;
using namespace Scryfall;
int main()
{
	// Open Database
	DBInterface db(DB_SQLITE, "test.db");

	// Create api
	ScryfallAPI api;

	//// search for card based on ID
	//auto result1 = api.BasicSearch("5b94f37f-ebdf-4b79-a615-58331d27cf4e");
	//auto result2 = api.BasicSearch("random");


	string search_prompt = api.parse_query
		({
			{
				"type", ":",
				"land creature"
			}
		});

	cout << "SCRYFALL SEARCH: " << search_prompt << endl;


	std::unique_ptr<ScryfallObject> res = api.AdvancedSearch(search_prompt);


	if (res->get_attr("object") == "error")
	{
		cerr << "API Call failed with: "
			<< endl
			<< ">> Code "
			<< res->get_attr("code")
			<< endl
			<< ">> Description: "
			<< res->get_attr("details")
			<< endl;
	}

	if (res->get_attr("object") == "list")
	{
		cout << "Total Results: " << res->get_attr("total_cards") << endl;
		// auto data = res->get_data();
		// for (auto res->)
		// {
		// 	cout << "[" << i+1 << "] " << res->get_attr("data")[i]["name"] << endl;
		// }
	}

	return 0;
}
