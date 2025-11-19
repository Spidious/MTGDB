//
// Created by Luke Deffenbaugh on 11/19/2025.
//

#include "scrydb.h"

namespace Scryfall
{
    std::unordered_set<string> scrydb::db_lock{};

    scrydb::scrydb(const string uri) : db_uri(uri)
    {
        // Open the DB at the URI
        if (!open_db()) return;

        // Open failed
        cerr << "Could not open database: " << uri << endl;
    }

    int scrydb::open_db()
    {
        // Check if the specific uri has already been locked
        if (db_lock.contains(SCRYDB_URI))
        {
            cerr << "Cannot open \"" << SCRYDB_URI << "\": Already exists" << endl;
            return 1;
        }
        if (db != nullptr)

        try
        {
            // insert the uri to the lock. Create the DB Interface and create a pointer out of it.
            db_lock.insert(SCRYDB_URI);
            db = std::make_unique<DBInterface>(SCRYDB_SERVER, SCRYDB_URI);

            // return the pointer.
            return 0;
        }
        catch (const std::exception& e)
        {
            // Output error and remove from db_lock
            cerr << e.what() << endl;
            db_lock.erase(SCRYDB_URI);

            // Return a nullptr for fail
            return 1;
        }
    }

    scrydb::~scrydb()
    {

    }

} // Scryfall