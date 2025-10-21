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
	APIResult result1;

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
	cout << result1["name"] << endl;/*
	cout << result2["name"] << endl;
	result1 = result2;
	cout << result1["name"] << endl;*/


	return 0;
}
