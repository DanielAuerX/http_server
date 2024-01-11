#pragma once

#include <string>

namespace utils
{

    class SwissArmyToolbox
    {
    public:
        SwissArmyToolbox();
        std::string loadTextFromFile(const std::string &filepath) const;
        std::string loadImageFromFile(const std::string &filepath) const;
        void replaceWith(std::string &text, const std::string &original, const std::string &replacement) const;
        void log(const std::string &message) const;
    };
}