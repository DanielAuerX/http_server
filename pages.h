#pragma once

#include <string>

namespace html
{
    class PageWizard
    {
    public:
        PageWizard();

        std::string getHomepage();

        std::string getDogPage();

        std::string getImage(std::string fileName);

        std::string get404Page();

    private:
        const std::string textHeader;
        const std::string imageHeader;
    };
}