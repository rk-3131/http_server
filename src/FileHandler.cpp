#include "../include/FileHandler.h"

#include <fstream>
#include <iterator>

using namespace std;

string readFile(const string& filePath)
{
    ifstream file(filePath);

    if (!file.is_open()) {
        return "";
    }

    string content(
        (istreambuf_iterator<char>(file)),
        istreambuf_iterator<char>()
    );

    return content;
}