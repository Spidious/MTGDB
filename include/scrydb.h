//
// Created by Luke Deffenbaugh on 11/19/2025.
//

#ifndef MTGDB_SCRYDB_H
#define MTGDB_SCRYDB_H

#include <scryfall_api.h>
#include <db_interface.h>
#include <unordered_set>

#define SCRYDB_SERVER DB_SQLITE
#define SCRYDB_URI "scrydb.db"

/**
 * Output Codes:
 * 0 : Success
 * 1 : Cannot open database
 */

namespace Scryfall
{
    class scrydb
    {
        static std::unordered_set<string> db_lock;

        std::unique_ptr<DBInterface> db = nullptr;

        const string db_uri;
        int open_db();

    public:
        scrydb(const string uri);
        scrydb() = delete;

        ~scrydb();



    };
} // Scryfall

#endif //MTGDB_SCRYDB_H