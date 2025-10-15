#ifndef SCRYFALL_API_H
#define SCRYFALL_API_H
//#define CPPHTTPLIB_OPENSSL_SUPPORT
#pragma once

#include <iostream>
#include <mutex>
#include <string>
#include <thread>
//#include <httplib.h>
#include <curl/curl.h>
#include <sstream>

#define SCRYFALL_API_DELAY_MS 75 // Must be 50-100ms between requests
#define SCRYFALL_API_ENDPOINT "https://api.scryfall.com"
#define API_ENDPOINT_SEARCH "/cards/search?order=cmc&q="
#define API_ENDPOINT_ID "/cards/"

using namespace std;

class ScryfallAPI {
	static mutex api_mutex;
	//httplib::Client cli;

	static string url_encode(const string& str_value);
	string call_api(const string& path);

public:
	ScryfallAPI();
	string BasicSearch(const string& id);
	string BasicSearch(const string& name, const string& type, const string& set = "", const int collect_num = 0);
	string AdvancedSearch(const string& query);
};


#endif
