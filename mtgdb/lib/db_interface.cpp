#include "db_interface.h"

using namespace std;

/*
#################################################
# Abstract DatabaseConnector Implementation
#################################################
*/

// Default implementation of Close (does nothing)
void DatabaseConnector::Close() {}

// Virtual destructor to ensure proper cleanup of derived classes
DatabaseConnector::
~DatabaseConnector() {
	Close();
	cout << "DatabaseConnector destroyed" << endl;
}

/*
#################################################
# SQLite Connector Implementation
#################################################
*/

// Constructor to initialize SQLite connection variables
DBInterface::sqlite_connector::
sqlite_connector() {
	db = nullptr;
	errMsg = nullptr;
	rc = 0;
}

// Connect to SQLite database using the provided URI
bool DBInterface::sqlite_connector::
Connect(const char* uri) {
	rc = sqlite3_open(uri, &db);
	return(rc == SQLITE_OK);
}

// Execute SQL command on SQLite database
bool DBInterface::sqlite_connector::
Execute(const char* sql) {
	rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);
	return(rc == SQLITE_OK);
}

// Close SQLite database connection
void DBInterface::sqlite_connector::
Close() {
	sqlite3_close(db);
}

/*
#################################################
# MySQL Connector Implementation
#################################################
*/

bool DBInterface::mysql_connector::
Connect(const char* uri) {
	return true; // Placeholder implementation
}

bool DBInterface::mysql_connector::
Execute(const char* sql) {
	return true; // Placeholder implementation
}

void DBInterface::mysql_connector::
Close() {
	// Placeholder implementation
}

/*
#################################################
# MariaDB Connector Implementation
#################################################
*/

bool DBInterface::mariadb_connector::
Connect(const char* uri) {
	return true; // Placeholder implementation
}

bool DBInterface::mariadb_connector::
Execute(const char* sql) {
	return true; // Placeholder implementation
}

void DBInterface::mariadb_connector::
Close() {
	// Placeholder implementation
}

/*
#################################################
# DBInterface Implementation
#################################################
*/


// Default constructor to initialize database connection
DBInterface::DBInterface(const int db_specifier, const char* uri) {
	// Choose database connector based on db_specifier
	if (db_specifier == DB_SQLITE) {
		connector = new sqlite_connector();
	}
	else if (db_specifier == DB_MYSQL) {
		connector = new mysql_connector();
	}
	else if (db_specifier == DB_MARIADB) {
		connector = new mariadb_connector();
	}
	else {
		// Invalid database specifier
		throw invalid_argument("Invalid database specifier");
	}

	// Create connector and initialize connection
	connector = new sqlite_connector();
	if(!connector->Connect(uri)) {
		throw runtime_error("Failed to connect to database");
	}
}

// Execute SQL command on database
void DBInterface::Execute(const char* sql) {
	if (!connector->Execute(sql)) {
		cerr << "SQL error" << endl;
	}
}

// Close database connection
void DBInterface::Close() {
	connector->Close();
}

// Destructor to close database connection
DBInterface::~DBInterface() {
	// Close database connection
	Close();
	cout << "Database connection closed!!!" << endl;
}