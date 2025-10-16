#include "scryfall_api.h"

// Define the mutex
std::mutex ScryfallAPI::api_mutex;

// ScryfallAPI constructor
ScryfallAPI::
ScryfallAPI()
{
	// Initialize the curl client
	cli = curl_easy_init();
	if (!cli) {
		throw std::runtime_error("Failed to initialize CURL");
	}

	// Set required headers
	headers = NULL;
	headers = curl_slist_append(headers, "Accept: application/json");
	headers = curl_slist_append(headers, "User-Agent: mtgdb");
	curl_easy_setopt(cli, CURLOPT_HTTPHEADER, headers);
}

// ScryfallAPI destructor
ScryfallAPI::~ScryfallAPI() {
	// Clean up and free curl resources. 
	curl_slist_free_all(headers);
	curl_easy_cleanup(cli);
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
			oss << '%' << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(c);
		}
	}
	return oss.str();
}

// Format and submit the raw API call, enforcing rate limit.
string ScryfallAPI::
call_api(const string& path)
{
	// Lock the mutex to ensure thread safety and rate limiting
	lock_guard<mutex> lock(api_mutex);

	// Construct the full URL
	std::ostringstream oss;
	oss << SCRYFALL_API_ENDPOINT << path;

	// Construct the response string
	string response;

	// Call the API
	curl_easy_setopt(cli, CURLOPT_URL, oss.str().c_str());
	curl_easy_setopt(cli, CURLOPT_HTTPGET, 1L);
	//curl_easy_setopt(cli, CURLOPT_WRITEDATA, &response);
	CURLcode res = curl_easy_perform(cli);

	// Enforce the rate limit
	this_thread::sleep_for(chrono::milliseconds(SCRYFALL_API_DELAY_MS));
	return response;
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