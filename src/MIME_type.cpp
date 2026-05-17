#include "../include/MIME_type.h"

#include<string>
using namespace std;

string getContentType(const string& path) {

    if (path.find(".html") != string::npos){
        return "text/html";
    }

    if (path.find(".css") != string::npos){
        return "text/css";
    }
        
    if (path.find(".js") != string::npos){
        return "application/javascript";
    }
        
    if (path.find(".jpg") != string::npos || path.find(".jpeg") != string::npos){
        return "image/jpeg";
    }
        
    if (path.find(".png") != string::npos){
        return "image/png";
    }

    if (path.find(".gif") != string::npos){
        return "image/gif";
    }

    if (path.find(".svg") != string::npos){
        return "image/svg";
    }

    if (path.find(".webp") != string::npos){
        return "image/webp";
    }

    if(path.find(".json") != string::npos){
        return "application/json";
    }

    return "text/plain";
}