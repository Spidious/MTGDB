// mtgdb.cpp : Defines the entry point for the application.
//

#include "mtgdb.h"

using namespace std;



int main(int argc, char* argv[])
{
	// Check for CLI args and pass to handler if it exists
	if (argc > 1) cli_handler(argc, argv);

	Scryfall::scrydb db("scrydb.db");

	Scryfall::scrydb db2("scrydb.db");

	return 0;
}


