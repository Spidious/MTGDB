// mtgdb.cpp : Defines the entry point for the application.
//

#include "mtgdb.h"

using namespace std;

int main()
{
	// TODO: DB Type should be configurable by user.
	// Open Database
	DBInterface db(DB_SQLITE, "test.db");
	ScryfallAPI api;
	cout << "Random card by ID:" << endl;
	api.BasicSearch("5b94f37f-ebdf-4b79-a615-58331d27cf4e");
	cout << endl;
	cout << "Searching random card by name:" << endl;
	api.BasicSearch("Mechanical Mobster", "Artifact", "", 168);
	return 0;
}
