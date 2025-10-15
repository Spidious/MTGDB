#include "scryfall_api.h"

// Define the mutex
std::mutex ScryfallAPI::api_mutex;

// ScryfallAPI constructor
ScryfallAPI::
ScryfallAPI()
	//: cli(SCRYFALL_API_ENDPOINT)
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
		if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~' || c == '+')
		{
			oss << c;
		}
		else
		{
			; // oss << '%' << std::uppercase << std::hex << setw(2) << setfill('0') << static_cast<int>(c);
		}
	}
	return oss.str();
}

// Format and submit the raw API call, enforcing rate limit.
string ScryfallAPI::
call_api(const string& path)
{
	// Ensure simultaneous API calls are not being made. 
	lock_guard<mutex> lock(api_mutex);

	// Make the raw API call
	//auto res = cli.Get(path);

	// Check output
	// TODO: Exceptions needed
	/*if (res && res->status == 200)
		std::cout << res->body << std::endl;
	else
		std::cout << "Request failed\n";*/

	// Enforce Scryfall API rate limit
	this_thread::sleep_for(chrono::milliseconds(SCRYFALL_API_DELAY_MS));

	// Return the body of the response
	return "res->body";
}

// Search Scryfall by it's ID
string ScryfallAPI::
BasicSearch(const string& id) {
	return call_api(API_ENDPOINT_ID + id);
}

string ScryfallAPI::
BasicSearch(const string& name, const string& type, const string& set, const int collect_num) {
	// Start search stream
	std::ostringstream oss;
	// Add name and type searches
	oss << "name:" << name
		<< "+type:" << type;

	// Add optional searches
	if (set != "")
		oss << "+set:" << set;
	if (collect_num > 0)
		oss << "+number:" << collect_num;

	// Call and return the result of the API call.
	return call_api(API_ENDPOINT_SEARCH + url_encode(oss.str()));
}

string ScryfallAPI::
AdvancedSearch(const string& query) {
	return ""; // Placeholder Implementation
}