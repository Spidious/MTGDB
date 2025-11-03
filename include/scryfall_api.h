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
#include <unordered_set>
// #include <sstream>

#define SCRYFALL_API_DELAY_MS 75 // Must be 50-100ms between requests
#define SCRYFALL_API_ENDPOINT "https://api.scryfall.com"
#define API_ENDPOINT_SEARCH "/cards/search?order=cmc&q="
#define API_ENDPOINT_ID "/cards/"

using namespace std;
using namespace nlohmann;

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

const unordered_set<string> SearchOps = {
	":", ">", "<", ">=", "<=", "!=", "-"
};

struct SearchItem {
	string field;
	string op;
	string value;
} typedef searchitem;

/// <summary>
/// Represents the result of an API call, inheriting from std::string.
/// Specifically designed to parse and handle Scryfall API responses.
/// </summary>
class APIResult : private json{
private:

	/// <summary>
	/// Holds type of json object 
	/// Available types: "error", "list", "set", "card", "ruling", "card_symbol", "catalog", "bulk_data"
	/// </summary>
	string object_type; 

	/// <summary>
	/// Holds status codes. <0 = exceptions && >0 = http status
	/// </summary>
	int status;

	/// <summary>
	/// Hold a pointer to an exception (when status <0)
	/// </summary>
	exception* e_ptr;

	/// <summary>
	/// Parse and update a new string 
	/// Primary point of entry for JSON
	/// </summary>
	/// <param name="str">The string JSON input</param>
	void update_parse(const string& str);

public:

	/// <summary>
	/// JSON string input constructor
	/// </summary>
	/// <param name="json">The string JSON input</param>
	explicit APIResult(const string& json = "{}");

	/// <summary>
	/// << operator override
	/// </summary>
	/// <param name="str">The string JSON input</param>
	/// <returns>Pointer to current APIResult</returns>
	APIResult& operator<<(const string& str);

	/// <summary>
	/// Allows retrieval of json data
	/// </summary>
	using json::dump;

	/// <summary>
	/// Use base class operator in public access
	/// </summary>
	using json::operator[];

	/// <summary>
	/// Specifies default destructor
	/// </summary>
	~APIResult() = default;
};

/// <summary>
/// Callback function for writing received data, typically used with data transfer libraries such as libcurl.
/// </summary>
/// <param name="ptr">Pointer to the received data buffer.</param>
/// <param name="size">Size of each data element in bytes.</param>
/// <param name="nmemb">Number of data elements to write.</param>
/// <param name="userdata">Pointer to user-defined data or context.</param>
/// <returns>The number of bytes actually handled. Returning a value different from (size * nmemb) will signal an error to the caller.</returns>
static size_t write_callback(char* ptr, size_t size, size_t nmemb, void* userdata);

/// <summary>
/// Api interface for api.scryfall.com
/// </summary>
class ScryfallAPI {

	/// <summary>
	/// Mutex to ensure thread safety and rate limiting
	/// </summary>
	static mutex api_mutex;

	/// <summary>
	/// A pointer to http headers
	/// </summary>
	struct curl_slist* headers;

	/// <summary>
	/// A pointer to the curl client
	/// </summary>
	CURL* cli;

	/// <summary>
	/// Store the result of the API call
	/// </summary>
	APIResult res;

	/// <summary>
	/// Static function to encode strings with % encoding for URLs
	/// </summary>
	/// <param name="str_value">String value to be encoded</param>
	/// <returns>Encoded string</returns>
	static string url_encode(const string& str_value);

	/// <summary>
	/// Validate the search query. 
	/// Query should be ready for URL (Decoded): c:rg+mana:{G}+t:bird
	/// </summary>
	/// <param name="str_value"></param>
	/// <returns></returns>
	static bool validate_search(const string& query);

	/// <summary>
	/// Calls an api endpoint, enforcing rate limit
	/// </summary>
	/// <param name="path">The specific endpoint needing to be called</param>
	/// <param name="buffer">Buffer in which to put the
	/// <returns>Result of the API call</returns>
	CURLcode call_api(const string& path, const void* buffer);

	/// <summary>
	/// Return a APIResult representing a failed request/result
	/// </summary>
	/// <param name="e">Pointer to the exception</param>
	/// <param name="code">custom error code (default -1)</param>
	/// <returns>APIResult object with status set to fail code</returns>
	void res_update(const exception* e, const int code = -1);

	/// <summary>
	/// Return an APIResult parsed with JSON
	/// Creates necessary objects for returned data
	/// </summary>
	/// <param name="json">string to raw JSON</param>
	/// <returns></returns>
	void res_update(const string& json);

public:

	/// <summary>
	/// recursive parse a scryfall API query into base format (Non-encoded)
	/// </summary>
	/// <param name="queries">list of {field, operator, value} ex. => {{"name", ":", "fish"}, {"mana", ">=", "ug"}}</param>
	/// <returns>fully parsed array of search queries. ex. => name:fish+man>=ug</returns>
	static string parse_query(vector<searchitem> queries);

	/// <summary>
	/// An API constructor that initializes the curl client and sets required headers
	/// </summary>
	ScryfallAPI();

	/// <summary>
	/// Search for a card by its unique SCRYFALL ID
	/// Can also be used with any of the special keywords: random, random-rare, random-foil, random-art
	/// </summary>
	/// <param name="id">Scryfall ID or Keyword</param>
	/// <returns>Raw API Result</returns>
	void BasicSearch(const string& id);

	/// <summary>
	/// Performs a basic search using the specified name, type, and optional set and collection number.
	/// </summary>
	/// <param name="name">The name to search for.</param>
	/// <param name="type">The type to search for.</param>
	/// <param name="set">The set to search within (optional).</param>
	/// <param name="collect_num">The collection number to search for (optional).</param>
	/// <returns>Raw API Result</returns>
	void BasicSearch(const string& name, const string& type, const string& set = "", int collect_num = 0);

	/// <summary>
	/// Performs a scryfall search using the advanced query syntax on https://scryfall.com/docs/syntax
	/// </summary>
	/// <param name="query">String search query</param>
	/// <returns>Raw API Result</returns>
	void AdvancedSearch(const string query);

	const APIResult& GetResult() const;

	

	/// <summary>
	/// Destroys the ScryfallAPI object and releases any associated resources.
	/// </summary>
	~ScryfallAPI();
};

/********************************************************************************************************
* Scryfall API Exceptions
********************************************************************************************************/

/// <summary>
/// General base exception 
/// </summary>
class Scryfall_Exception : public exception {
private:
	string message;
	
public:
	Scryfall_Exception(const string& msg = "Unknown Exception") : message(msg) {}

	/// <summary>
	/// Returns a char* string representing the error being thrown.
	/// </summary>
	/// <returns>char array</returns>
	const char* what() const noexcept override {
		return message.c_str();
	}
};

/// <summary>
/// Thrown when search query is incorrect or invalid
/// </summary>
class Invalid_Search_Query : public Scryfall_Exception {
public:
	Invalid_Search_Query(const string msg) : Scryfall_Exception("Exception - Invalid Search Query : " + msg) {}
};

/// <summary>
/// Thrown when API returns invalid response
/// </summary>
class Invalid_Response : public Scryfall_Exception {
public:
	Invalid_Response(const string msg) : Scryfall_Exception("Exception - Invalid API Response : " + msg) {}
};


#endif
