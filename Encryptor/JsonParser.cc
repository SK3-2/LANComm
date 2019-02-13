#include "rapidjson/document.h"
#include "rapidjson/rapidjson.h"
#include <string>
#include <iostream>
using std::string;

class JsonParser
{
public:

  JsonParser(const char* Message){
    doc.Parse(Message);
  };
  
  bool hasMember(const char* keyString){
    if(doc.HasMember(keyString)) 
      return true;
    return false;    
  }

  string getString(const char* keyString){
    if (!hasMember(keyString)){
      return "";
    }
    
    rapidjson::Value& value = doc[keyString];

    if (value.IsString()){
      return value.GetString();
    }
    else if(value.IsNumber()){
      return std::to_string(value.GetDouble());         
     }
    else{
      return "";
    }
  }

private:
  rapidjson::Document doc;

};

int main(void){
  const char* msg = "{\"hello\":\"world\"}";
  JsonParser A(msg);
  std::cout<<"hello"<<std::endl;
  std::cout<<A.getValue("hello")<<std::endl;
  return 0;
}


