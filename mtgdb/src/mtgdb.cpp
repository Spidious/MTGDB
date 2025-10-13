// mtgdb.cpp : Defines the entry point for the application.
//

#include "mtgdb.h"
#include <db_interface.h>

using namespace std;

int main()
{
	DBInterface db;
	db.Execute("CREATE TABLE IF NOT EXISTS cards (id INTEGER PRIMARY KEY, name TEXT, mana_cost TEXT, type_line TEXT, oracle_text TEXT);");
	return 0;
}
