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
	return 0;
}
