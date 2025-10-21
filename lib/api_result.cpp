#include "scryfall_api.h"

APIResult::APIResult(const string json) {
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
		throw invalid_argument(e.what());
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