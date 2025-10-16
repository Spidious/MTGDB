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
			oss << '%' << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(c);
		}
	}
	return oss.str();
}

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
	size_t totalSize = size * nmemb;
	std::string* str = static_cast<std::string*>(userp);
	str->append(static_cast<char*>(contents), totalSize);
	return totalSize;
}

// Format and submit the raw API call, enforcing rate limit.
string ScryfallAPI::
call_api(const string& path)
{
	lock_guard<mutex> lock(api_mutex);

	ostringstream url;
	url << SCRYFALL_API_ENDPOINT << path;

	cout << "CALLING: " << url.str() << endl;

	CURL* curl = curl_easy_init();
	string response;

	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.str().c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

		struct curl_slist* headers = NULL;
		headers = curl_slist_append(headers, "User-Agent: mtgdb/1.0");
		headers = curl_slist_append(headers, "Accept: application/json");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

		CURLcode res = curl_easy_perform(curl);

		curl_slist_free_all(headers);
		curl_easy_cleanup(curl);

		if (res != CURLE_OK)
			return "Request failed";
	} else
		return "CURL init failed";

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