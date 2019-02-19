#include "LANComm.h"

UserIDJson::UserIDJson(string user_id): user_id_(user_id) {}

string UserIDJson::getUserID() {
	return user_id_;
}

string UserIDJson::getClassName() {
	return class_name_;
}

void UserIDJson::setUserID(string user_id) {
	user_id_ = user_id;
}

void UserIDJson::deserializer(const char* JsonData) {
	JsonParser jsonparser(JsonData);
	setUserID(jsonparser.getString("UserID"));
}

template <typename Writer>
void UserIDJson::serializer(Writer& writer) const {
	writer.String("ClassName");
	writer.String(class_name_);
	writer.String("UserID");
	writer.String(user_id_);
} 
