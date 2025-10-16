#ifndef SCRYFALL_API_H
#define SCRYFALL_API_H
//#define CPPHTTPLIB_OPENSSL_SUPPORT
#pragma once

#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <iomanip>
#include <curl/curl.h>
#include <sstream>

#define SCRYFALL_API_DELAY_MS 75 // Must be 50-100ms between requests
#define SCRYFALL_API_ENDPOINT "https://api.scryfall.com"
#define API_ENDPOINT_SEARCH "/cards/search?order=cmc&q="
#define API_ENDPOINT_ID "/cards/"

using namespace std;

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
	/// Static function to encode strings with % encoding for URLs
	/// </summary>
	/// <param name="str_value">String value to be encoded</param>
	/// <returns>Encoded string</returns>
	static string url_encode(const string& str_value);

	/// <summary>
	/// Calls an api endpoint, enforcing rate limit
	/// </summary>
	/// <param name="path">The specific endpoint needing to be called</param>
	/// <returns>Result of the API call</returns>
	CURLcode call_api(const string& path, const string* response);

public:
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
	string BasicSearch(const string& id);

	/// <summary>
	/// Performs a basic search using the specified name, type, and optional set and collection number.
	/// </summary>
	/// <param name="name">The name to search for.</param>
	/// <param name="type">The type to search for.</param>
	/// <param name="set">The set to search within (optional).</param>
	/// <param name="collect_num">The collection number to search for (optional).</param>
	/// <returns>Raw API Result</returns>
	string BasicSearch(const string& name, const string& type, const string& set = "", const int collect_num = 0);

	/// <summary>
	/// Performs a scryfall search using the advanced query syntax on https://scryfall.com/docs/syntax
	/// </summary>
	/// <param name="query">String search query</param>
	/// <returns>Raw API Result</returns>
	string AdvancedSearch(const string& query);

	/// <summary>
	/// Destroys the ScryfallAPI object and releases any associated resources.
	/// </summary>
	~ScryfallAPI();
};


#endif
