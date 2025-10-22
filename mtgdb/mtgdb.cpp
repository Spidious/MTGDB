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

	APIResult result1;
	cout << "Reached" << endl;
	try {
		 result1 = api.BasicSearch("test", "test", "test", 5);
	}
	catch (Invalid_Response e) {
		cout << "Error1: " << e.what() << endl;
	}
	catch (Scryfall_Exception e) {
		cout << "Error2: " << e.what() << endl;
	}

	// Print result
	cout << result1["name"] << endl;


	return 0;
}
