#include "db_interface.h"
using namespace std;
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
	rc = sqlite3_open_v2(uri, &db, SQLITE_OPEN_READWRITE, nullptr);
	if (rc == SQLITE_CANTOPEN)
	{
		cout << "DB Doesn't exist" << endl;
		// Create database
		rc = sqlite3_open_v2(uri, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
		// Retrieve default schema
		fstream schema_file;
		schema_file.open("schema.sql", ios::in); // TODO: Stop using hard-coded schema name
		if (!schema_file.is_open()) {
			cout << "Failed to open schema.sql\n";
		}
		std::string schema((std::istreambuf_iterator<char>(schema_file)), std::istreambuf_iterator<char>());
		// Execute schema
		if (rc == SQLITE_OK) {
			cout << "Executing Schema" << endl;
			sqlite_connector::Execute(schema.c_str());
			rc = SQLITE_OK; // Execute() causes this to change.
		}
	}
	return(rc == SQLITE_OK);
}

// Execute SQL command on SQLite database
bool DBInterface::sqlite_connector::
Execute(const char* sql) {
	rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);
	return(rc == SQLITE_OK);
}

char* DBInterface::sqlite_connector::
GetErr() const {
	return errMsg;
}

// Close SQLite database connection
void DBInterface::sqlite_connector::
Close() {
	sqlite3_close(db);
}

// Close DB connection when destroyed
DBInterface::sqlite_connector::
~sqlite_connector(){
	sqlite_connector::Close();
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

// Close DB connection when destroyed
DBInterface::mysql_connector::
~mysql_connector(){
	mysql_connector::Close();
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

// Close DB connection when destroyed
DBInterface::mariadb_connector::
~mariadb_connector() {
	mariadb_connector::Close();
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
}