#include "pages.h"

#include <sstream>
#include <fstream>
#include <streambuf>

namespace html
{

    PageWizard::PageWizard() : textHeader("HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: "),
                               imageHeader("HTTP/1.1 200 OK\nContent-Type: image/jpeg\nContent-Length: ")
    {
    }

    std::string PageWizard::getHomepage()
    {
        std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hellow from the server</p><p></p><button onclick=\"window.location.href='/dog'\" style=\"font-size: 20px; padding: 10px 20px;\">View cute dog</button></body></html>";
        std::ostringstream ss;
        ss << textHeader << htmlFile.size() << "\n\n"
           << htmlFile;
        return ss.str();
    }

    std::string PageWizard::getDogPage()
    {
        std::ostringstream htmlResponse;
        htmlResponse << "<!DOCTYPE html><html lang=\"en\"><body>";
        htmlResponse << "<h1> CUTEST DOGGY </h1><p> The cutest doggy world wide!</p>";
        htmlResponse << "<img src=\"cute_doggy.jpg\" alt=\"the cutest doggy\" style=\"width: 40%;\">";
        htmlResponse << "<p></p><button onclick=\"window.location.href='/'\" style=\"font-size: 20px; padding: 10px 20px;\">Return home</button></body></html>";
        std::ostringstream ss;
        ss << textHeader << htmlResponse.str().size() << "\n\n"
           << htmlResponse.str();
        return ss.str();
    }

    std::string PageWizard::getImage(std::string fileName)
    {
        // read image file content and respond with correct headers
        std::string filePath = "resources/" + fileName;
        std::ifstream imageFile(filePath, std::ios::binary);
        std::ostringstream imageBuffer;
        imageBuffer << imageFile.rdbuf();
        std::string imageContent = imageBuffer.str();

        std::ostringstream response;
        response << imageHeader << imageContent.size() << "\n\n";
        response << imageContent;

        return response.str();
    }

    std::string PageWizard::get404Page()
    {
        std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1> ERROR 404 </h1><p> THE PAGE YOU ARE LOOKING FOR DOES NOT EXIST!</p><button onclick=\"window.location.href='/'\">Home</button></body></html>";
        std::ostringstream ss;
        ss << textHeader << htmlFile.size() << "\n\n"
           << htmlFile;
        return ss.str();
    }

}