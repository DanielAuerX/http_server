#include "../include/pages.h"

#include <sstream>
#include <streambuf>

namespace html
{

    PageWizard::PageWizard() : fileLocation("../frontend/"), swissArmyToolbox(utils::SwissArmyToolbox())
    {
    }

    std::string PageWizard::getHomepage(const std::string &cookie)
    {
        std::string homeHtml = swissArmyToolbox.loadTextFromFile(fileLocation + "homepage.html");
        std::ostringstream ss;
        ss << getHeader("text/html", homeHtml.size()) << "\n\n"
           << homeHtml;
        std::string response = ss.str();
        return setUsername(cookie, response);
    }

    std::string PageWizard::getHomepageWithCookie(const std::string &cookie)
    {
        std::string homeHtml = swissArmyToolbox.loadTextFromFile(fileLocation + "homepage.html");
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
        std::string dogHtml = swissArmyToolbox.loadTextFromFile(fileLocation + "dogpage.html");
        std::ostringstream response;
        response << getHeader("text/html", dogHtml.size()) << "\n\n"
                 << dogHtml;
        return response.str();
    }

    std::string PageWizard::getImage(std::string fileName)
    {
        std::string filePath = "resources/" + fileName;
        std::string imageContent = swissArmyToolbox.loadTextFromFile(filePath);

        std::ostringstream response;
        response << getHeader("image/jpeg", imageContent.size()) << "\n\n"
                 << imageContent;

        return response.str();
    }

    std::string PageWizard::get404Page()
    {
        std::string errorHtml = swissArmyToolbox.loadTextFromFile(fileLocation + "404.html");
        std::ostringstream response;
        response << getHeader("text/html", errorHtml.size()) << "\n\n"
                 << errorHtml;
        return response.str();
    }

    std::string PageWizard::getCss()
    {
        std::string cssContent = swissArmyToolbox.loadTextFromFile(fileLocation + "styles.css");

        std::ostringstream response;
        response << getHeader("text/css", cssContent.size()) << "\n\n"
                 << cssContent;

        return response.str();
    }
    std::string PageWizard::getIndex()
    {
        std::string indexHtml = swissArmyToolbox.loadTextFromFile(fileLocation + "index.html");
        std::ostringstream response;
        response << getHeader("text/html", indexHtml.size()) << "\n\n"
                 << indexHtml;
        return response.str();
    }

    std::string PageWizard::setUsername(const std::string &username, std::string &page)
    {
        std::string cleanedUsername = username;
        swissArmyToolbox.replaceWith(cleanedUsername, "+", " ");
        swissArmyToolbox.replaceWith(page, "#USER#", cleanedUsername);
        return page;
    }

    std::string PageWizard::getHeader(const std::string &type, const std::size_t &length)
    {
        std::ostringstream header;
        header << "HTTP/1.1 200 OK\nContent-Type: " << type << "\nContent-Length: " << length;
        return header.str();
    }
}