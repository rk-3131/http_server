#include <sstream>
#include "../include/FileHandler.h"
#include "../include/Https_request.h"
#include "../include/HandleClient.h"
using namespace std;

HttpRequest parseRequest(const string &request)
{
    istringstream requestStream(request);

    HttpRequest http_request;

    requestStream >> http_request.method >> http_request.path >> http_request.version;

    if (http_request.method == "POST")
    {
        string line;
        getline(requestStream, line);
        while (getline(requestStream, line))
        {
            if (line == "\r" || line.empty())
            {
                break;
            }
            cout << "Header: " << line << endl;
        }
        getline(requestStream, http_request.body);
    }
    return http_request;
}
