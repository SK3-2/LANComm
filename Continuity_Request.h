#define RAPIDJSON_HAS_STDSTRING 1

#ifndef Continuity_Request_H_
#define Continuity_Request_H_

#include "LANComm.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"

using namespace std;

class Continuity_Request {
  public:
    Continuity_Request() = default;
    //string getClassName();
    string getNetworkMedia();
    string getContents();
    //void setClassName(string);
    void setNetworkMedia(string);
    void setContents(string);
    bool isValid();
    virtual void serializeClass(string) const;
    virtual void serializContents(string, string, string, string) const;
    void deserializeClass(string);
    virtual void deserializeContents(string) = 0;
  protected:
    string network_media_;
    string team_ = "SK2-2";
    string contents_;
    rapidjson::Document doc_;
};

#endif
