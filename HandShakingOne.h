#ifndef HANDSHAKING_H_
#define HANDSHAKING_H_

#include "Continuity_Request.h"

class HandShakingOne: public Continuity_Request {
    public:
        struct deviceInfo {
            string userID_;
            string deviceID_;
            string pubKey_;
            string networkInfo_;
        };
    private:
        deviceInfo deviceInfo_;
    public:
        HandShakingOne() = default;
        void deserializeContents(string);
        string serializeContents() const;
        void serializeClass(string) const;
        deviceInfo getDeviceInfo();
        void setUserID(string);
        void setDeviceID(string);
        void setPubKey(string);
        void setNetworkInfo(string);
};

#endif
