#include "scryfall_api.h"

// Define the mutex
std::mutex ScryfallAPI::api_mutex;

static size_t write_callback(char* contents, size_t size, size_t nmemb, void* userdata) {
	size_t totalSize = size * nmemb;
	std::string* response = static_cast<std::string*>(userdata);
	response->append(static_cast<char*>(contents), totalSize);
	return totalSize;
}

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
	curl_easy_setopt(cli, CURLOPT_HTTPGET, 1L);
	curl_easy_setopt(cli, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(cli, CURLOPT_HTTPHEADER, headers);
}

// ScryfallAPI destructor
ScryfallAPI::~ScryfallAPI() {
	// Clean up and free curl resources. 
	curl_slist_free_all(headers);
	curl_easy_cleanup(cli);
}

// Parse exceptions into API result
void ScryfallAPI::res_update(const exception* e, const int code)
{
	cout << "[ DEBUG ] Hit" << endl;
	ostringstream buffer;
	buffer << "{\"object\":\"error\", \"code\":\""
		   << code
		   << "\", \"status\":404, \"details\":\"Runtime Exception: "
		   << e->what() << "\"}";

	res << buffer.str();
}

void ScryfallAPI::res_update(const string& json) {

	try
	{
		res << json;
	}
	catch (invalid_argument e)
	{
		cerr << "Invalid json argument: " << e.what() << endl << json << endl;
		res_update(&e);
	}
	catch (...)
	{
		cerr << "Unable to update result" << endl;
		res_update(new exception());
	}
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

// Validate the scryfall search url query
bool ScryfallAPI::
validate_search(const string& query) 
{
	// Break apart query at the + markers
	// 
	return true;
}

// Format and submit the raw API call, enforcing rate limit.
CURLcode ScryfallAPI::
call_api(const string& path, const void* buffer)
{
	// Lock the mutex to ensure thread safety and rate limiting
	lock_guard<mutex> lock(api_mutex);

	// Construct the full URL
	std::ostringstream oss;
	oss << SCRYFALL_API_ENDPOINT << path;

	// Call the API
	curl_easy_setopt(cli, CURLOPT_URL, oss.str().c_str());
	curl_easy_setopt(cli, CURLOPT_WRITEDATA, buffer);
	cout << "[DEBUG] API Call: " << oss.str() << endl;
	CURLcode res = curl_easy_perform(cli);

	// Enforce the rate limit
	this_thread::sleep_for(chrono::milliseconds(SCRYFALL_API_DELAY_MS));
	return res;
}




// Search Scryfall by it's ID
void ScryfallAPI::
BasicSearch(const string& id) {
	string api_res;
	call_api(API_ENDPOINT_ID + id, &api_res);

	// Store the API result
	res_update(api_res);
}

void ScryfallAPI::
BasicSearch(const string& name, const string& type, const string& set , const int collect_num) {
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
	string api_res;
	call_api(API_ENDPOINT_SEARCH + url_encode(oss.str()), &api_res); // Error checking moved (May need to put this back in an if statement)

	// Store the API result
	res_update(api_res);
}

const APIResult& ScryfallAPI::
GetResult() const
{
	return this->res;
}


string ScryfallAPI::parse_query(vector<searchitem> queries) {
	// Base case (Check that queries is empty)
	if (!queries.size()) return "";

	// Grab back of the queries
	auto [field, op, value] = queries.back();
	queries.pop_back();

	// Validate Field and op (Both must exist
	if (!SearchFields.contains(field) || !SearchOps.contains(op))
	{
		// TODO: Actual error handling, not drop.
		return parse_query(queries); // Temporarily ignore invalid fields
	}

	// Create parsed query for output
	ostringstream parsed_query;

	// Append the field, op, and value to the query, deliminated by spaces in value
	char* token_value = strtok(value.data(), " ");
	while (token_value)
	{
		// Grab the next token
		char* next_token = strtok(nullptr, " ");

		// Append to query
		parsed_query << field << op << token_value;

		// Append a + if there exists a next token
		if (next_token)
			parsed_query << "+";

		// iterate current token to next
		token_value = next_token;
	}

	// Return the parsed query and recursively call this function
	return parse_query(queries) + ((queries.size()) ? "+" : "") + parsed_query.str();
}