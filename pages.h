#pragma once

#include <string>
#include <sstream>

namespace html
{
    std::string getHomepage()
    {
        std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hellow from the server</p><button onclick=\"window.location.href='/otherpage'\">Leave home</button></body></html>";
        std::ostringstream ss;
        ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n"
           << htmlFile;
        return ss.str();
    }

    std::string getOtherPage()
    {
        std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1> OTHER PAGE </h1><p> This is another page!</p><button onclick=\"window.location.href='/'\">Return home</button></body></html>";
        std::ostringstream ss;
        ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n"
           << htmlFile;
        return ss.str();
    }

    std::string get404Page()
    {
        std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1> ERROR 404 </h1><p> THE PAGE YOU ARE LOOKING FOR DOES NOT EXIST!</p><button onclick=\"window.location.href='/'\">Return home</button></body></html>";
        std::ostringstream ss;
        ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n"
           << htmlFile;
        return ss.str();
    }
}