#include<string>
using namespace std;


class HttpResponse
{
public:

    int statusCode;

    string contentType;

    string body;

    string build(){
        return body;
    }
};