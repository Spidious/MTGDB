#include "scryfall_api.h"

using namespace Scryfall;
using namespace std;

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
	if (str.empty()) {
		// This will happen upon creation of the object unless a parameter is used in the constructor
		throw std::invalid_argument("Empty string :: Must contain \"object\"");
	}

	try {
		// Attempt to parse the JSON and update the body of the object
		this->clear();
		this->update(APIResult::parse(str));
		const auto obj = (*this)["object"];
		const std::string tst_obj = obj;
		if (obj.is_null() || !ObjectTypes.contains(obj)) throw std::invalid_argument("Invalid/NULL object found in JSON"); // Assert that there must be a valid object in the JSON
		object_type = obj;
	}
	catch (const exception e)
	{
		e_handle_exception(e);
	}
	catch (...)
	{
		// Unknown exception.
		auto e = std::runtime_error("Unknown parse error");
		e_handle_exception(e);
	}
}

void APIResult::e_handle_exception(const std::exception& e)
{
	// Update body with json error
	ostringstream buffer;
	buffer << "{\"object\":\"error\", \"code\":\"exception\", \"status\":-1, \"details\":\"Runtime Exception: "
		   << e.what() << "\"}";
	update_parse(buffer.str());

	// Update status with -1
	status = -1;
	e_ptr = &e;
}

int APIResult::getStatus() const
{
	return status;
}

const std::exception* APIResult::getException() const
{
	return e_ptr;
}

string APIResult::data() const
{
	return this->dump();
}

APIResult& APIResult::operator<<(const string& str)
{
	// Update the contents with a parsed json object
	update_parse(str);
	return *this;
}

