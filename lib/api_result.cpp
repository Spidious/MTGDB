#include "scryfall_api.h"

APIResult::APIResult(const string json) :
	json(), 
	object_type(""), 
	status(0), 
	e_ptr(nullptr) 
{
	update_parse(json);
}

void APIResult::update_parse(const string str) {
	try {
		this->update(this->parse(str));
		object_type = (*this)["object"];
	}
	catch (const nlohmann::json::parse_error e)
	{
		// TODO: make this explain what happened
		string r = e.what();
		throw invalid_argument("Unable to parse JSON: " + r);
	}
	catch (...)
	{
		throw invalid_argument("Unknown error");
	}
}

APIResult& APIResult::operator<<(const string& str)
{
	// Update the contents with a parsed json
	update_parse(str);
	return *this;
}