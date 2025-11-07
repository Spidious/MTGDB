#ifndef SCRYFALL_API_H
#define SCRYFALL_API_H
//#define CPPHTTPLIB_OPENSSL_SUPPORT
#pragma once

#include <iostream>
#include <mutex>
#include <string>
#include <thread>
// #include <iomanip>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <vector>
#include <set>
#include <unordered_set>
#include <memory>
// #include <sstream>

#define SCRYFALL_API_DELAY_MS 75 // Must be 50-100ms between requests
#define SCRYFALL_API_ENDPOINT "https://api.scryfall.com"
#define API_ENDPOINT_SEARCH "/cards/search?order=cmc&q="
#define API_ENDPOINT_ID "/cards/"

using namespace std;
using namespace nlohmann;

namespace Scryfall
{
	/********************************************************************************************************
	* Global Namespace Variables																			*
	********************************************************************************************************/

	/**
	 * Set of all supported Scryfall object types
	 */
	const unordered_set<string> ObjectTypes = {
		"empty", "error", "list", "set", "card", "ruling", "card_symbol", "catalog", "bulk_data"
	};

	/**
	 * Set of fields used in Scryfall Searches
	 */
	const unordered_set<string> SearchFields = {
		"n", "name",
		"c", "color",
		"id", "identity",
		"t", "type",
		"has", "is", "include", "in",
		"o", "oracle", "fo", "fulloracle",
		"kw", "keyword",
		"m", "mana", "manavalue", "devotion", "produces",
		"pow", "power", "tou", "toughness", "loy", "loyalty",
		"r", "rarity",
		"s", "set", "e", "edition", "cn", "number", "b", "block",
		"cheapest", "tix", "usd", "eur",
		"a", "artist", "artists",
		"ft", "flavor", "wm", "watermark", "illustrations",
		"border", "frame", "stamp",
		"year", "date",
		"art", "arttag", "atag", "function", "otag", "oracletag",
		"lang", "language",
		"new",
	};

	/**
	 * Set of operators used in Scryfall searches
	 */
	const unordered_set<string> SearchOps = {
		":", ">", "<", ">=", "<=", "!=", "-"
	};

	/**
	 * Search item used in Scrfall API searches
	 */
	struct SearchItem {
		string field;
		string op;
		string value;
	} typedef searchitem;

	/********************************************************************************************************
	* Helper Functions																						*
	********************************************************************************************************/

	/**
	 * Callback function for writing received data, typically used with data transfer libraries such as libcurl.
	 * @param ptr Pointer to the received data buffer.
	 * @param size Size of each data element in bytes.
	 * @param nmemb Number of data elements to write.
	 * @param userdata Pointer to user-defined data or context.
	 * @return The number fo bytes actually handled. Returning a value different from size * nmemb indicates error
	 */
	static size_t write_callback(char* ptr, size_t size, size_t nmemb, void* userdata);

	/********************************************************************************************************
	* Scryfall Object and derived classes																	*
	********************************************************************************************************/

	/**
	 * Polymorphic Object containing the JSON information returned by Scryfall API
	 */
	class ScryfallObject : protected json
	{
		/**
		 * String object type (To determine ScryfallObject derived class)
		 */
		const std::string obj_type;

	protected:
		/**
		 * Generate ScryError json based object
		 * @param err String description of error
		 * @param status Integer status (Default: -1)
		 * @return Unique ScryfallObject pointer to ScryError
		 */
		static std::unique_ptr<ScryfallObject> generate_error(const std::string err, const int status = -1);

	public:
		/**
		 * Constructor to build Scryfall Object
		 * @param j Json object
		 */
		explicit ScryfallObject(const json& j);

		/**
		 * Factory function -> Determine proper derived class given JSON string
		 * @param json String JSON Scryfall object
		 * @return Unique Scryfall Object Pointer to Scryfall Object or derived class
		 * @exception ScryError Object ScryError unique pointer is returned upon fail
		 */
		static std::unique_ptr<ScryfallObject> from_json(const string& json);

		/**
		 *	Factory function -> Determine proper derived class given JSON string
		 * @param json JSON Scryfall object
		 * @return Unique Scryfall Object Pointer to Scryfall Object or derived class
		 * @exception ScryError Object ScryError unique pointer is returned upon fail
		 */
		static std::unique_ptr<ScryfallObject> from_json(const json& json);

		/**
		 * Retrieve the object type value
		 * @return string object type
		 */
		std::string get_object_type() const;

		/**
		 * Retrieve the name value of object
		 * @return Name of object or empty string if one is unavailable
		 */
		virtual std::string get_name() const;

		/**
		 * Retrieve attribute given the Key (arg)
		 * @tparam T Expected type of returned json argument
		 * @param arg Key
		 * @param default_value Default value if one is not found (Default T{})
		 * @return Value of Json object at key "arg" of type "T"
		 * @todo Handle errors for edge-cases
		 */
		template <typename T> T get_attr(const std::string& arg, const T& default_value = T{}) const {
			return this->value(arg, default_value);
		}

		/**
		 * Default destructor
		 */
		virtual ~ScryfallObject() = default;
	};

	/**
	 * ScryfallObject derived class for Error objects
	 * @todo potentially make this inherit std::exception
	 */
	class ScryError final: public ScryfallObject
	{
		/**
		 * Message or description of the error
		 */
		std::string err_msg;

	public:
		/**
		 * Basic constructor
		 * @param j JSON object
		 */
		explicit ScryError(const json& j);

		/**
		 * Typical
		 * @return Return the err_msg
		 */
		std::string what();

		/**
		 * Default destructor
		 */
		~ScryError() override = default;
	};

