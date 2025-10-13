// db_interface.h : Include file for standard system include files,
// or project specific include files.

#ifndef DB_INTERFACE_H
#define DB_INTERFACE_H
#pragma once
#include <iostream>
#include <sqlite3.h>

/// <summary>
/// Establishes a connection to a database using the specified URI.
/// </summary>
class DatabaseConnector {
	public:
		DatabaseConnector() = default;
		/// <summary>
		/// Establishes a connection to the specified URI.
		/// </summary>
		/// <param name="uri">A null-terminated string containing the URI to connect to.</param>
		/// <returns>True if the connection was successful; otherwise, false.</returns>
		virtual bool Connect(const char* uri) = 0;
		/// <summary>
		/// Executes the specified SQL statement.
		/// </summary>
		/// <param name="sql">A pointer to a null-terminated string containing the SQL statement to execute.</param>
		/// <returns>True if the SQL statement was executed successfully; otherwise, false.</returns>
		virtual bool Execute(const char* sql) = 0;
		/// <summary>
		/// Closes the resource or connection associated with the object.
		/// </summary>
		virtual void Close() = 0;
		/// <summary>
		/// Destroys the DatabaseConnector object and releases any associated resources.
		/// </summary>
		virtual ~DatabaseConnector() = 0;
};

/// <summary>
/// Provides an interface for managing database connections and executing SQL commands.
/// </summary>
class DBInterface {
	DatabaseConnector* connector;

	public:
		/// <summary>
		/// Initialize the database connection 
		/// TODO: Handle SQLite or MySQ	    L/MariaDB connections
		/// TODO: Add connection parameters (host, user, password, dbname)
		/// </summary>
		/// <param name="db_specifier">Specifier for database types DB_SQLITE, DB_MYSQL, DB_MARIADB</param>
		/// <param name="uri">URI (or path) for database connection</param>
		/// <returns>testing</returns>
		/// <exception cref="std::invalid_argument">Thrown when an invalid database specifier is provided.</exception>
		/// <exception cref="std::runtime_error">Thrown when the connection to the database fails.</exception>
		DBInterface(const int db_specifier, const char* uri);

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

	private:
		/// <summary>
		/// Database connector for SQLite
		/// </summary>
		class sqlite_connector : public DatabaseConnector {
			sqlite3* db;
			char* errMsg;
			int rc;

			public:
				sqlite_connector();
				bool Connect(const char* uri) override;
				bool Execute(const char* sql) override;
				void Close() override;
		};
		/// <summary>
		/// Database Connector for MySQL
		/// </summary>
		class mysql_connector : public DatabaseConnector {
			public:
				mysql_connector() = default;
				bool Connect(const char* uri) override;
				bool Execute(const char* sql) override;
				void Close() override;
		};
		/// <summary>
		/// Database Connector for MariaDB
		/// </summary>
		class mariadb_connector : public DatabaseConnector {
			public:
				mariadb_connector() = default;
				bool Connect(const char* uri) override;
				bool Execute(const char* sql) override;
				void Close() override;
		};
};

#define DB_SQLITE 1
#define DB_MYSQL 2
#define DB_MARIADB 3

#endif 


