#include "LANComm.h"

JsonParser::JsonParser() {

}
JsonParser::JsonParser(const char* Message) {
	doc_.Parse(Message);
}

void JsonParser::setMessage(const char* Message) {
	doc_.Parse(Message);
}

bool JsonParser::hasMember(const char* keyString) {
	if(doc_.HasMember(keyString)) 
		return true;
	return false;    
}

string JsonParser::getString(const char* keyString) {

	if (!hasMember(keyString)){
		return "";
	}

	rapidjson::Value& value = doc_[keyString];

	if (value.IsString()){
		return value.GetString();
	}
	else if(value.IsNumber()){
		return std::to_string(value.GetDouble());         
	}
	else{
		return "";
	}
}