	/**
	 * ScryfallObject derived class for List objects
	 */
	class ScryList final: public ScryfallObject
	{
		/**
		 * Direct API url for continuation of list
		 */
		std::string next_url;
		/**
		 * vector array of objects contained in List
		 */
		std::vector<std::shared_ptr<ScryfallObject>> data;

		/**
		 * Non-Polymorphic call function to retrieve the next_url
		 * @return another list object unique pointer
		 */
		std::unique_ptr<ScryfallObject> call_next_list() const;

	public:
		/**
		 * Basic Constructor
		 * @param j JSON object
		 */
		explicit ScryList(const json& j);

		/**
		 * Retrieves the length of the associated list of data
		 * @return integer size of data vector
		 */
		int size() const;

		/**
		 * Retrieves the shared pointer to the object at vector index
		 * @param index integer index
		 * @return shared pointer to object at index "index"
		 * @todo disallow indexes greater than or equal to size()
		 */
		std::shared_ptr<ScryfallObject> at(int index) const;

		/**
		 * Default destructor
		 */
		~ScryList() override = default;
	};

	/**
	 * ScryfallObject derived class for Set objects
	 */
	class ScrySet final: public ScryfallObject
	{
	public:
		/**
		 * Basic Constructor
		 * @param j JSON object
		 */
		explicit ScrySet(const json& j) : ScryfallObject(j) {}

		/**
		 * Default destructor
		 */
		~ScrySet() override = default;
	};

	/**
	 * ScryfallObject derived class for Card objects
	 */
	class ScryCard final: public ScryfallObject
	{
	public:
		/**
		 * Basic Constructor
		 * @param j JSON object
		 */
		explicit ScryCard(const json& j) : ScryfallObject(j) {}

		/**
		 * Return the name of the Card
		 * @return String card name
		 */
		std::string get_name() const override;

		/**
		 * Default destructor
		 */
		~ScryCard() override = default;
	};

	/**
	 * ScryfallObject derived class for Card objects
	 */
	class ScryRuling final: public ScryfallObject
	{
	public:
		/**
		 * Basic Constructor
		 * @param j JSON object
		 */
		explicit ScryRuling(const json& j) : ScryfallObject(j) {}

		/**
		 * Default destructor
		 */
		~ScryRuling() override = default;
	};

	/********************************************************************************************************
	* Scryfall API Wrapper																					*
	********************************************************************************************************/

	/**
	 * Scryfall API wrapper
	 */
	class ScryfallAPI {

		/**
		 * Mutex to ensure thread safety and rate limiting
		 */
		static mutex api_mutex;

		/**
		 * A pointer to http headers
		 */
		struct curl_slist* headers;

		/**
		 * A pointer to the CURL client
		 */
		CURL* cli;

		/**
		 * Encode strings with % encoding for URLs
		 * @param str_value String value to be encoded
		 */
		static string url_encode(const string& str_value);

		/**
		 * Validate the search query.
		 * @param query string query ready for URL (Decoded): c:rg+mana:{G}+t:bird
		 * @return boolean value denoting whether validation passed/failed
		 */
		static bool validate_search(const string& query);

		/**
		 * CURL call to the API
		 * @param path string https path to API
		 * @param buffer buffer in which to write the string
		 * @return CURLcode status
		 */
		CURLcode call_api(const string& path, const void* buffer);

		/**
		 * mark ScryLists as friend so that they may access private members
		 * @todo: Instead of this, create some kind of public raw_call for the API
		 */
		friend ScryList;

	public:

		/**
		 * Recursive parse a Scryfall API query into base format
		 * @param queries List of {field, operator, value} ex. => {{"name", ":", "fish"}, {"mana", ">=", "ug"}}
		 * @return Fully parsed array of search quereis. ex. => name:fish+man>=ug
		 */
		static string parse_query(vector<searchitem> queries);

		/**
		 * Basic Constructor
		 */
		ScryfallAPI();

		/**
		 * Basic API search based on string arg
		 * @param id basic search keywords: <Scryfall_ID>, random, random-rare, random-foil, random-art
		 * @return Unique pointer to ScryfallObject or derived class
		 */
		std::unique_ptr<ScryfallObject> BasicSearch(const string& id);

		/**
		 * Scryfall
		 * @param query
		 * @return
		 */
		std::unique_ptr<ScryfallObject> AdvancedSearch(const string query);


		/**
		 * Default destructor
		 */
		~ScryfallAPI();
	};

	/********************************************************************************************************
	* Scryfall API Exceptions																				*
	********************************************************************************************************/

		/**
		 * Default exception for Scryfall objects
		 */
		class Scryfall_Exception : public exception {
		private:
			/**
			 * error message for objects
			 */
			string message;

		public:
			/**
			 * Exception Constructor
			 * @param msg error message for exception
			 */
			Scryfall_Exception(const string& msg = "Unknown Exception") : message(msg) {}

			/**
			 * Output exception message
			 * @return char* msg
			 */
			const char* what() const noexcept override {
				return message.c_str();
			}
		};

		/**
		 * Derived exception for Invalid searches
		 */
		class Invalid_Search_Query : public Scryfall_Exception {
		public:
			/**
			 * Exception Constructor
			 * @param msg Error message for exception
			 */
			Invalid_Search_Query(const string msg) : Scryfall_Exception("Exception - Invalid Search Query : " + msg) {}
		};

		/**
		 * Derived exception for Invalid responses
		 */
		class Invalid_Response : public Scryfall_Exception {
		public:
			/**
			 * Exception Constructor
			 * @param msg Error message for exception
			 */
			Invalid_Response(const string msg) : Scryfall_Exception("Exception - Invalid API Response : " + msg) {}
		};
	}



#endif
