#include "scryfall_api.h"

ScryfallAPI::ScryfallAPI() {
	// TODO: Remove this
	call_api("test");
	// Constructor Implementation Placeholder
}

string ScryfallAPI::
url_encode(const string& value) {
	return ""; // Placeholder Implementation
}

string ScryfallAPI::
call_api(const string& query) {
	// Ensure simultaneous API calls are not being made. 
	api_mutex.lock();

	httplib::Client cli(SCRYFALL_API_ENDPOINT);
	auto res = cli.Get("cards/random"); // Test implementation
	if (res)
		std::cout << res->body << std::endl;
	else
		std::cout << "Request failed\n";

	// Enforce Scryfall API rate limit
	this_thread::sleep_for(chrono::milliseconds(SCRYFALL_API_DELAY_MS));
	api_mutex.unlock();
	return ""; // Placeholder Implementation
}

string ScryfallAPI::
BasicSearch(const string& id) {
	return ""; // Placeholder Implementation
}

string ScryfallAPI::
BasicSearch(const string& name, const string& type, const string& set, const int collect_num) {
	return ""; // Placeholder Implementation
}

string ScryfallAPI::
AdvancedSearch(const string& query) {
	return ""; // Placeholder Implementation
}