#include "../include/HandleClient.h"

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
    istringstream requestStream(request);

    string method;
    string path;
    string version;

    requestStream >> method >> path >> version;
    if (method.empty() ||
        path.empty() ||
        version.empty())
    {
        close(clientSocket);
        return;
    }
    if (path == "/favicon.ico")
    {
        close(clientSocket);
        return;
    }
    if (method == "GET" &&
        path == "/api/hello")
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
    if (method == "POST")
    {
        cout << "POST Method detected" << endl;
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
        string body;
        getline(requestStream, body);
        cout << "Body: " << body << endl;
        size_t pos = body.find("=");
        string username =
            body.substr(pos + 1);
        if (path == "/submit")
        {
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
    cout << "Method: " << method << endl;
    cout << "Path: " << path << endl;
    cout << "Version: " << version << endl;
    string filePath;

    if (path == "/")
    {
        filePath = "../public/index.html";
    }
    else
    {
        filePath = "../public" + path;
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

