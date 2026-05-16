#include "../include/MIME_type.h"

#include<string>
using namespace std;

string getContentType(const string& path) {

    if (path.find(".html") != string::npos)
        return "text/html";

    if (path.find(".css") != string::npos)
        return "text/css";

    if (path.find(".js") != string::npos)
        return "application/javascript";

    return "text/plain";
}