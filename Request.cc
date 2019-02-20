#include "LANComm.h"

Request::Request(string user_id): user_id_(user_id) {}

string Request::getUserID() {
	return user_id_;
}

string Request::getClassName() {
	return class_name_;
}

void Request::setUserID(string user_id) {
	user_id_ = user_id;
}

void Request::deserializer(const char* JsonData) {
	JsonParser jsonparser(JsonData);
	setUserID(jsonparser.getString("UserID"));
}

template <typename Writer>
void Request::serializer(Writer& writer) const {
	writer.String("ClassName");
	writer.String(class_name_);
	writer.String("UserID");
	writer.String(user_id_);
} 
