// mtgdb.cpp : Defines the entry point for the application.
//

#include "mtgdb.h"

using namespace std;



int main(int argc, char* argv[])
{
	// Check for CLI args and pass to handler if it exists
	if (argc > 1) return cli_handler(argc, argv);


	return 0;
}


