#pragma once

#include "utils.h"
#include <string>

namespace html
{
    class PageWizard
    {
    public:
        PageWizard();

        std::string getHomepage(const std::string &cookie);

        std::string getHomepageWithCookie(const std::string &cookie);

        std::string getDogPage();

        std::string getImage(std::string fileName);

        std::string get404Page();

        std::string getCss();

        std::string getIndex();

    private:
        const std::string fileLocation;

        const utils::SwissArmyToolbox swissArmyToolbox;

        std::string setUsername(const std::string &username, std::string &page);

        std::string getHeader(const std::string &type, const std::size_t &length);
    };
}