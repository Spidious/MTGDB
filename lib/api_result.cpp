#include "scryfall_api.h"

using namespace Scryfall;
using namespace std;
// ###################################################################
ScryfallObject::
ScryfallObject(const json& j) :
	json(j),
	obj_type(j["object"])
{
	
}

std::unique_ptr<ScryfallObject> ScryfallObject::
from_json(const string& json)
{
	return from_json(parse(json));
}

std::unique_ptr<ScryfallObject> ScryfallObject::from_json(const json &json)
{
	try
	{
		// Grab the object type
		const std::string obj_type = json["object"];
		// Check for object in which a class exists for it

		if (obj_type == "error")
			return std::make_unique<ScryError>(json);
		if (obj_type == "list")
		{
			return std::make_unique<ScryList>(json);
		}
		if (obj_type == "set")
			return std::make_unique<ScrySet>(json);
		if (obj_type == "card")
			return std::make_unique<ScryCard>(json);
		if (obj_type == "rulings")
			return std::make_unique<ScryRuling>(json);

		// Check for anything accepted but no object exists
		if (ObjectTypes.contains(obj_type))
			return std::make_unique<ScryfallObject>(json);

		// If the tread reaches this point then obj_type is invalid
		throw invalid_argument(obj_type);
	}
	catch (std::exception& e)
	{
		return generate_error(e.what(), -1);
	}
}

std::unique_ptr<ScryfallObject> ScryfallObject::
generate_error(const std::string err, const int status)
{
	// Create json Error and pass to ScryError
	std::ostringstream err_msg;
	err_msg << "{\"object\":\"error\",\"code\":\"exception\",\"status\":"
			<< status
			<< ", \"details\":\""
			<< err
			<< "\"}";
	return std::make_unique<ScryError>(parse(err_msg.str()));
}

std::string ScryfallObject::
get_object_type() const
{
	return obj_type;
}

std::string ScryfallObject::get_name() const
{
	return get_attr<std::string>("name");
}

ScryError::
ScryError(const json& j) : 
	ScryfallObject(j), 
	err_msg(j.value("description", "Unknown Error (Bad Description)"))
{

}

std::string ScryError::
what()
{
	// Build output "code (status): details"
	std::ostringstream o_err;
	o_err << (*this)["code"] << " ("
		<< (*this)["status"] << "): "
		<< (*this)["details"];

	// return the string
	return o_err.str();
}

ScryList::
ScryList(const json& j) :
	ScryfallObject(j)
{

	next_url = j.value("next_page", "");

	// Populate the dataset
	// Handle each item separately
	for (int i = 0; i < j["total_cards"]; i++) {
		std::shared_ptr<ScryfallObject> obj(from_json(j["data"][i]).release());
		data.push_back(obj);
	}
}

int ScryList::size() const
{
	return data.size();
}

std::shared_ptr<ScryfallObject> ScryList::at(const int index) const
{
	return data.at(index);
};

std::unique_ptr<ScryfallObject> ScryList::
call_next_list() const
{
	ScryfallAPI api;
	std::string n_page;
	api.call_api((*this)["next_page"], &n_page);

	return from_json(n_page);
}

std::string ScryCard::get_name() const {
	return (*this)["name"]; // TODO: This should be stored as a variable of the object
}


// ###################################################################
