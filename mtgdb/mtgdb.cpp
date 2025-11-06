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


	const APIResult& result = api.GetResult();


	if (result["object"] == "error")
	{
		cerr << "API Call failed with: "
			<< endl
			<< ">> Code "
			<< result["code"]
			<< endl
			<< ">> Description: "
			<< result["details"]
			<< endl;
	}

	if (result["object"] == "list")
	{
		cout << "Total Results: " << result["total_cards"] << endl;
		for (int i = 0; i < result["total_cards"]; i++)
		{
			cout << "[" << i+1 << "] " << result["data"][i]["name"] << endl;
		}
	}

	return 0;
}
