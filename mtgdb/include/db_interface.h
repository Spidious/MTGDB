// db_interface.h : Include file for standard system include files,
// or project specific include files.

#ifndef DB_INTERFACE_H
#define DB_INTERFACE_H
#pragma once
#include <iostream>
#include <sqlite3.h>

class DBInterface {
	sqlite3* db;
	char* errMsg;
	int rc;

	public:
		/// <summary>
		/// Initialize the database connection 
		/// TODO: Handle SQLite or MySQL/MariaDB connections
		/// TODO: Add connection parameters (host, user, password, dbname)
		/// </summary>
		DBInterface();

		/// <summary>
		/// Execute database commands
		/// </summary>
		/// <param name="sql">SQL query to be run on database</param>
		void Execute(const char* sql);

		/// <summary>
		/// Close current database connection
		/// </summary>
		void Close();

		/// <summary>
		///	Call close on destruction
		/// </summary>
		~DBInterface();
};

#endif 

// TODO: Reference additional headers your program requires here.
