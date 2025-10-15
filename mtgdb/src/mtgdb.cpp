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
	std::string result = api.BasicSearch("random"); // or call_api("/cards/random")

	//// search for card based on ID
	//cout << "Random card by ID:" << endl;
	//api.BasicSearch("5b94f37f-ebdf-4b79-a615-58331d27cf4e");
	//cout << endl;

	//// Search for card based on information
	//cout << "Searching random card by name:" << endl;
	//api.BasicSearch("Mechanical Mobster", "Artifact", "", 168);

	return 0;
}
