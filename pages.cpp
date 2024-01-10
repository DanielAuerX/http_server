#include "pages.h"

#include <sstream>
#include <fstream>
#include <streambuf>

namespace html
{

    PageWizard::PageWizard() : fileLocation("../frontend/")
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
        ss << getHeader("text/html", homeHtml.size()) << "\n\n" << homeHtml;
        std::string response = ss.str();
        return setUsername(cookie, response);
    }

    std::string PageWizard::getHomepageWithCookie(const std::string &cookie)
    {
        std::string homeHtml = loadHTMLFromFile(fileLocation + "homepage.html");
        std::ostringstream response;
        std::string cookieWithAge = "user=" + cookie + "; Max-Age=864000";

        response << getHeader("text/html", homeHtml.size()) << "\n";
        response << "Set-Cookie: " << cookieWithAge << "\n\n";
        response << homeHtml;
        std::string genericResponse = response.str();
        return setUsername(cookie, genericResponse);
    }

    std::string PageWizard::getDogPage()
    {
        std::string dogHtml = loadHTMLFromFile(fileLocation + "dogpage.html");
        std::ostringstream response;
        response << getHeader("text/html", dogHtml.size()) << "\n\n" << dogHtml;
        return response.str();
    }

    std::string PageWizard::getImage(std::string fileName)
    {
        std::string filePath = "resources/" + fileName;
        std::ifstream imageFile(filePath, std::ios::binary);
        std::ostringstream imageBuffer;
        imageBuffer << imageFile.rdbuf();
        std::string imageContent = imageBuffer.str();

        std::ostringstream response;
        response << getHeader("image/jpeg", imageContent.size()) << "\n\n";
        response << imageContent;

        return response.str();
    }

    std::string PageWizard::get404Page()
    {
        std::string errorHtml = loadHTMLFromFile(fileLocation + "404.html");
        std::ostringstream response;
        response << getHeader("text/html", errorHtml.size()) << "\n\n" << errorHtml;
        return response.str();
    }

    std::string PageWizard::getCss()
    {
        std::ifstream cssFile(fileLocation + "styles.css");
        std::ostringstream cssBuffer;
        cssBuffer << cssFile.rdbuf();
        std::string cssContent = cssBuffer.str();

        std::ostringstream response;
        response << getHeader("text/css", cssContent.size()) << "\n\n"
                 << cssContent;

        return response.str();
    }
    std::string PageWizard::getIndex()
    {
        std::string indexHtml = loadHTMLFromFile(fileLocation + "index.html");
        std::ostringstream response;
        response << getHeader("text/html", indexHtml.size()) << "\n\n" << indexHtml;
        return response.str();
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

    const std::string &PageWizard::getHeader(const std::string &type, const std::size_t &length)
    {
        std::ostringstream header;
        header << "HTTP/1.1 200 OK\nContent-Type: " << type << "\nContent-Length: " << length;
        return header.str();
    }
}