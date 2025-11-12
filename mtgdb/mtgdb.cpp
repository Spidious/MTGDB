// mtgdb.cpp : Defines the entry point for the application.
//

#include "mtgdb.h"

using namespace std;



int main(int argc, char* argv[])
{
	// Check for CLI args and pass to handler if it exists
	if (argc > 1) cli_handler(argc, argv);

	// Open Database
	DBInterface db(DB_SQLITE, "test.db");




	// Create search based on CLI input (Basic start, only use advanced search)
	// $ mtgdb -S type:spider,is:commander


	return 0;
}


