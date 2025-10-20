#include "scryfall_api.h"

APIResult::APIResult(std::string json) {
	this->update(nlohmann::json::parse(json));
} 