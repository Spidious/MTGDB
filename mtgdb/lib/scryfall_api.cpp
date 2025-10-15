#include "scryfall_api.h"

// ScryfallAPI constructor
ScryfallAPI::
ScryfallAPI()
	: cli(SCRYFALL_API_ENDPOINT)
{
	// Blank Constructor
}

// Encode the URL such that it follows % encoding
// https://en.wikipedia.org/wiki/Percent-encoding
string ScryfallAPI::
url_encode(const string& str_value)
{
	// Loop through the str_value and change non-alphanumeric or unaccepted characters represented as their hex value with a %
	std::ostringstream oss;
	for (const char c : str_value)
	{
		if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~')
		{
			oss << c;
		}
		else
		{
			oss << '%' << std::uppercase << std::hex << setw(2) << setfill('0') << static_cast<int>(c);
		}
	}
	return oss.str();
}

// Format and submit the raw API call, enforcing rate limit.
string ScryfallAPI::
call_api(const string& query)
{
	// Ensure simultaneous API calls are not being made. 
	api_mutex.lock();

	// Make the raw API call
	if (auto res = cli.Get(query))
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