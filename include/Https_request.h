#include<string>
#include<map>
using namespace std;


struct HttpRequest
{
    string method;
    string path;
    string version;

    map<string,string> headers;

    string body;
};