#include "Util.h"

// Constructor
Util::Util(){
    consoleMessage = "";
}
// set console message
void Util::setConsoleMessage(std::string message){
    consoleMessage = message;
};
// get console message
std::string Util::getConsoleMessage(){
    return consoleMessage;
};
// Float to string
std::string Util::stringify(float x){
  std::ostringstream o;
  if (!(o << x)){
    //@TODO throw BadConversion("stringify(float)");
  }
  return o.str();
}

