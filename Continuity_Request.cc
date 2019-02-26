#include "Continuity_Request.h"

string Continuity_Request::getNetworkMedia() {
	return network_media_;
}

string Continuity_Request::getContents() {
	return contents_;
}

void Continuity_Request::setNetworkMedia(string network_media) {
	network_media_ = network_media;
}

void Continuity_Request::setContents(string contents) {
	contents_ = contents;
}

bool Continuity_Request::isValid() {
    if(doc_.HasMember("Team") && doc_["Team"] == "SK2-2") {
        return true;
    }
    return false;
}

void Continuity_Request::deserializeClass(string JsonData) {
	JsonParser jsonparser(JsonData);
	setNetworkMedia(jsonparser.getString("NetworkMedia"));
	setContents(jsonparser.getString("Contents"));
}

void Continuity_Request::serializeClass(string contents) const {
    rapidjson::StringBuffer sb;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb); 
    writer.StartObject();
    writer.String("Type");
    writer.String("Request");
	writer.String("NetworkMedia");
	writer.String(network_media_);
	writer.String("Team");
	writer.String(contents_);
	writer.String("Contents");
	writer.String(contents);
    writer.EndObject();
} 

