#include "HandShakingOne.h"

void HandShakingOne::deserializeContents(string JsonData) {
  JsonParser jsonparser(JsonData.c_str());
  deviceInfo_ = {
    jsonparser.getString("UserID"),
    jsonparser.getString("DeviceID"),
    jsonparser.getString("PubKey"),
    jsonparser.getString("NetworkInfo")
  };
}

HandShakingOne::deviceInfo HandShakingOne::getDeviceInfo() {
  return deviceInfo_;
}

void HandShakingOne::serializeClass(string contents) const {
  rapidjson::StringBuffer sb;
  rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb); 
  writer.StartObject();
  writer.String("Type");
  writer.String("HandShakingOne");
  writer.String("NetworkMedia");
  writer.String(network_media_);
  writer.String("Team");
  writer.String("SK2-2");
  writer.String("Contents");
  writer.String(contents);
  writer.EndObject();
} 

string HandShakingOne::serializeContents() const {
  rapidjson::StringBuffer sb;
  rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb); 
  writer.String("DeviceInfo");
  writer.StartObject();
  writer.String("UserID");
  writer.String(deviceInfo_.userID_);
  writer.String("DeviceID");
  writer.String(deviceInfo_.deviceID_);
  writer.String("PublicKey");
  writer.String(deviceInfo_.pubKey_);
  writer.String("NetworkInfo");
  writer.String(deviceInfo_.networkInfo_);
  writer.EndObject();
  return sb.GetString();
} 
