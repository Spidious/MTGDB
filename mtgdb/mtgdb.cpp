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


	std::unique_ptr<ScryfallObject> res = std::move(api.AdvancedSearch(search_prompt));


	if (res->get_attr<string>("object") == "error")
	{
		cerr << "API Call failed with: "
			<< endl
			<< ">> Code "
			<< res->get_attr<string>("code")
			<< endl
			<< ">> Description: "
			<< res->get_attr<string>("details")
			<< endl;
	}

	if (res->get_attr<string>("object") == "list")
	{
		std::unique_ptr<ScryList> res_list = ScryCast<ScryfallObject, ScryList>(res);

		cout << "Total Results: " << res_list->size() << endl;
		for (int i = 0; i < res_list->size(); i++)
		{
			cout << "[" << i+1 << "] " << res_list->at(i)->get_name() << endl;
		}
	}

	return 0;
}
