#include "db_interface.h"
#include <sqlite3.h>

using namespace std;

// Default constructor to initialize database connection
DBInterface::DBInterface() {
	errMsg = nullptr;
	rc = sqlite3_open("test.db", &db);
	// Initialize database connection
	if (rc) {
		cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
	}
	else {
		cout << "Opened database successfully" << endl;
	}
}

// Execute SQL command on database
void DBInterface::Execute(const char* sql) {
	// Execute SQL command
	rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);
	if (rc != SQLITE_OK) {
		cerr << "SQL error: " << errMsg << endl;
		sqlite3_free(errMsg);
	}
	else {
		cout << "SQL executed successfully" << endl;
	}
}

// Close database connection
void DBInterface::Close() {
	// Close database connection
	sqlite3_close(db);
}

// Destructor to close database connection
DBInterface::~DBInterface() {
	// Close database connection
	Close();
	cout << "Database connection closed" << endl;
}