#include "LANComm.h"

Response::Response(string public_key, string device_id):public_key_(public_key), device_id_(device_id) {}

string Response::getPubKey() {
	return public_key_;
}

string Response::getDeviceID() {
	return device_id_;
}

string Response::getClassName() {
	return class_name_;
}

void Response::setPubKey(string public_key) {
	public_key_ = public_key;
}

void Response::setDeviceID(string device_id) {
	device_id_ = device_id;
}

void Response::deserializer(const char* JsonData) {
	JsonParser jsonparser(JsonData);
	setPubKey(jsonparser.getString("PubKey"));
	setDeviceID(jsonparser.getString("DeviceID"));
}

template <typename Writer>
void Response::serializer(Writer& writer) const {
	writer.String("ClassName");
	writer.String(class_name_);
	writer.String("PubKey");
	writer.String(public_key_);
	writer.String("DeviceID");
	writer.String(device_id_);
} 
