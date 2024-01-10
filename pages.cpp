#include "pages.h"

#include <sstream>
#include <fstream>
#include <streambuf>

namespace html
{

    PageWizard::PageWizard() : textHeader("HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: "),
                               imageHeader("HTTP/1.1 200 OK\nContent-Type: image/jpeg\nContent-Length: "),
                               cssHeader("HTTP/1.1 200 OK\nContent-Type: text/css\nContent-Length: "),
                               fileLocation("../frontend/")
    {
    }

    std::string PageWizard::loadHTMLFromFile(const std::string &filename)
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            return "<!DOCTYPE html><html lang=\"en\"><body><h1> ERROR 500 </h1><p> SERVER ERROR: CAN NOT LOAD THE HTML!</p></body></html>";
        }
        std::ostringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    std::string PageWizard::getHomepage(const std::string &cookie)
    {
        std::string homeHtml = loadHTMLFromFile(fileLocation + "homepage.html");
        std::ostringstream ss;
        ss << textHeader << homeHtml.size() << "\n\n"
           << homeHtml;
        std::string response = ss.str();
        return setUsername(cookie, response);

    }

    std::string PageWizard::getHomepageWithCookie(const std::string &cookie)
    {
        std::string homeHtml = loadHTMLFromFile(fileLocation + "homepage.html");
        std::ostringstream ss;
        std::string cookieWithAge = "user=" + cookie + "; Max-Age=864000";

        ss << textHeader << homeHtml.size() << "\n";
        ss << "Set-Cookie: " << cookieWithAge << "\n\n";
        ss << homeHtml;
        std::string response = ss.str();
        return setUsername(cookie, response);
    }

    std::string PageWizard::getDogPage()
    {
        std::string dogHtml = loadHTMLFromFile(fileLocation + "dogpage.html");
        std::ostringstream ss;
        ss << textHeader << dogHtml.size() << "\n\n"
           << dogHtml;
        return ss.str();
    }

    std::string PageWizard::getImage(std::string fileName)
    {
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
        std::string errorHtml = loadHTMLFromFile(fileLocation + "404.html");
        std::ostringstream ss;
        ss << textHeader << errorHtml.size() << "\n\n"
           << errorHtml;
        return ss.str();
    }

    std::string PageWizard::getCss()
    {
        std::ifstream cssFile(fileLocation + "styles.css");
        std::ostringstream cssBuffer;
        cssBuffer << cssFile.rdbuf();
        std::string cssContent = cssBuffer.str();

        std::ostringstream response;
        response << cssHeader << cssContent.size() << "\n\n"
                 << cssContent;

        return response.str();
    }
    std::string PageWizard::getIndex()
    {
        std::string indexHtml = loadHTMLFromFile(fileLocation + "index.html");
        std::ostringstream ss;
        ss << textHeader << indexHtml.size() << "\n\n"
           << indexHtml;
        return ss.str();
    }
    const std::string &PageWizard::setUsername(const std::string &username, std::string &page)
    {
        std::string cleanedUsername = username;
        size_t plusPos = cleanedUsername.find("+");
        if (plusPos != std::string::npos)
        {
            cleanedUsername.replace(plusPos, 1, " ");
        }
        
        std::string toFind = "#USER#";
        size_t startPos = page.find(toFind);
        if (startPos != std::string::npos)
        {
            page.replace(startPos, toFind.length(), cleanedUsername);
            return page;
        }
        else
        {
            return page;
        }
    }
}