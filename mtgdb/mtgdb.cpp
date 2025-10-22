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
	APIResult result;

	result = api.BasicSearch("Spider", "Legendary Creature");

	cout << "NAME: ";
	if (result["total_cards"] != 0)
	{
		for (int i = 0; i < result["total_cards"]; i++)
		{
			cout << result["data"][i]["name"] << ((i+1 != result["total_cards"]) ? " ||  " : "");
		}
		cout << ((result["next_page"] == NULL) ? "" : " || ...") << endl;
	}


	return 0;
}
