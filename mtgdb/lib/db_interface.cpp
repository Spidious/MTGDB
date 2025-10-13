#include "db_interface.h"

using namespace std;

void DatabaseConnector::Close() {}

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

DBInterface::sqlite_connector::
sqlite_connector() {
	db = nullptr;
	errMsg = nullptr;
	int rc = 0;
}

bool DBInterface::sqlite_connector::
Connect(const char* uri) {
	rc = sqlite3_open(uri, &db);
	return(rc == SQLITE_OK);
}

bool DBInterface::sqlite_connector::
Execute(const char* sql) {
	rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);
	return(rc == SQLITE_OK);
}

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
	return true;
}

bool DBInterface::mysql_connector::
Execute(const char* sql) {
	return true;
}

void DBInterface::mysql_connector::
Close() {
}

/*
#################################################
# MariaDB Connector Implementation
#################################################
*/

bool DBInterface::mariadb_connector::
Connect(const char* uri) {
	return true;
}

bool DBInterface::mariadb_connector::
Execute(const char* sql) {
	return true;
}

void DBInterface::mariadb_connector::
Close() {
}

/*
#################################################
# DBInterface Implementation
#################################################
*/


// Default constructor to initialize database connection
DBInterface::DBInterface() {
	connector = new sqlite_connector();
	if(!connector->Connect("test.db")) {
		cerr << "Can't open database" << endl;
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