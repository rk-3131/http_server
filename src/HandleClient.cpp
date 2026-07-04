#include "../include/HandleClient.h"
#include "../include/ParseRequest.h"
#include "../include/HttpResponse.h"

using namespace std;

void handleClient(int clientSocket)
{
    // this_thread::sleep_for(
    // chrono::seconds(5)
    // );
    cout << "Thread ID: "
         << this_thread::get_id()
         << endl;
    char buffer[4096] = {0};

    ssize_t bytesReceived =
        recv(clientSocket,
             buffer,
             sizeof(buffer),
             0);

    if (bytesReceived < 0)
    {
        cerr << "Receive failed\n";
        close(clientSocket);
        return;
    }

    // Convert request into string
    string request(buffer);

    cout << "\n========= REQUEST =========\n";
    cout << request << "\n";
    cout << "===========================\n";

    // Parse HTTP request
    HttpRequest http_request =
        parseRequest(request);

    if (http_request.method.empty() ||
        http_request.path.empty() ||
        http_request.version.empty())
    {
        close(clientSocket);
        return;
    }
    if (http_request.path == "/favicon.ico")
    {
        close(clientSocket);
        return;
    }

    if (http_request.method == "GET" &&
        http_request.path == "/api/hello")
    {
        string json =
            R"({
            "message": "Hello from C++ API"
        })";

        string response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: " +
            to_string(json.size()) +
            "\r\n"
            "\r\n" +
            json;

        send(clientSocket,
             response.c_str(),
             response.size(),
             0);

        close(clientSocket);
        return;
    }

    if (http_request.method == "POST")
    {

        cout << "POST Method detected" << endl;
        if (http_request.path == "/submit")
        {

            cout << "Body: " << http_request.body << endl;
            size_t pos = http_request.body.find("=");
            string username =
                http_request.body.substr(pos + 1);
            string html =
                "<h1>Hello " + username + " </h1>";

            string response =
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/html\r\n"
                "Content-Length: " +
                to_string(html.size()) +
                "\r\n"
                "\r\n" +
                html;

            send(clientSocket,
                 response.c_str(),
                 response.size(),
                 0);

            close(clientSocket);

            return;
        }
    }
    cout << "Method: " << http_request.method << endl;
    cout << "Path: " << http_request.path << endl;
    cout << "Version: " << http_request.version << endl;
    string filePath;

    if (http_request.path == "/")
    {
        filePath = "../public/index.html";
    }
    else
    {
        filePath = "../public" + http_request.path;
    }
    if (filePath.back() == '/')
    {
        close(clientSocket);
        return;
    }
    string body = readFile(filePath);
    if (body.empty())
    {

        body = "<h1>404 File Not Found</h1>";

        string response =
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: " +
            to_string(body.size()) + "\r\n"
                                     "\r\n" +
            body;

        send(clientSocket,
             response.c_str(),
             response.size(),
             0);

        close(clientSocket);
        return;
    }
    // Build HTTP response
    string contentType = getContentType(filePath);

    string response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: " +
        contentType + "; charset=UTF-8\r\n"
                      "Content-Length: " +
        to_string(body.size()) + "\r\n"
                                 "\r\n" +
        body;

    send(clientSocket,
         response.c_str(),
         response.size(),
         0);

    close(clientSocket);
}
