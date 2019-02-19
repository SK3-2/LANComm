#include "LANComm.h"

UserIDJson::UserIDJson(string user_id): user_id_(user_id) {}

string UserIDJson::getUserID() {
	return user_id_;
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
	writer.String("UserID");
	writer.String(user_id_);
} 
