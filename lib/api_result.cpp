#include "scryfall_api.h"

// Initialize member variables and update json base
APIResult::APIResult(const string& json) :
	nlohmann::json(),
	status(0), 
	e_ptr(nullptr) 
{
	update_parse(json);
}

// Primary method for updating the contents of the object.
void APIResult::update_parse(const string& str) {
	if (str.empty() || str == "{}") {
		// This will happen upon creation of the object unless a parameter is used in the constructor
		return; 
	}

	try {
		// Attempt to parse the JSON and update the body of the object
		this->update(APIResult::parse(str));
		const auto obj = (*this)["object"];
		object_type = (obj == NULL) ? "" : obj;
	}
	catch (const nlohmann::json::parse_error e)
	{
		// Failure to parse the json
		string r = e.what();
		throw invalid_argument("Unable to parse JSON: " + r);
	}
	catch (const std::exception& e)
	{
		// Unknown exception failure
		string r = e.what();
		throw invalid_argument("Unable to handle parameter >> " + r);
	}
	catch (...)
	{
		// Unknown exception.
		throw invalid_argument("Unknown parse/update error");
	}
}

APIResult& APIResult::operator<<(const string& str)
{
	// Update the contents with a parsed json object
	update_parse(str);
	return *this;
}

