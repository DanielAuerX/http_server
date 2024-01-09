#pragma once

#include <string>

namespace html
{
    class PageWizard
    {
    public:
        PageWizard();

        std::string loadHTMLFromFile(const std::string &filename);

        std::string getHomepage();

        std::string getDogPage();

        std::string getImage(std::string fileName);

        std::string get404Page();

        std::string getCss();

    private:
        const std::string textHeader;
        const std::string imageHeader;
        const std::string cssHeader;
        const std::string fileLocation;
    };
}