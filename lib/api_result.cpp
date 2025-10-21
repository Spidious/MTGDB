#include "scryfall_api.h"

APIResult::APIResult(std::string json) {
	this->update(nlohmann::json::parse(json));
}

APIResult& APIResult::operator<<(const string& str)
{
	// Update the contents with a parsed json
	try
	{
		this->update(nlohmann::json::parse(str));
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
	return *this;
}


string APIResult::operator[](const string& str)
{
	string o_string;
	try
	{
		o_string = static_cast<nlohmann::json&>(*this)[str].get<string>();
	}
	catch (const nlohmann::json::parse_error e)
	{
		throw invalid_argument(e.what());
	}
	catch (...)
	{
		throw invalid_argument("Unknown error");
	}
	return o_string;
}