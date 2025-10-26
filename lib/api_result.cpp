#include "scryfall_api.h"

APIResult::APIResult(const string& json) :
	nlohmann::json(),
	status(0), 
	e_ptr(nullptr) 
{
	update_parse(json);
}

void APIResult::update_parse(const string& str) {
	if (str.empty() || str == "{}") {
		// TODO: build a log to handle this
		// Do not crash the code.
		cerr << "Cannot parse string: " << str << endl;
		return; 
	}

	try {
		this->update(APIResult::parse(str));
		const auto obj = (*this)["object"];
		object_type = (obj == NULL) ? "" : obj;
	}
	catch (const nlohmann::json::parse_error e)
	{
		// TODO: make this explain what happened
		string r = e.what();
		throw invalid_argument("Unable to parse JSON: " + r);
	}
	catch (const std::exception& e)
	{
		// TODO: Build a log to handle this
		cerr << e.what() << endl;
	}
	catch (...)
	{
		throw invalid_argument("Unknown parse/update error");
	}
}

APIResult& APIResult::operator<<(const string& str)
{
	// Update the contents with a parsed json
	update_parse(str);
	return *this;
}