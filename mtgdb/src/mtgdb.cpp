// mtgdb.cpp : Defines the entry point for the application.
//

#include "mtgdb.h"
#include <sqlite3.h>

using namespace std;

int main()
{
    sqlite3* db;
    char* errMsg = nullptr;

    int rc = sqlite3_open("test.db", &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return rc;
    }

    const char* sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, name TEXT);";
    rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
    else {
        std::cout << "Database created and table added!" << std::endl;
    }

    sqlite3_close(db);
	return 0;
}
