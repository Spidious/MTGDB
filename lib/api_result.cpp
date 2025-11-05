#include "scryfall_api.h"

using namespace Scryfall;
using namespace std;

// ###################################################################
ScryfallObject::ScryfallObject(const json& j) : json(j), obj_type((*this)["object"]) {}

std::unique_ptr<ScryfallObject> ScryfallObject::from_json(const string& json)
{
	try
	{
		// Grab the object type
		const std::string obj_type = parse(json)["object"];

		// Check for object in which a class exists for it
		if (obj_type == "error")
			return std::make_unique<ScryError>(parse(json));
		if (obj_type == "list")
			return std::make_unique<ScryList>(parse(json));
		if (obj_type == "set")
			return std::make_unique<ScrySet>(parse(json));
		if (obj_type == "card")
			return std::make_unique<ScryCard>(parse(json));
		if (obj_type == "rulings")
			return std::make_unique<ScryRuling>(parse(json));

		// Check for anything accepted but no object exists
		if (ObjectTypes.contains(obj_type))
			return std::make_unique<ScryfallObject>(parse(json));

		// If the tread reaches this point then obj_type is invalid
		throw invalid_argument(obj_type);
	}
	catch (std::exception& e)
	{
		// Create json Error and pass to ScryError
		std::ostringstream err_msg;
		err_msg << "{\"object\":\"error\",\"code\":\"exception\",\"status\":-1, \"details\":\""
			    << e.what()
				<< "\"}";
		return std::make_unique<ScryError>(err_msg.str());
	}
}

std::unique_ptr<ScryfallObject> ScryfallObject::generate_error(const exception& e, const int status)
{
	// Create json Error and pass to ScryError
	std::ostringstream err_msg;
	err_msg << "{\"object\":\"error\",\"code\":\"exception\",\"status\":"
			<< status
			<< ", \"details\":\""
			<< e.what()
			<< "\"}";
	return std::make_unique<ScryError>(err_msg.str());
}

std::string ScryfallObject::get_object_type() const
{
	return obj_type;
}

// ###################################################################



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

