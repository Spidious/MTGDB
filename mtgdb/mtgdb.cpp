// mtgdb.cpp : Defines the entry point for the application.
//

#include "mtgdb.h"

using namespace std;
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
				"name", ":",
				"spider"
			},
			{
				"type", ":",
				"Legendary Creature"
			}
		});

	api.AdvancedSearch(search_prompt);

	const APIResult& result = api.GetResult();
	;

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

	return 0;
}
