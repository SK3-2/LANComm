#include "LANComm.h"


PubKeyJson::PubKeyJson(string public_key):public_key_(public_key) {}

string PubKeyJson::getPubKey() {
	return public_key_;
}

string PubKeyJson::getClassName() {
	return class_name_;
}

void PubKeyJson::setPubKey(string public_key) {
	public_key_ = public_key;
}

void PubKeyJson::deserializer(const char* JsonData) {
	JsonParser jsonparser(JsonData);
	setPubKey(jsonparser.getString("PubKey"));
}

template <typename Writer>
void PubKeyJson::serializer(Writer& writer) const {
	writer.String("ClassName");
	writer.String(class_name_);
	writer.String("PubKey");
	writer.String(public_key_);
} 
