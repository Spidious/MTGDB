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

	// search for card based on ID
	APIResult result = api.BasicSearch("5b94f37f-ebdf-4b79-a615-58331d27cf4e");

	// Print result
	cout << result["name"] << endl;

	return 0;
}
