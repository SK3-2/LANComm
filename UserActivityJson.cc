#include "LANComm.h"

UserActivity::UserActivity(string user_id, string activity_type, string device_type, string device_name): user_id_(user_id), activity_type_(activity_type), device_type_(device_type), device_name_(device_name) {} 

void UserActivity::setUserID(string user_id) {
	user_id_ = user_id;
} 
void UserActivity::setActivityType(string activity_type) {
	activity_type_ = activity_type;
}
void UserActivity::setDeviceType(string device_type) {
	device_type_ = device_type;
} 
void UserActivity::setDeviceName(string device_name) {
	device_name_ = device_name;
} 

string UserActivity::getClassName() {
	return class_name_;
}
string UserActivity::getUserID() {
	return user_id_;
} 
string UserActivity::getActivityType() {
	return activity_type_;
}
string UserActivity::getDeviceType() {
	return device_type_;
} 
string UserActivity::getDeviceName() {
	return device_name_;
} 

void UserActivity::deserializer(const char* JsonData) {
	JsonParser jsonparser(JsonData);
	setClassName(jsonparser.getString("ClassName"));
	setUserID(jsonparser.getString("UserID"));
	setActivityType(jsonparser.getString("ActivityType"));
	setDeviceType(jsonparser.getString("DeviceType"));
	setDeviceName(jsonparser.getString("DeviceName"));
}

template <typename Writer>
void UserActivity::serializer(Writer& writer) const {
	writer.String("ClassName");
	writer.String(class_name_);
	writer.String("UserID");
	writer.String(user_id_);
	writer.String("ActivityType");
	writer.String(activity_type_);
//	writer.String("DeviceInfo");
//	writer.StartObject();
	writer.String("DeviceType");
	writer.String(device_type_);
	writer.String("DeviceName");
	writer.String(device_name_);
//	writer.EndObject();
} 
