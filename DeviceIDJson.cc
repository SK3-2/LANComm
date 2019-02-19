#include "LANComm.h"

DeviceIDJson::DeviceIDJson(string device_id): device_id_(device_id) {}

string DeviceIDJson::getDeviceID() {
	return device_id_;
}

string DeviceIDJson::getClassName() {
	return class_name_;
}

void DeviceIDJson::setDeviceID(string device_id) {
	device_id_ = device_id;
}

void DeviceIDJson::deserializer(const char* JsonData) {
	JsonParser jsonparser(JsonData);
	setDeviceID(jsonparser.getString("DeviceID"));
}

template <typename Writer>
void DeviceIDJson::serializer(Writer& writer) const {
	writer.String("ClassName");
	writer.String(class_name_);
	writer.String("DeviceID");
	writer.String(device_id_);
} 
