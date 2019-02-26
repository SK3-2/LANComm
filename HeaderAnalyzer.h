#include "LANComm.h"

class HeaderAnalyzer {
  public:
    HeaderAnalyzer() = default;
    void analyzeHeader(string Message) {
      jsonparser(Message.c_str());
      if(!jsonparser.getString("Type").compare("") {
         
      }
      if(!jsonparser.getString("Type").compare("HandShakingOne") {
        HandShakinOne.deserializeClass(Message)         
      }
      if(!jsonparser.getString("Type").compare("HandShakingTwo") {
        
      }
      if(!jsonparser.getString("Type").compare("UserActivity") {
        
      }
    } 
    string
  private:
    Document doc_;
    JsonParser jsonparser;

};
